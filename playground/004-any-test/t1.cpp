#include "test.h"

namespace ts7 {
  namespace jsonrpc {
    namespace playground {
      namespace any_collection {
        /**
         * @brief simple
         *
         * Just testing, if all structs and type demangling is working as expected. It just prints every type at
         * least once and for C it is tested with multiple template types.
         *
         * @note This example does not care about storing different types, it just meant to ensure that everything
         * is doing what it shall do.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        void simple() {
          std::cout << "Simple print" << std::endl;
          print<A>();
          print<B>();
          print<C<int>>();
          print<C<double*>>();
          print<C<A>>();
          std::cout << std::endl;
        }
      }
    }
  }
}
