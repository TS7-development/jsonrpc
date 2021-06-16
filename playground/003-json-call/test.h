#pragma once

#include <cassert>
#include <cxxabi.h>
#include <string>

#include <boost/json.hpp>

namespace ts7 {
  namespace jsonrpc {
    namespace playground {
      namespace json_call {
        /// Simple test function, which just dumps both parameters
        void testFunc(const std::string& value1, int value2);

        /**
         * @brief Always false
         *
         * This bool is a evaluated at compile time and will be always false no matter which and how many types are
         * provided to the variadic template. This is required for static_assert. If you just pass false to
         * it, the compiler will optimize it and directly fail, because it will be false no matter which type is
         * provided. This bool on the other hand requires the types to be evaluated to it can't be directly
         * optimized away and will only lead to the static_assert fail, if a certain type is used.
         *
         * This approach is decribed in:
         * https://artificial-mind.net/blog/2020/10/03/always-false
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template< typename ... T>
        constexpr bool always_false = false;

        /**
         * @brief From Json Converter
         *
         * This template is meant to convert from a json value to any other type. Every supported type needs it's own
         * specialization. All unsupported types will use the unspecialized template, which will fail a
         * static_assert in its constructor. This way you will see at compile time, that the software does not know
         * how to convert from json value to your specific type.
         *
         * @note The error message will anywhere contain "[T = Dummy]" (assuming the type was Dummy). The exact
         * wording and format is compiler dependend.
         *
         * @todo Find a way to embedded the typename into the error message.
         *       Already tried:
         *        - https://akrzemi1.wordpress.com/2017/06/28/compile-time-string-concatenation/
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename T>
        struct FromJson {
            /**
             * @brief constructor
             *
             * This constructor will always fail at compile time. The error message depends in wording and format on
             * the compiler, but should always anywhere contain, for which type T it has failed. This means that
             * this type has not specialized conversion, which needs to be done to support it.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            constexpr FromJson() {
              // Source: https://artificial-mind.net/blog/2020/10/03/always-false
              static_assert(always_false<T>, "Unsupported type");
            }
        };

        /**
         * @brief Json value to String
         *
         * Converts a json value to a string, if the provided content of the json::value was a string.
         */
        template <>
        struct FromJson<std::string> {
            std::string operator()(const boost::json::value& v) const {
              assert(v.is_string());
              return v.as_string().c_str();
            }
        };

        template <>
        struct FromJson<int> {
            int operator()(const boost::json::value& v) const {
              return static_cast<int>(v.as_int64());
            }
        };

        void example1(const boost::json::object& test);
        void example2(const boost::json::object& test);
        void example3(const boost::json::object& test);
        void example4(const boost::json::object& test, const boost::json::object& test2);
        void example5(const boost::json::object& test3);
        void example6(const boost::json::object& test3);
      }
    }
  }
}
