#pragma once

#include "asjson.hpp"
#include "util.hpp"

namespace ts7 {
  namespace jsonrpc {
    class Notification {
      public:
        Notification(const std::string& method)
          : method(method)
        {}

        std::string getMethod() const {
          return method;
        }

        boost::json::value& operator[](std::string_view key) {
          return params.at(key.data());
        }

        const boost::json::value& operator[](std::string_view key) const {
          return params.at(key.data());
        }

        operator boost::json::object() const {
          boost::json::object n;
          n["jsonrpc"] = "2.0";
          n["method"] = method;
          n["params"] = params;

          return n;
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
        static Notification Create(const std::string& method, TArgs... args) {
          Notification req(method);
          req.addArgument(args...);
          return req;
        }

      protected:
        std::string method;
        boost::json::object params;
    };

    template <typename TId>
    static inline std::ostream& operator<<(std::ostream& os, const Notification& n) {
      return os << static_cast<boost::json::object>(n);
    }
  }
}
