#include "test.h"

#include <boost/variant.hpp>

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace any_collection {
        struct variant_visitor : public boost::static_visitor<> {
            template<typename  T>
            void operator()(const T& t) const {
              t.print();
            }
        };

        void variant() {
          using variant_type = boost::variant<A, B, C<int>, C<double>, C<A>>;

          std::cout << "Variant print" << std::endl;

          variant_type v1 = A();
          variant_type v2 = B();
          variant_type v3 = C<int>();

          boost::apply_visitor(variant_visitor(), v1);
          boost::apply_visitor(variant_visitor(), v2);
          boost::apply_visitor(variant_visitor(), v3);

          std::cout << std::endl;
        }
      }
    }
  }
}
