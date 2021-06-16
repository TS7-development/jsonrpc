#pragma once

#include <cxxabi.h>
#include <iostream>

namespace ts7 {
  namespace jsonrpc {
    namespace playground {
      namespace any_collection {
        /// Testing struct A
        struct A {
            /// JUst prints "A"
            void print() const {
              std::cout << "A" << std::endl;
            }
        };

        /// Testing struct B
        struct B {
            /// Just prints "B"
            void print() const {
              std::cout << "B" << std::endl;
            }
        };

        /**
         * @brief Type name
         *
         * @tparam Type that shall be demangled.
         *
         * @return Returns the demangled type name.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename T>
        char* type_name() {
          return abi::__cxa_demangle(typeid (T).name(), 0, 0, nullptr);
        }


        /// Templated testing struct C
        template <typename T>
        struct C {
            /// Just prints "C<T>", where T is replaced by the demangled type name.
            void print() const {
              std::cout << "C<" << type_name<T>() << ">" << std::endl;
            }
        };

        /// Create a default instance of T and calls print on it
        template <typename T>
        void print() {
          T t;
          t.print();
        }

        void simple();
        void any();
        void storage();
        void variant();
        void variant_storage();
      }
    }
  }
}
