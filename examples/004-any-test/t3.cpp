#include "test.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

#include <boost/any.hpp>

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace any_collection {
        /**
         * @brief boost::any storage
         *
         * Stores instances of boost:any and is capable of calling member functions to those.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        struct Storage {
            /// Function pointer to call print on a boost::any.
            using print_t = std::function<void(const boost::any&)>;

            /// Storage entry node
            struct node_t {
                /// RTTI type id hash
                std::size_t type_hash;
                /// Any value
                boost::any value;
            };

            /**
             * @brief Add any type
             *
             * Adds an instance of any type, defined by \p U.
             *
             * @note This function is not only the instance under it's name in a map. Also a lambda is created that
             * casts the any type to it's original type and calls the member function print() on it, stored by
             * it's type id hash.
             *
             * @tparam U The type of the instance that shall be stored.
             *
             * @param name Name of the element to store, like a JSON RPC method name.
             * @param u The forward reference of an instance, that shall be stored.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            template <typename U>
            void add(const std::string& name, U&& u) {
              elements[name] = node_t{typeid(U).hash_code(), std::forward<U>(u)};
              types[typeid(U).hash_code()] = [](const boost::any& a) {
                boost::any_cast<U>(a).print();
              };
            }

            /**
             * @brief print
             *
             * Calls the print function on the instance stored with under \p name.
             *
             * @param name Name of the element, that shall be used.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            void print(const std::string& name) {
              const node_t& node = elements[name];
              print_t p = types[node.type_hash];
              p(node.value);
            }

            /// Stored elements map
            std::map<std::string, node_t> elements;

            /// Stored type cast lambda's to call print()
            std::map<std::size_t, print_t> types;
        };

        /**
         * @brief storage example
         *
         * Demonstrates how to store and call \p print with different types, which is working fine with the boost::any type.
         *
         * Advantage of that solution is, that there is no need to adjust that \ref Storage, if a new type is added
         * since boost::any can store any type. In exchange the \ref Storage needs to be adjusted for every new
         * member function that shall be suppored, which happens way less than new types.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        void storage() {
          std::cout << "Storage print" << std::endl;

          Storage s;
          s.add("ConfigurationSend", A());
          s.add("ConfigurationRead", B());
          s.add("ConfigurationChecksum", C<int>());
          s.add("ConfigurationTime", C<double>());
          s.add("ConfigurationInvalidate", A());

          s.print("ConfigurationSend");
          s.print("ConfigurationChecksum");

          std::cout << std::endl;
        }
      }
    }
  }
}
