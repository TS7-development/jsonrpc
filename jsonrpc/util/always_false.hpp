#pragma once

namespace ts7 {
  namespace jsonrpc {
    namespace util {
      /**
       * @brief Always false
       *
       * This bool is a evaluated at compile time and will be always false no matter which and how many types are
       * provided to the variadic template. This is required for static_assert. If you just pass false to
       * it, the compiler will optimize it and directly fail, because it will be false no matter which type is
       * provided. This bool on the other hand requires the types to be evaluated to it can't be directly
       * optimized away and will only lead to the static_assert fail, if a certain type is used.
       *
       * This approach is decribed in:
       * https://artificial-mind.net/blog/2020/10/03/always-false
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template< typename ... T>
      constexpr bool always_false = false;
    }
  }
}
