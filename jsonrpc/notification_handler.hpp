#pragma once

#include <functional>

#include "parameter.hpp"
#include "error/error.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename... TArgs>
    class NotificationHandler {
      public:
        using maybe_failed = error::maybe_failed<void, error::ErrorCode>;
        using spec_failure = error::maybe_failed<std::string, error::ErrorCode>;
        using callback_t = std::function<maybe_failed(TArgs... args)>;
        using tuple_t = std::tuple<Parameter<TArgs>...>;

        template <typename... UArgs>
        NotificationHandler(callback_t callback, UArgs... args)
          : callback(callback),
            parameter(Parameter<TArgs>(args)...)
        {}

        maybe_failed operator()(const boost::json::object& notification) {
          if (!notification.contains("jsonrpc")) {
            return error::JsonrpcMissing();
          }

          const boost::json::value& jsonrpc = notification.at("jsonrpc");
          if (!jsonrpc.is_string()) {
            return error::JsonrpcNotAString(util::GetJsonType(jsonrpc));
          }

          const std::string& spec = jsonrpc.as_string().c_str();
          if ("2.0" != spec) {
            return error::JsonrpcUnknownSpecification(spec);
          }

          if (!notification.contains("method")) {
            return error::MethodMissing();
          }

          const boost::json::value& method = notification.at("method");
          if (!method.is_string()) {
            return error::MethodNotAString(util::GetJsonType(method));
          }

          if (!notification.contains("params")) {
            return error::ParamsMissing();
          }

          const boost::json::value& params = notification.at("params");
          if (!params.is_object()) {
            return error::ParamsNotAnObject(util::GetJsonType(params));
          }

          const boost::json::object& paramsObj = params.as_object();
          maybe_failed applied = apply(paramsObj, std::make_index_sequence<std::tuple_size<tuple_t>::value>{});
          if (!applied) {
            return applied;
          }

          return maybe_failed();
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
        maybe_failed apply(const boost::json::object& o, std::index_sequence<Is...>) {
           return callback( std::get<Is>(parameter).load(o) ... );
        }
    };
  }
}
