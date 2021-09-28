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
         * @brief Success value
         *
         * @return Returns the success instance.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline const TSuccess& getSuccess() const {
          return success;
        }

        /**
         * @brief Failure value
         *
         * @return Returns the failure instance.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline const TFailed& getFailed() const {
          return failed;
        }

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
        /// True, if succeeded. Otherwise false.´
        bool succeeded;
      };

      /**
       * @brief Maybe failed
       *
       * Represents a return type, that can fail or succeed. If succeeded the
       * original return value of the function is accessable. There is no
       * return value stored in case of failure.
       *
       * @tparam TSuccess Data type, if succeeded.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename TSuccess>
      struct maybe_failed<TSuccess, void> {
        /// Success data type
        using success_t = TSuccess;
        /// Failed data type
        using failed_t = void;
        /// Success callback
        using success_fn = std::function<void(const TSuccess&)>;
        /// Failed callback
        using failed_fn = std::function<void()>;

        /**
         * @brief constructor
         *
         * Creates an return type, that can fail or succeed. In this case the
         * return type is treated as failure.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline maybe_failed()
         : success(TSuccess()),
           succeeded(false)
        {}

        /**
         * @brief constructor
         *
         * Create a return type for the success case.
         *
         * @param success Success instance, that shall be copied and stored.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline maybe_failed(const TSuccess& success)
         : success(success),
           succeeded(true)
        {}

        /// copy constructor
        constexpr inline maybe_failed(const maybe_failed&) = default;

        /// move constructor
        constexpr inline maybe_failed(maybe_failed&&) = default;

        constexpr inline const TSuccess& getSuccess() const {
          return success;
        }

        /**
         * @brief Success cast operator
         *
         * @return Returns true, if succeeded. Otherwise it returns false.
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
         * @note This should be only used, in case of success.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline operator const TSuccess&() const {
          return success;
        }

        /**
         * @brief On success
         *
         * Executes the provided callback, if succeeded.
         *
         * @param fn Success callback.
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
         * Executes the provided callback, if failed.
         *
         * @param fn Failure callback.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        inline void onFailure(failed_fn fn) const {
          if (!succeeded) {
            fn();
          }
        }

        /**
         * @brief evaluate
         *
         * Evaluates the state of the instance and executes either the
         * success or failure callback.
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

          f();
        }

        /// Success instance
        TSuccess success;
        /// True if succeeded. Otherwise false.
        bool succeeded;
      };

      /**
       * @brief Maybe failed
       *
       * Represents a return type that can have maybe failed. This template
       * specialization only supports failure information, but no success
       * information.
       *
       * @tparam TFailed Data type of the failure information.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename TFailed>
      struct maybe_failed<void, TFailed> {
        /// Success type (void)
        using success_t = void;
        /// Failure data type
        using failed_t = TFailed;
        /// Success callback
        using success_fn = std::function<void()>;
        /// Failure callback
        using failed_fn = std::function<void(const TFailed&)>;

        /**
         * @brief constructor
         *
         * Creates a success.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline maybe_failed()
         : failed(TFailed()),
           succeeded(true)
        {}

          /**
         * @brief constructor
         *
         * Creates a failure and stores the corresponding failure information.
         *
         * @param failed Instance of the failed information to copy.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline maybe_failed(const TFailed& failed)
         : failed(failed),
           succeeded(false)
        {}

        /// copy constructor
        constexpr inline maybe_failed(const maybe_failed&) = default;
        /// move constructor
        constexpr inline maybe_failed(maybe_failed&&) = default;

        constexpr inline const TFailed& getFailed() const {
          return failed;
        }

        /**
         * @brief Success cast operator
         *
         * @return Returns true, if succeeded. Otherwise it returns false.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline operator bool() const {
          return succeeded;
        }

        /**
         * @brief Failure cast operator
         *
         * @return Returns the stored failure instance.
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
         * Executes the success callback, if succeeded.
         *
         * @param fn Success callback.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        inline void onSuccess(success_fn fn) const {
          if (succeeded) {
            fn();
          }
        }

        /**
         * @brief On failure
         *
         * Executes the failure callback, if failed.
         *
         * @param fn Failure callback.
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
         * Evaluates the result and executes the success callback, if
         * succeeded. Otherwise it executes the failure callback.
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
            s();
            return;
          }

          f(failed);
        }

        /// Failure information
        TFailed failed;

        /// Success state, which is true on success. Otherwise it is false.
        bool succeeded;
      };

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
      template <typename TFailed>
      struct maybe_failed<bool, TFailed> {
        /// Data type, if the function succeeded
        using success_t = bool;
        /// Data type, if the function failed
        using failed_t = TFailed;
        /// Callback function, if succeeded
        using success_fn = std::function<void(const bool&)>;
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
        constexpr inline maybe_failed(const bool& success)
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
         : success(bool()),
           failed(failed),
           succeeded(false)
        {}

        /// copy constructor
        constexpr inline maybe_failed(const maybe_failed&) = default;

        /// move constructor
        constexpr inline maybe_failed(maybe_failed&&) = default;

        /**
         * @brief Success value
         *
         * @return Returns the success instance.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline const bool& getSuccess() const {
          return success;
        }

        /**
         * @brief Failure value
         *
         * @return Returns the failure instance.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline const TFailed& getFailed() const {
          return failed;
        }

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
        bool success;
        /// Failure instance
        TFailed failed;
        /// True, if succeeded. Otherwise false.´
        bool succeeded;
      };

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
      template <typename TSuccess>
      struct maybe_failed<TSuccess, bool> {
        /// Data type, if the function succeeded
        using success_t = TSuccess;
        /// Data type, if the function failed
        using failed_t = bool;
        /// Callback function, if succeeded
        using success_fn = std::function<void(const TSuccess&)>;
        /// Callback function, if failed
        using failed_fn = std::function<void(const bool&)>;

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
           failed(bool()),
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
        constexpr inline maybe_failed(const bool& failed)
         : success(TSuccess()),
           failed(failed),
           succeeded(false)
        {}

        /// copy constructor
        constexpr inline maybe_failed(const maybe_failed&) = default;

        /// move constructor
        constexpr inline maybe_failed(maybe_failed&&) = default;

        /**
         * @brief Success value
         *
         * @return Returns the success instance.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline const TSuccess& getSuccess() const {
          return success;
        }

        /**
         * @brief Failure value
         *
         * @return Returns the failure instance.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline const bool& getFailed() const {
          return failed;
        }

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
        bool failed;
        /// True, if succeeded. Otherwise false.´
        bool succeeded;
      };
    }
  }
}
