#include "variadic-members.h"

#include <iostream>
#include <tuple>
#include <string>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace variadic_member {
      /**
       * @brief Dummy structure
       *
       * Dummy structure to immitate the parameter structure of request handler.
       *
       * @tparam T Data type of the parameter
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename T>
      struct Dummy {
          /// constructor
          Dummy(const T& t)
            : t(t)
          {}

          /// Value
          T t;
      };

      /**
       * @brief Test
       *
       * Structure that stores any amount of variadic template parameters in a tuple
       * and is capable of dumping them all.
       *
       * @tparam TArgs List of parameter.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename... TArgs>
      struct Test {
          /**
         * @brief constructor
         *
         * Creates the test structure and stores all values of the variadic parameter.
         *
         * @param args List of values.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        Test(const std::tuple<Dummy<TArgs>...>& args)
          : parameter(args)
        {}

        /// Tuple of values
        std::tuple<Dummy<TArgs>...> parameter;

        /**
         * @brief apply
         *
         * Dumps all stored values of the tuple.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        void apply() {
          int n = 0;

          std::apply([&n](auto const&... r) {
            ((std::cout << "Parameter #" << (++n) << ": " << r.t << std::endl), ...);
          }, parameter);
        }
      };

      /**
       * @brief dump
       *
       * Demonstrates the usage of the Dummy and Test structures to dump any amount of variables at once.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      void dump() {
        auto tup = std::make_tuple<Dummy<int>, Dummy<int>>(
          Dummy<int>(3),
          Dummy<int>(7)
        );

        Test<int, int> t(tup);

        t.apply();
      }
    }
  }
}
