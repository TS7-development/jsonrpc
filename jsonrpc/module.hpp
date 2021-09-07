#pragma once

#include <functional>
#include <map>
#include <string>

#include <boost/json.hpp>

#include "error.hpp"
#include "error/error.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId>
    class Module {
      public:
        using procedure_t = std::function<boost::json::object(const boost::json::object&)>;

        procedure_t& operator[](const std::string& method) {
          return procedures[method];
        }

        boost::json::object operator()(const boost::json::object& request) {
          TId id;

          if (!request.contains("id")) {
            return error(id, error::IdMissing());
          }

          const boost::json::value& id_value = request.at("id");
          id = id_conv(id_value);

          if (!request.contains("method")) {
            return error(id, error::MethodMissing());
          }

          const boost::json::value& method_value = request.at("method");
          if (util::GetJsonType(method_value) != util::JsonType::STRING) {
            return error(id, error::MethodNotAString(util::GetJsonType(method_value)));
          }

          std::string method = method_conv(method_value);
          if (procedures.find(method) == procedures.end()) {
            return error(id, error::MethodNotFound(method));
          }

          procedure_t procedure = procedures[method];
          return procedure(request);
        }

      protected:
        util::FromJson<TId> id_conv;
        util::FromJson<std::string> method_conv;
        Error<TId> error;
        std::map<std::string, procedure_t> procedures;
    };
  }
}
