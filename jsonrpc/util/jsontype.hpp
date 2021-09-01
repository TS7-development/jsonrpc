#pragma once

#include <cstdint>

#include <boost/json.hpp>

namespace ts7 {
  namespace jsonrpc {
    namespace util {
      enum JsonType : std::int32_t {
        NONE,
        BOOL,
        NUMBER,
        STRING,
        OBJECT,
        ARRAY,
      };

      JsonType GetJsonType(const boost::json::value& value) {
        if (value.is_null())
          return JsonType::NONE;

        if (value.is_bool())
          return JsonType::BOOL;

        if (value.is_number())
          return JsonType::NUMBER;

        if (value.is_string())
          return JsonType::STRING;

        if (value.is_object())
          return JsonType::OBJECT;

        if (value.is_array())
          return JsonType::ARRAY;

        return JsonType::NONE;
      }

      std::string GetJsonTypeName(JsonType t) {
        switch(t) {
          case JsonType::NONE:
            return "null";
          case JsonType::BOOL:
            return "bool";
          case JsonType::NUMBER:
            return "number";
          case JsonType::STRING:
            return "string";
          case JsonType::OBJECT:
            return "object";
          case JsonType::ARRAY:
            return "array";
        }

        return "unknown";
      }

      std::string GetJsonTypeName(boost::json::value& value) {
        return GetJsonTypeName(GetJsonType(value));
      }
    }
  }
}
