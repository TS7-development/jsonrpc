#pragma once

#include "asjson.hpp"
#include "util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId>
    class Request {
      public:
        Request(const std::string& method, const TId& id)
          : method(method),
            id(id)
        {}

        std::string getMethod() const {
          return method;
        }

        const TId& getID() const {
          return id;
        }

        boost::json::value& operator[](std::string_view key) {
          return params.at(key.data());
        }

        const boost::json::value& operator[](std::string_view key) const {
          return params.at(key.data());
        }

        operator boost::json::object() const {
          boost::json::object req;
          req["jsonrpc"] = "2.0";
          req["id"] = AsJson<TId>(id);
          req["method"] = method;
          req["params"] = params;

          return req;
        }

        template <typename TName, typename TValue>
        void addArgument(const TName& name, const TValue& value) {
          params[name] = AsJson<TValue>(value);
        }

        template <typename TName, typename TValue, typename ...TArgs>
        void addArgument(const TName& name, const TValue& value, TArgs... args) {
          params[name] = AsJson<TValue>(value);
          addArgument(args...);
        }

        template <typename... TArgs>
        static Request Create(const std::string& method, const TId& id, TArgs... args) {
          Request req(method, id);
          req.addArgument(args...);
          return req;
        }

      protected:
        std::string method;
        TId id;
        boost::json::object params;
    };

    template <typename TId>
    static inline std::ostream& operator<<(std::ostream& os, const Request<TId>& req) {
      return os << static_cast<boost::json::object>(req);
    }
  }
}
