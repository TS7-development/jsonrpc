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
        using id_t = TId;
        using procedure_t = std::function<boost::json::value(const boost::json::object&)>;

        inline procedure_t& operator[](const std::string& method) {
          return procedures[method];
        }

        inline boost::json::value operator()(const boost::json::object& request) {
          if ( request.contains("id") ) {
            return handleRequest(request);
          }

          return handleNotification(request);
        }

      protected:
        boost::json::value handleRequest(const boost::json::object& request) {
          TId id;

          if (!request.contains("id")) {
            return error(id, error::IdMissing());
          }

          const boost::json::value& id_value = request.at("id");
          typename util::FromJson<TId>::conversion_failure converted_id = id_conv(id_value);
          if ( !converted_id ) {
            return converted_id.getFailed();
          }

          id = converted_id.getSuccess();

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

        boost::json::value handleNotification(const boost::json::object& notification) {
          if ( !notification.contains("method") ) {
            // Missing field method
            return boost::json::value();
          }

          const boost::json::value& method_value = notification.at("method");
          if (util::GetJsonType(method_value) != util::JsonType::STRING) {
            // Method is not a string
            return boost::json::value();
          }

          std::string method = method_conv(method_value);
          if (procedures.find(method) == procedures.end()) {
            // Method not found
            return boost::json::value();
          }

          procedure_t procedure = procedures[method];
          return procedure(notification);
        }

        util::FromJson<TId> id_conv;
        util::FromJson<std::string> method_conv;
        Error<TId> error;
        std::map<std::string, procedure_t> procedures;
    };
  }
}
