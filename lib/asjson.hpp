#pragma once

#include "util.hpp"

namespace ts7 {
  namespace jsonrpc {
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
    struct AsJson<int> {
        /**
         * @brief constructor
         *
         * Stores the provided integer value for later conversion.
         *
         * @param n The integer value that shall be converted.
         */
        constexpr inline explicit AsJson(int n)
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
        int value;
    };
  }
}
