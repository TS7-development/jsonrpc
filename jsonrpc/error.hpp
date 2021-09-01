#pragma once

#include <string>

#include "request.hpp"
#include "error/error.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId>
    class Error {
      public:        
        Error() = default;

#ifndef TS7_JSONRPC_SUPPORT_RESPONSE_METHOD_NAME
        boost::json::object operator()(const TId& id, const error::ErrorCode& code) const {
          boost::json::object o;
          o["jsonrpc"] = "2.0";
          o["id"] = util::AsJson<TId>(id);
          o["error"] = static_cast<boost::json::object>(code);

          return o;
        }
#else
        boost::json::object operator()(const TId& id, const std::string& method, const error::ErrorCode& code) const {
          boost::json::object o;
          o["jsonrpc"] = "2.0";
          o["method"] = method;
          o["id"] = util::AsJson<TId>(id);
          o["error"] = static_cast<boost::json::object>(code);

          return o;
        }
#endif

      protected:
    };
  }
}
