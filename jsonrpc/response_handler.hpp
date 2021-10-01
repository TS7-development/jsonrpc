#pragma once

#include <functional>

#include "error/error.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId, typename TResult>
    class ResponseHandler {
      public:
        using maybe_failed = error::maybe_failed<void, error::ErrorCode>;
        using callback_t = std::function<void(const TId&, const TResult&)>;

        constexpr inline ResponseHandler(callback_t callback)
          : callback(callback)
        {}

        maybe_failed operator()(const boost::json::object& object) {
          if (!object.contains("jsonrpc")) {
            return error::JsonrpcMissing();
          }

          const boost::json::value& jsonrpc = object.at("jsonrpc");
          if (!jsonrpc.is_string()) {
            return error::JsonrpcNotAString(util::GetJsonType(jsonrpc));
          }

          const std::string& spec = jsonrpc.as_string().c_str();
          if ("2.0" != spec) {
            return error::JsonrpcUnknownSpecification(spec);
          }

          if (!object.contains("id")) {
            return error::IdMissing();
          }

          const boost::json::value& id = object.at("id");
          util::JsonType id_type = util::GetJsonType(id);
          if (!util::AsJson<TId>::IsType(id_type)) {
            return error::IdWrongType<TId>(id_type);
          }

          if (!object.contains("result")) {
            return error::ResultMissing();
          }

          const boost::json::value& result = object.at("result");
          util::FromJson<TId> id_value;
          util::FromJson<TResult> result_value;

          callback(id_value(id), result_value(result));
          return maybe_failed();
        }

      protected:
        callback_t callback;
    };
  }
}
