#include "test.h"

#include <boost/any.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace any_collection {
      /**
       * @brief any
       *
       * This example is now using boost:any to store different instances. After that those get casted back to
       * their original type and print get called on it.
       *
       * @note This example does not about storing the boost:any, it only meant to play around with boost:any to
       * get a feeling for how to use it.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
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
