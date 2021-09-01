#pragma once

#include <functional>

namespace ts7 {
  namespace jsonrpc {
    namespace error {
      template <typename TSuccess, typename TFailed>
      struct maybe_failed {
        using success_t = TSuccess;
        using failed_t = TFailed;
        using success_fn = std::function<void(const TSuccess&)>;
        using failed_fn = std::function<void(const TFailed&)>;

        constexpr inline maybe_failed(const TSuccess& success)
         : success(success),
           failed(TFailed()),
           succeeded(true)
        {}

        constexpr inline maybe_failed(const TFailed& failed)
         : success(TSuccess()),
           failed(failed),
           succeeded(false)
        {}

        constexpr inline maybe_failed(const maybe_failed&) = default;
        constexpr inline maybe_failed(maybe_failed&&) = default;

        constexpr inline operator bool() const {
          return succeeded;
        }

        constexpr inline operator const TSuccess&() const {
          return success;
        }

        constexpr inline operator const TFailed&() const {
          return failed;
        }

        inline void onSuccess(success_fn fn) const {
          if (succeeded) {
            fn(success);
          }
        }

        inline void onFailure(failed_fn fn) const {
          if (!succeeded) {
            fn(failed);
          }
        }

        inline void evaluate(success_fn s, failed_fn f) {
          if (succeeded) {
            s(success);
            return;
          }

          f(failed);
        }

        TSuccess success;
        TFailed failed;
        bool succeeded;
      };

      template <typename TSuccess>
      struct maybe_failed<TSuccess, void> {
        using success_t = TSuccess;
        using failed_t = void;
        using success_fn = std::function<void(const TSuccess&)>;
        using failed_fn = std::function<void()>;

        constexpr inline maybe_failed()
         : success(TSuccess()),
           succeeded(false)
        {}

        constexpr inline maybe_failed(const TSuccess& success)
         : success(success),
           succeeded(true)
        {}

        constexpr inline maybe_failed(const maybe_failed&) = default;
        constexpr inline maybe_failed(maybe_failed&&) = default;

        constexpr inline operator bool() const {
          return succeeded;
        }

        constexpr inline operator const TSuccess&() const {
          return success;
        }

        inline void onSuccess(success_fn fn) const {
          if (succeeded) {
            fn(success);
          }
        }

        inline void onFailure(failed_fn fn) const {
          if (!succeeded) {
            fn();
          }
        }

        inline void evaluate(success_fn s, failed_fn f) {
          if (succeeded) {
            s(success);
            return;
          }

          f();
        }

        TSuccess success;
        bool succeeded;
      };

      template <typename TFailed>
      struct maybe_failed<void, TFailed> {
        using success_t = void;
        using failed_t = TFailed;
        using success_fn = std::function<void()>;
        using failed_fn = std::function<void(const TFailed&)>;

        constexpr inline maybe_failed()
         : failed(TFailed()),
           succeeded(true)
        {}

        constexpr inline maybe_failed(const TFailed& failed)
         : failed(failed),
           succeeded(false)
        {}

        constexpr inline maybe_failed(const maybe_failed&) = default;
        constexpr inline maybe_failed(maybe_failed&&) = default;

        constexpr inline operator bool() const {
          return succeeded;
        }

        constexpr inline operator const TFailed&() const {
          return failed;
        }

        inline void onSuccess(success_fn fn) const {
          if (succeeded) {
            fn();
          }
        }

        inline void onFailure(failed_fn fn) const {
          if (!succeeded) {
            fn(failed);
          }
        }

        inline void evaluate(success_fn s, failed_fn f) {
          if (succeeded) {
            s();
            return;
          }

          f(failed);
        }

        TFailed failed;
        bool succeeded;
      };
    }
  }
}
