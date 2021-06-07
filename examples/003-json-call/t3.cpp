#include "test.h"

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace json_call {
        /**
         * @brief Test version 3
         *
         * This test is an improved version of Test_v2. This test is now enabling the instance owner to provide the
         * required parameter names. The paramater amount is still limitted to 2 parameters.
         *
         * @tparam TReturn The return type of the callback.
         * @tparam T1 First paramater type.
         * @tparam T2 Second parameter type.
         *
         * @see Test_v2, Test_v1
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename TReturn, typename T1, typename T2>
        struct Test_v3 {
            /// Callback type
            using callback_t = std::function<TReturn(T1, T2)>;

            /// Const reference remove
            template <typename U>
            using remove_cref = typename std::remove_const<typename std::remove_reference<U>::type>::type;

            /**
             * @brief Paramater
             *
             * Class that stores the name and type of a paramter for later usage.
             *
             * @tparam U The parameter type.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            template <typename U>
            struct Parameter {
                /**
                 * @brief constructor
                 *
                 * Explicit constructor that just stores the parameter name as copy.
                 *
                 * @note A copy should be fine here, since we expect quite short parameter names (max. 100 characters).
                 *
                 * @param name The parameter name that shall be stored.
                 *
                 * @since 1.0
                 *
                 * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
                 */
                constexpr inline explicit Parameter(const std::string& name)
                  : name(name)
                {}

                /**
                 * @brief Call operator
                 *
                 * Enables the call operation on instances of this class. Therefore it takes a json object is using
                 * the povided name to retrieve the corresponding value.
                 *
                 * @attention In this test there is no checking, if the parameter exists or if the type matches. This
                 * decision was made to keep the test for now as simple as possible.
                 *
                 * @param o The provided json object that contains the parameter.
                 *
                 * @return Returns the retrieved value.
                 *
                 * @since 1.0
                 *
                 * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
                 */
                remove_cref<U> operator()(const boost::json::object& o) {
                  FromJson<remove_cref<U>> v;
                  return v(o.at(name));
                }

                /// The parameter name
                std::string name;
            };

            /**
             * @brief constructor
             *
             * Constructor that is storing the provided callback and creating the two parameters by it's provided
             * names.
             *
             * @param fn The provided callback.
             * @param p1 Name of the first parameter.
             * @param p2 Name of the second parameter.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            inline Test_v3(callback_t fn, const std::string& p1, const std::string& p2)
              : fn(fn),
                p1(p1),
                p2(p2)
            {}

            /**
             * @brief Call operator
             *
             * Enables the call operation for instances of this class. This takes a json object that contains all required
             * parameter.
             *
             * @param o Json object that is providing the parameter.
             *
             * @return Returns whatever the call back returns.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            TReturn operator()(const boost::json::object& o) {
              remove_cref<T1> t1 = p1(o);
              remove_cref<T2> t2 = p2(o);
              return fn(t1, t2);
            }

            callback_t fn;
            Parameter<T1> p1;
            Parameter<T2> p2;
        };

        /**
         * @brief example3
         *
         * This example uses the improved json callback Test_v3. This new version is now capable of getting the
         * parameter names configured. To keep things as simple as possible the parameter amount is still fixed
         * to 2. In this example there is also now type checking of even existance checking for the required
         * parameter.
         *
         * This example is still using the same function signature and parameter names as \ref example1 and \ref example2. Also
         * the \ref testFunc callback is still used.
         *
         * @param test The json object that contains the required parameter.
         *
         * @see example1, example2, testFunc
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        void example3(const boost::json::object& test) {
          Test_v3<void, const std::string&, int> t3(testFunc, "value1", "value2");
          t3(test);
        }
      }
    }
  }
}
