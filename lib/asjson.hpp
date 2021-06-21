#pragma once

#include "util.hpp"

namespace ts7 {
  namespace jsonrpc {
    /**
     * @brief Convert to json
     *
     * Converts any type to a json.
     *
     * @note This version does not contain the static_assert. Check \ref FromJson, to see how this can be
     * achieved.
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
       * @param ref Const reference that shall be used.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      constexpr inline explicit AsJson(const T& ref)
        : ref(ref)
      {}

      /// Stored const reference
      const T& ref;

      /**
       * @brief Cast operator
       *
       * Casts the provided const reference to a json value.
       *
       * @attention This operator is not implemented and will cause a linker error. For any type that shall be
       * supported a corresponding template specialization needs to be created.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      operator boost::json::value() const;
    };

    template<>
    struct AsJson<int> {
        constexpr inline explicit AsJson(int n)
          : value(n)
        {}

        operator boost::json::value() const {
          return value;
        }

        int value;
    };
  }
}
