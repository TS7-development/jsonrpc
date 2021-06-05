#include "test.h"

#include <boost/any.hpp>

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace any_collection {
        void any() {
          std::cout <<"Any print" << std::endl;
          boost::any a = A();
          boost::any_cast<A>(a).print();

          boost::any ci = C<int>();
          boost::any_cast<C<int>>(ci).print();

          std::cout << std::endl;
        }
      }
    }
  }
}
