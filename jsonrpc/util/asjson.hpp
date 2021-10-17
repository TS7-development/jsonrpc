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
       * @brief Convert bool to json
       *
       * Converts a boolean to a json value.
       *
       * @note This is the template specialization of \ref AsJson.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template<>
      struct AsJson<bool> {
          static constexpr const JsonType type = JsonType::BOOL;

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
          constexpr inline explicit AsJson(bool n)
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
          bool value;
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
      struct AsJson<std::int8_t> {
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
          constexpr inline explicit AsJson(std::int8_t n)
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
          std::int8_t value;
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
      struct AsJson<std::int16_t> {
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
          constexpr inline explicit AsJson(std::int16_t n)
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
          std::int16_t value;
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
      struct AsJson<std::int64_t> {
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
          constexpr inline explicit AsJson(std::int64_t n)
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
          std::int64_t value;
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
      struct AsJson<std::uint8_t> {
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
          constexpr inline explicit AsJson(std::uint8_t n)
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
          std::uint8_t value;
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
      struct AsJson<std::uint16_t> {
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
          constexpr inline explicit AsJson(std::uint16_t n)
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
          std::uint16_t value;
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
      struct AsJson<std::uint64_t> {
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
          constexpr inline explicit AsJson(std::uint64_t n)
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
          std::uint64_t value;
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
      struct AsJson<float> {
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
          constexpr inline explicit AsJson(float n)
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
          float value;
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
      struct AsJson<double> {
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
          constexpr inline explicit AsJson(double n)
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
          double value;
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
      template<typename T>
      struct AsJson<std::vector<T>> {
          static constexpr const JsonType type = JsonType::ARRAY;

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
          constexpr inline explicit AsJson(const std::vector<T>& n)
            : value(n)
          {}

          /**
           * @brief Json value cast
           *
           * Casts the instance to a json value. In this case to a number with the !value stored in \ref value.
           */
          operator boost::json::value() const {
            boost::json::array a;

            for (const T& t : value) {
              a.push_back(AsJson<T>(t));
            }

            return a;
          }

          /// THe stored value
          std::vector<T> value;
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
      struct AsJson<boost::json::object> {
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
          inline explicit AsJson(const boost::json::object& n)
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
          boost::json::object value;
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
      struct AsJson<boost::json::array> {
          static constexpr const JsonType type = JsonType::ARRAY;

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
          inline explicit AsJson(const boost::json::array& n)
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
          boost::json::array value;
      };
    }
  }
}
