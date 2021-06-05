#include "test.h"

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace any_collection {
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
