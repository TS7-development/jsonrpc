#pragma once

#include <string>
#include <experimental/source_location>

#include "always_false.hpp"
#include "jsontype.hpp"

namespace ts7 {
  namespace jsonrpc {
    namespace util {
      /**
       * @brief Convert to json
       *
       * Converts any type to a json.
       *
       * @tparam T The type that shall be converted to a json value.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename T>
      struct AsJson {
        /**
         * @brief constructor
         *
         * Creates an instance that stores the const reference to the provided variable.
         *
         * @attention Using the templated version instead of a fully specialized version will cause a static_assert
         * fail in the constructor. It is mandatory to specialize this template for new types, that shall be supported.
         *
         * @param ref Const reference that shall be used.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline explicit AsJson(const T&) {
          static_assert(always_false<T>, "Type not supported");
        }
      };

      /**
       * @brief Convert int to json
       *
       * Converts an integer to a json value.
       *
       * @note This is the template specialization of \ref AsJson.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template<>
      struct AsJson<std::int32_t> {
          static constexpr const JsonType type = JsonType::NUMBER;

          static inline constexpr bool IsType(JsonType t) {
            return type == t;
          }

          /**
           * @brief constructor
           *
           * Stores the provided integer value for later conversion.
           *
           * @param n The integer value that shall be converted.
           */
          constexpr inline explicit AsJson(std::int32_t n)
            : value(n)
          {}

          /**
           * @brief Json value cast
           *
           * Casts the instance to a json value. In this case to a number with the !value stored in \ref value.
           */
          operator boost::json::value() const {
            return value;
          }

          /// THe stored value
          std::int32_t value;
      };

      /**
       * @brief Convert int to json
       *
       * Converts an integer to a json value.
       *
       * @note This is the template specialization of \ref AsJson.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template<>
      struct AsJson<std::uint32_t> {
          static constexpr const JsonType type = JsonType::NUMBER;

          static inline constexpr bool IsType(JsonType t) {
            return type == t;
          }

          /**
           * @brief constructor
           *
           * Stores the provided integer value for later conversion.
           *
           * @param n The integer value that shall be converted.
           */
          constexpr inline explicit AsJson(std::uint32_t n)
            : value(n)
          {}

          /**
           * @brief Json value cast
           *
           * Casts the instance to a json value. In this case to a number with the !value stored in \ref value.
           */
          operator boost::json::value() const {
            return value;
          }

          /// THe stored value
          std::uint32_t value;
      };

      /**
       * @brief Convert int to json
       *
       * Converts an integer to a json value.
       *
       * @note This is the template specialization of \ref AsJson.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template<>
      struct AsJson<std::string> {
          static constexpr const JsonType type = JsonType::STRING;

          static inline constexpr bool IsType(JsonType t) {
            return type == t;
          }

          /**
           * @brief constructor
           *
           * Stores the provided integer value for later conversion.
           *
           * @param n The integer value that shall be converted.
           */
          inline explicit AsJson(const std::string& value)
            : value(value)
          {}

          inline explicit AsJson(std::string&& value)
            : value(std::move(value))
          {}

          /**
           * @brief Json value cast
           *
           * Casts the instance to a json value. In this case to a number with the !value stored in \ref value.
           */
          operator boost::json::value() const {
            return value.c_str();
          }

          /// THe stored value
          std::string value;
      };

      template<>
      struct AsJson<std::experimental::source_location> {
        static constexpr const JsonType type = JsonType::OBJECT;

        static inline constexpr bool IsType(JsonType t) {
          return type == t;
        }

        /**
         * @brief constructor
         *
         * Stores the provided integer value for later conversion.
         *
         * @param n The integer value that shall be converted.
         */
        inline explicit AsJson(const std::experimental::source_location& value)
          : value(value)
        {}

        /**
         * @brief Json value cast
         *
         * Casts the instance to a json value. In this case to a number with the !value stored in \ref value.
         */
        operator boost::json::value() const {
          boost::json::object o;
          o["file"] = value.file_name();
          o["function"] = value.function_name();
          o["line"] = value.line();
          o["column"] = value.column();

          return o;
        }

        /// THe stored value
        std::experimental::source_location value;
      };
    }
  }
}
