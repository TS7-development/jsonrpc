#pragma once

#include "util.hpp"

namespace ts7 {
  namespace jsonrpc {
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
        FromJson() = default;

        inline FromJson(const boost::json::value& v)
          : value(v.as_string().c_str())
        {}

        std::string operator()(const boost::json::value& v) const {
          assert(v.is_string());
          return v.as_string().c_str();
        }

        operator std::string() const {
          return value;
        }

        std::string value;
    };

    template <>
    struct FromJson<int> {
        FromJson() = default;

        inline FromJson(const boost::json::value& v)
          : value(static_cast<int>(v.as_int64()))
        {}

        int operator()(const boost::json::value& v) const {
          return static_cast<int>(v.as_int64());
        }

        operator int() const {
          return value;
        }

        int value;
    };
  }
}
