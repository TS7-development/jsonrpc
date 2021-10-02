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

        inline boost::json::value operator()(const boost::json::object& request) {
          error::maybe_failed<TId, boost::json::object> id = getID(request);

          if (!request.contains("method")) {
            if ( id ) {
              // Seems to be a request
              return error(id, error::MethodMissing());
            }
            else {
              // Seems to be a notification
              return boost::json::value();
            }
          }

          const boost::json::value& method_value = request.at("method");
          if (util::GetJsonType(method_value) != util::JsonType::STRING) {
            if ( id ) {
              // Seems to be a request
              return error(id, error::MethodNotAString(util::GetJsonType(method_value)));
            }
            else {
              // Seems to be a notification
              return boost::json::value();
            }
          }

          boost::json::value jsonrpc_check = ensureJsonrpc(request, id);

          std::string method = str_conv(method_value);
          const Entry& entry = procedures[method];
          if ( !entry.isValid() && fallback) {
            // Do not perform checks in this case
            // fallback is fully in charge of it
            return fallback(request);
          }

          if ( !id && request.contains("id")) {
            return id.getFailed();
          }

          if ( !entry.isValid() ) {
            // We know already that we do not have a fallback
            return error(id, error::MethodNotFound(method));
          }

          if ( entry.requiresID() ) {
            // Request handling
            if ( id ) {
              if ( !jsonrpc_check.is_null()) {
                // Ensure that handler is only called on valid jsonrpc
                return jsonrpc_check;
              }

              return handleRequest(request, id.getSuccess(), entry);
            }


            return id.getFailed();
          }


          // Notification handling
          if ( !jsonrpc_check.is_null()) {
            // Ensure that handler is only called on valid jsonrpc
            return jsonrpc_check;
          }

          return handleNotification(request, entry);
        }

        inline void addRequest(const std::string& name, procedure_t procedure) {
          procedures[name] = Entry::Request(procedure);
        }

        inline void addNotification(const std::string& name, procedure_t procedure) {
          procedures[name] = Entry::Notification(procedure);
        }

        inline void setFallback(procedure_t procedure) {
          fallback = procedure;
        }

      protected:
        struct Entry {
            inline Entry() = default;
            inline Entry(const Entry&) = default;
            inline Entry(Entry&&) = default;

            Entry& operator=(const Entry&) = default;
            Entry& operator=(Entry&&) = default;

            inline Entry(procedure_t procedure, bool requires_id)
              : procedure(procedure),
                requires_id(requires_id)
            {}

            inline bool isValid() const {
              return (nullptr != procedure);
            }

            inline bool requiresID() const {
              return requiresID();
            }

            operator procedure_t() const {
              return procedure;
            }

            static inline Entry Request(procedure_t procedure) {
              return Entry{procedure, true};
            }

            static inline Entry Notification(procedure_t procedure) {
              return Entry{procedure, false};
            }

            procedure_t procedure;
            bool requires_id;
        };
        error::maybe_failed<TId, boost::json::object> getID(const boost::json::object& request) {
          TId id;

          if (!request.contains("id")) {
            return error(id, error::IdMissing());
          }

          const boost::json::value& id_value = request.at("id");
          typename util::FromJson<TId>::conversion_failure converted_id = id_conv(id_value);
          if ( !converted_id ) {
            return error(id, error::IdWrongType<TId>(converted_id.getFailed()));
          }

          return converted_id.getSuccess();
        }

        boost::json::value generateErrorIfRequired(error::maybe_failed<TId, boost::json::object> id, const error::ErrorCode& code) {
          if ( id ) {
            return error(id, code);
          }
          else {
            return boost::json::value();
          }
        }

        boost::json::value ensureJsonrpc(const boost::json::object& request, error::maybe_failed<TId, boost::json::object> id) {
          if ( !request.contains("jsonrpc") ) {
            return generateErrorIfRequired(id, error::JsonrpcMissing());
          }

          const boost::json::value& spec_value = request.at("jsonrpc");
          util::FromJson<std::string>::conversion_failure spec_succeeded = str_conv(spec_value);
          if ( !spec_succeeded ) {
            return generateErrorIfRequired(id, error::JsonrpcNotAString(spec_succeeded));
          }

          if ( "2.0" != static_cast<std::string>(spec_succeeded) ) {
            return generateErrorIfRequired(id, error::JsonrpcUnknownSpecification(spec_succeeded));
          }

          if ( !request.contains("params") ) {
            return generateErrorIfRequired(id, error::ParamsMissing());
          }

          if ( !request.at("params").is_object() ) {
            return generateErrorIfRequired(id, error::ParamsNotAnObject(util::GetJsonType(request.at("params"))));
          }

          // Return null on success
          return boost::json::value();
        }

        boost::json::value handleRequest(const boost::json::object& request, const TId&, procedure_t procedure) {
          return procedure(request);
        }

        boost::json::value handleNotification(const boost::json::object& notification, procedure_t procedure) {
          return procedure(notification);
        }

        util::FromJson<TId> id_conv;
        util::FromJson<std::string> str_conv;
        Error<TId> error;
        std::map<std::string, Entry> procedures;
        procedure_t fallback;
    };
  }
}
