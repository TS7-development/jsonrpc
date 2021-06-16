#include "test.h"

#include <boost/variant.hpp>

namespace ts7 {
  namespace jsonrpc {
    namespace playground {
      namespace any_collection {
        /**
         * @brief variant_visitor
         *
         * Visitor that calls the print() member function on the stored variant type.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        struct variant_visitor : public boost::static_visitor<> {
            /**
             * @brief Print call
             *
             * Calls the print() member function on the variant.
             *
             * @tparam T The original type of the stored variant.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            template<typename  T>
            void operator()(const T& t) const {
              t.print();
            }
        };

        /**
         * @brief variant example
         *
         * Example to demonstrate simple usage of the boost variant type.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
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
