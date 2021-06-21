#pragma once

#include <string>

#include "asjson.hpp"
#include "request.hpp"
#include "util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId>
    class Response {
      public:
        Response(const std::string& method, const TId& id)
          : method(method),
            id(id),
            result(boost::json::object())
        {}

        template <typename TResult>
        Response(const std::string& method, const TId& id, const TResult& res)
          : method(method),
            id(id),
            result(AsJson<TResult>(res))
        {}

        Response(const Request<TId>& req)
          : Response(req.getMethod(), req.getID())
        {}

        template <typename TResult>
        Response(const Request<TId>& req, const TResult& res)
          : Response(req.getMethod(), req.getID(), res)
        {}

        const std::string& getMethod() const {
          return method;
        }

        const TId& getID() const {
          return id;
        }

        operator boost::json::object() const {
          boost::json::object o;
          o["jsonrpc"] = "2.0";
          o["method"] = method;
          o["id"] = AsJson<TId>(id);
          o["result"] = result;

          return o;
        }

      protected:
        std::string method;
        TId id;
        boost::json::value result;
    };
  }
}
