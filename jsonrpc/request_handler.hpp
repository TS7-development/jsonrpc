#pragma once

#include <functional>

#include "parameter.hpp"
#include "error/error.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId, typename TRet, typename... TArgs>
    class RequestHandler {
      public:
        using maybe_failed = error::maybe_failed<TRet, error::ErrorCode>;
        using spec_failure = error::maybe_failed<std::string, error::ErrorCode>;
        using callback_t = std::function<maybe_failed(const TId& id, TArgs... args)>;
        using tuple_t = std::tuple<Parameter<TArgs>...>;

        template <typename... UArgs>
        RequestHandler(callback_t callback, UArgs... args)
          : callback(callback),
            parameter(Parameter<TArgs>(args)...)
        {}

        maybe_failed operator()(const boost::json::object& request, TId& parsedId) {
          if (!request.contains("jsonrpc")) {
            return error::JsonrpcMissing();
          }

          const boost::json::value& jsonrpc = request.at("jsonrpc");
          if (!jsonrpc.is_string()) {
            return error::JsonrpcNotAString(util::GetJsonType(jsonrpc));
          }

          const std::string& spec = jsonrpc.as_string().c_str();
          if ("2.0" != spec) {
            return error::JsonrpcUnknownSpecification(spec);
          }

          if (!request.contains("id")) {
            return error::IdMissing();
          }

          const boost::json::value& id = request.at("id");
          util::JsonType id_type = util::GetJsonType(id);
          if (!util::AsJson<TId>::IsType(id_type)) {
            return error::IdWrongType<TId>(id_type);
          }

          util::FromJson<TId> id_value;
          parsedId = id_value(id);

          if (!request.contains("method")){
            return error::MethodMissing();
          }

          const boost::json::value& method = request.at("method");
          if (!method.is_string()) {
            return error::MethodNotAString(util::GetJsonType(method));
          }

          if (!request.contains("params")) {
            return error::ParamsMissing();
          }

          const boost::json::value& params = request.at("params");
          if (!params.is_object()) {
            return error::ParamsNotAnObject(util::GetJsonType(params));
          }

          const boost::json::object& paramsObj = params.as_object();
          maybe_failed applied = apply(parsedId, paramsObj, std::make_index_sequence<std::tuple_size<tuple_t>::value>{});
          if (!applied) {
            return applied;
          }

          return static_cast<TRet>(applied);
        }

        maybe_failed operator()(const boost::json::object& request) {
          [[maybe_unused]] TId id;
          return (*this)(request, id);
        }

      protected:
        callback_t callback;
        tuple_t parameter;

        /**
         * @brief Function pointer execution
         *
         * Parses all parameter and executes the stored function pointer.
         *
         * @tparam Is Index sequence.
         *
         * @param o Const reference to the JSON object that shall be parsed.
         *
         * @return Returns the result of the stored function pointer.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <std::size_t... Is>
        maybe_failed apply(const TId& id, const boost::json::object& o, std::index_sequence<Is...>) {
           return callback( id, std::get<Is>(parameter).load(o) ... );
        }
    };
  }
}
