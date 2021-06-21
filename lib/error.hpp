#pragma once

#include <string>

#include "asjson.hpp"
#include "errorcodes.hpp"
#include "request.hpp"
#include "util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId>
    class Error {
      public:
        Error(const std::string& method, const TId& id, std::int32_t code, std::string_view message)
          : method(method),
            id(id)
        {
          boost::json::object errorObject;
          errorObject["code"] = code;
          errorObject["message"] = message.data();
          error = errorObject;
        }

        template <typename TArg>
        Error(const std::string& method, const TId& id, const TArg& arg)
          : method(method),
            id(id),
            error(AsJson<TArg>(arg))
        {}

        template <typename ...TArgs>
        Error(const std::string& method, const TId& id, ErrorCodes errorCode, TArgs... args)
          : Error(method, id, static_cast<std::int32_t>(errorCode), GetFormattedErrorDescription(errorCode, args...))
        {}

        Error(const Request<TId>& req, std::int32_t code, std::string_view message)
          : Error(req.getMethod(), req.getID(), code, message)
        {}

        template <typename TArg>
        Error(const Request<TId>& req, const TArg& arg)
          : Error(req.getMethod(), req.getID(), arg)
        {}

        template <typename ...TArgs>
        Error(const Request<TId>& req, ErrorCodes errorCode, TArgs... args)
          : Error(req.getMethod(), req.getID(), static_cast<std::int32_t>(errorCode), GetFormattedErrorDescription(errorCode, args...))
        {}

        template <typename ...TArgs>
        Error(ErrorCodes errorCode, TArgs... args)
          : Error("invalidMethod", TId(), static_cast<std::int32_t>(errorCode), GetFormattedErrorDescription(errorCode, args...))
        {}

        std::string getMethod() const {
          return method;
        }

        const TId& getID() const {
          return id;
        }

        boost::json::value& operator[](std::string_view key) {
          if (!error.is_object()) {
            error = boost::json::object();
          }

          return error.as_object().at(key.data());
        }

        const boost::json::value& operator[](std::string_view key) const {
          return error.as_object().at(key.data());
        }

        operator boost::json::object() const {
          boost::json::object o;
          o["jsonrpc"] = "2.0";
          o["method"] = method;
          o["id"] = AsJson<TId>(id);
          o["error"] = error;

          return o;
        }

      protected:
        std::string method;
        TId id;
        boost::json::value error;
    };
  }
}
