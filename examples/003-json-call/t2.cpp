#include "test.h"

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace json_call {
        /**
         * @brief Test version 2
         *
         * This test is an improved version of Test_v1. In this test we stick with the parameter amount fixed to
         * 2 and also still use fixed names for the parameters. But the return type as well as the two parameter
         * types can now be templated. Everything aside of it, is still working as in Test_v1.
         *
         * @tparam TReturn Return type of the provided function.
         * @tparam T1 Type of the first argument.
         * @tparam T2 Type of the second argument.
         *
         * @see Test_v1
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename TReturn, typename T1, typename T2>
        struct Test_v2 {
            /// Type definition of the callback
            using callback_t = std::function<TReturn(T1, T2)>;

            /**
             * @brief Type definition to remove the const modifier as well as the reference of a provided type.
             *
             * @tparam U The provided type.
             */
            template <typename U>
            using remove_cref = typename std::remove_const<typename std::remove_reference<U>::type>::type;

            /**
             * @brief constructor
             *
             * Explicit constructor that takes the callback, that shall be used.
             *
             * @param fn The provided callback.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            inline explicit Test_v2(callback_t fn)
              : fn(fn)
            {}

            /**
             * @brief Call operator
             *
             * Enables the call operation on instances of this class, which will convert the provided json object to
             * the corresponding call of the provided callback \p fn.
             *
             * @param o Arguments provided as json object.
             * @return Returns what ever the callback returns.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            TReturn operator()(const boost::json::object& o) {
              FromJson<remove_cref<T1>> v1;
              FromJson<remove_cref<T2>> v2;

              return fn(v1(o.at("value1")), v2(o.at("value2")));
            }

            /// Provided callback for usage with the json parameters
            callback_t fn;
        };

        /**
         * @brief example2
         *
         * This example uses Test_v2 with the same function signature as Test_v1, but this time provided as template
         * parameters. This test is still limited to 2 paramters and still uses fixed names for the paramter. The
         * provided callback is again testFunc as in example1.
         *
         * @param test
         *
         * @see example1
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        void example2(const boost::json::object& test) {
          Test_v2<void, const std::string&, int> t2(testFunc);
          t2(test);
        }
      }
    }
  }
}
