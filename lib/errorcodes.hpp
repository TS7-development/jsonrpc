#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#include <boost/format.hpp>

namespace ts7 {
  namespace jsonrpc {
    enum class ErrorCodes : std::int32_t {
      JSONRPC_MISSING = 1,
      JSONRPC_NOT_A_STRING,
      JSONRPC_UNKNOWN_SPECIFICATION,
      METHOD_MISSING,
      METHOD_NOT_A_STRING,
      ID_MISSING,
      PARAMS_MISSING,
      PARAMS_NOT_AN_OBJECT,
      PARAMETER_MISSING,
      PARAMETER_WRONG_TYPE
    };

     [[maybe_unused]] static constexpr inline const char* GetErrorDescription(ErrorCodes errorCode) {
      switch(errorCode) {
        case ErrorCodes::JSONRPC_MISSING:
          return "Missing field \"jsonrpc\"";
        case ErrorCodes::JSONRPC_NOT_A_STRING:
          return "Field \"jsonrpc\" is not a string";
        case ErrorCodes::JSONRPC_UNKNOWN_SPECIFICATION:
          return "Unknown JSON-RPC specification \"%s\", exptected \"2.0\"";
        case ErrorCodes::METHOD_MISSING:
          return "Missing field \"method\"";
        case ErrorCodes::METHOD_NOT_A_STRING:
          return "Field \"method\" is not a string";
        case ErrorCodes::ID_MISSING:
          return "Missing field \"id\"";
        case ErrorCodes::PARAMS_MISSING:
          return "Missing field \"params\"";
        case ErrorCodes::PARAMS_NOT_AN_OBJECT:
          return "Field \"params\" is not an object";
        case ErrorCodes::PARAMETER_MISSING:
          return "Missing parameter \"%s\"";
        case ErrorCodes::PARAMETER_WRONG_TYPE:
          return "Parameter paramater \"%s\" has type %s, but %s is required";
      }

      return "Unknown error";
    }


    template <typename ...TArgs>
    static inline std::string GetFormattedErrorDescription(ErrorCodes errorCode, TArgs... args) {
      char buf[256] = {0};
      const char* fmt = GetErrorDescription(errorCode);
      std::sprintf(buf, fmt, args...);
      return buf;
    }
  }
}
