#include "test.h"

#include <map>
#include <string>

#include <boost/variant.hpp>

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace any_collection {
        /// Variant type definition
        using variant_type = boost::variant<A, B, C<int>, C<double>, C<A>>;

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
         * @brief The VariantStorage struct
         *
         * %Storage class for supported types of the \ref variant_type.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        struct VariantStorage {
            /**
             * @brief add
             *
             * Adds an instance of a type that is supported by \ref variant_type.
             *
             * @tparam T Type of the instance that shall be added.
             *
             * @param name The name of the element that shall be added.
             * @param t Forward reference of the instance that shall be added.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            template <typename T>
            void add(const std::string& name, T&& t) {
              callbacks[name] = std::forward<T>(t);
            }

            /**
             * @brief print
             *
             * Calls the print() member function of the under \p name stored element.
             *
             * @note This requires the usage of \ref variant_visitor.
             *
             * @param name Name of the element.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            void print(const std::string& name) {
              boost::apply_visitor(variant_visitor(), callbacks[name]);
            }

            /// Variant storage map
            std::map<std::string, variant_type> callbacks;
        };

        /**
         * @brief variant_storage example
         *
         * This example demonstrates how to store elements by using boost::variant.
         *
         * This has worked quite fine. But compared to boost:any example \ref storage, this has some problems.
         * First we always need to update \ref variant_type to support every new type (i.e. every template
         * combination for a JSON RPC callback signature). Secondly, for every new method that shall be callable on
         * those type, we also need to implement a new visitor. This is basically the same workload as with
         * boost::any that requires a new callback lambda for every new supported member function.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        void variant_storage() {
          std::cout << "VariantStorage print" << std::endl;

          VariantStorage vs;
          vs.add("ConfigurationSend", A());
          vs.add("ConfigurationRead", B());
          vs.add("ConfigurationChecksum", C<int>());

          vs.print("ConfigurationSend");
          vs.print("ConfigurationRead");
          vs.print("ConfigurationChecksum");

          std::cout << std::endl;
        }
      }
    }
  }
}
