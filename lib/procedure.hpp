#pragma once

#include "asjson.hpp"
#include "callfromjson.hpp"
#include "error.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId, typename TOnRequest>
    class Procedure
    {
      public:
        Procedure(const TOnRequest& onRequestHandler)
          : onRequestHandler(onRequestHandler)
        {}

        boost::json::object onRequest(const boost::json::object& req) {
          if (!req.contains("jsonrpc")) {
            return Error<TId>(ErrorCodes::JSONRPC_MISSING);
          }

          if (!req.at("jsonrpc").is_string()) {
            return Error<TId>(ErrorCodes::JSONRPC_NOT_A_STRING);
          }

          if (req.at("jsonrpc").as_string() != "2.0") {
            return Error<TId>(ErrorCodes::JSONRPC_UNKNOWN_SPECIFICATION, req.at("jsonrpc").as_string());
          }

          if (!req.contains("id")) {
            return Error<TId>(ErrorCodes::ID_MISSING);
          }

          if (!req.contains("method")) {
            return Error<TId>(ErrorCodes::METHOD_MISSING);
          }

          if (!req.at("method").is_string()) {
            return Error<TId>(ErrorCodes::METHOD_NOT_A_STRING);
          }

          if (!req.contains("params")) {
            return Error<TId>(req.at("method").as_string().data(), FromJson<TId>(req.at("id")), ErrorCodes::PARAMS_MISSING);
          }

          if (!req.at("params").is_object()) {
            return Error<TId>(req.at("method").as_string().data(), FromJson<TId>(req.at("id")), ErrorCodes::PARAMS_NOT_AN_OBJECT);
          }

          boost::json::object params = req.at("params").as_object();
          typename TOnRequest::return_t x = onRequestHandler(params);

          boost::json::object response;
          response["jsonrpc"] = "2.0";
          response["id"] = req.at("id");
          response["method"] = req.at("method");
          response["result"] = AsJson<typename TOnRequest::return_t>(x);

          return response;
        }

      protected:
        TOnRequest onRequestHandler;
    };
  }
}
