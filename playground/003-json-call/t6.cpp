#include "test.h"
#include "t5.h"

#include <iostream>

// #define T6_CREATE_COMPILER_ERROR

namespace ts7 {
  namespace jsonrpc {
    namespace playground {
      namespace json_call {
        /**
         * @brief Test version 6
         *
         * This type definition copies exactly the behaviour of \ref Test_v4.
         *
         * @note This test is only meant to demonstrate the compiler error message for an unsupported type of
         * FromJson. To see the force the compiler error, it is required to define T6_CREATE_COMPILER_ERROR.
         *
         * @tparam TReturn Return type of the json callback.
         * @tparam T1 Type of the first parameter.
         * @tparam T2 Type of the second parameter.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename TReturn, typename T1, typename T2>
        using Test_v6 = Test_v5<TReturn, T1, T2>;

        // Dummy struct that is not convertible (at the moment)
        struct Dummy {
            int x;
            int y;
            int z;
        };

        /**
         * @brief example6
         *
         * This examples demonstrates what happens, when we try to use an unsupported typ (\ref Dummy). Aside of it
         * the example just dumps "Dummy called", when the callback could be executed successful.
         *
         * @param test3 Provided json object with the parameter.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger
         */
        void example6([[maybe_unused]] const boost::json::object& test3) {
          using Tv6 = Test_v4<void, const Dummy&, const Dummy&>;

          Tv6 t6(
            [](const Dummy&, const Dummy&) {
              std::cout << "Dummy called" << std::endl;
            },
            "a",
            "b"
          );

        #ifdef T6_CREATE_COMPILER_ERROR
          t6(test3);
        #endif
        }
      }
    }
  }
}

