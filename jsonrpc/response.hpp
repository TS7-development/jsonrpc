#pragma once

#include <string>

#include "request.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId, typename TResult>
    class Response {
      public:
        Response() = default;

#ifndef TS7_JSONRPC_SUPPORT_RESPONSE_METHOD_NAME
        boost::json::object operator()(const TId& id, const TResult& result) const {
          boost::json::object o;
          o["jsonrpc"] = "2.0";
          //o["method"] = method;
          o["id"] = util::AsJson<TId>(id);
          o["result"] = util::AsJson<TResult>(result);

          return o;
        }
#else
        boost::json::object operator()(const TId& id, const std::string& method, const TResult& result) const {
          boost::json::object o;
          o["jsonrpc"] = "2.0";
          o["method"] = method;
          o["id"] = util::AsJson<TId>(id);
          o["result"] = util::AsJson<TResult>(result);

          return o;
        }
#endif
    };
  }
}
