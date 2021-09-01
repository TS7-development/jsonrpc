#pragma once

#include <functional>

namespace ts7 {
  namespace jsonrpc {
    namespace error {
      /**
       * @brief Maybe failed
       *
       * Class that represents a return type that maybe has failed. This
       * stores the success value, if the function succeeded. Otherwise
       * it stores the failed return type, if the function has failed.
       *
       * @note TSuccess and TFailed need to support the default constructor.
       *
       * @tparam TSuccess The return type, if the function succeeded.
       * @tparam TFailed The return type, if the function failed.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename TSuccess, typename TFailed>
      struct maybe_failed {
        /// Data type, if the function succeeded
        using success_t = TSuccess;
        /// Data type, if the function failed
        using failed_t = TFailed;
        /// Callback function, if succeeded
        using success_fn = std::function<void(const TSuccess&)>;
        /// Callback function, if failed
        using failed_fn = std::function<void(const TFailed&)>;

        /**
         * @brief constructor
         *
         * Creates the maybe_failed object for a successful case and
         * stores a copy of the success object inside.
         *
         * @param success Success instance that shall be copied.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline maybe_failed(const TSuccess& success)
         : success(success),
           failed(TFailed()),
           succeeded(true)
        {}

        /**
         * @brief constructor
         *
         * Creates the maybe_failed object for a failed case and stores a
         * copy of the failure object inside.
         *
         * @param failed Failure instance that shall be copied.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline maybe_failed(const TFailed& failed)
         : success(TSuccess()),
           failed(failed),
           succeeded(false)
        {}

        /// copy constructor
        constexpr inline maybe_failed(const maybe_failed&) = default;

        /// move constructor
        constexpr inline maybe_failed(maybe_failed&&) = default;

        /**
         * @brief operator bool
         *
         * @return Returns true, if a success instance is stored within.
         * Otherwise it returns false.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline operator bool() const {
          return succeeded;
        }

        /**
         * @brief Success cast operator
         *
         * @return Returns the success instance.
         *
         * @note This should only be used, if the the maybe_failed object was
         * created as success.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline operator const TSuccess&() const {
          return success;
        }

        /**
         * @brief Failure cast operator
         *
         * @return Returns the failure instance.
         *
         * @note This should only be used, if the maybe_failed object was
         * created as failure.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline operator const TFailed&() const {
          return failed;
        }

        /**
         * @brief On success
         *
         * Provide a callback, if the maybe_failed object was created as
         * success.
         *
         * @param fn Callback, that shall be executed, if success.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        inline void onSuccess(success_fn fn) const {
          if (succeeded) {
            fn(success);
          }
        }

        /**
         * @brief On failure
         *
         * Provide a callback, if the maybe_failed object was created as
         * failure.
         *
         * @param fn Callback, that shall be executed, if failure.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        inline void onFailure(failed_fn fn) const {
          if (!succeeded) {
            fn(failed);
          }
        }

        /**
         * @brief evaluate
         *
         * Executes the success callback \p s, on success. Otherwise the
         * failure callback \p f will be called.
         *
         * @param s Success callback.
         * @param f Failure callback.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        inline void evaluate(success_fn s, failed_fn f) {
          if (succeeded) {
            s(success);
            return;
          }

          f(failed);
        }

        /// Success instance
        TSuccess success;
        /// Failure instance
        TFailed failed;
        /// True, if succeeded. Otherwise false.
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
