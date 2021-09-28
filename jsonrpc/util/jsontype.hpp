#pragma once

#include <cstdint>

#include <boost/json.hpp>

namespace ts7 {
  namespace jsonrpc {
    namespace util {
      /// JSON data types
      enum JsonType : std::int32_t {
        /// Null
        NONE,
        /// Bool value
        BOOL,
        /// Number value
        NUMBER,
        /// String value
        STRING,
        /// Object value
        OBJECT,
        /// Array value
        ARRAY,
      };

      /**
       * @brief Get JSON type
       *
       * @param value The value that shall be checked.
       *
       * @return Returns the current data type of the
       * provided boost::json::value.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline JsonType GetJsonType(const boost::json::value& value) {
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

      /**
       * @brief Get JSON type name
       *
       * @param t The JSON type.
       *
       * @return Returns the name of the provided JSON type.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline std::string GetJsonTypeName(JsonType t) {
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

      /**
       * @brief Get JSON type name
       *
       * @param value The value that shall be checked.
       *
       * @return Returns the JSON type name of the
       * provided boost::json::value.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline std::string GetJsonTypeName(boost::json::value& value) {
        return GetJsonTypeName(GetJsonType(value));
      }
    }
  }
}
