#pragma once

#include <string>

#include <boost/json.hpp>

#include "always_false.hpp"
#include "jsontype.hpp"
#include "../error/error.hpp"

namespace ts7 {
  namespace jsonrpc {
    namespace util {
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

      template <>
      struct FromJson<std::int32_t> {
          using conversion_failure = error::maybe_failed<std::int32_t, JsonType>;

          inline FromJson() = default;

          conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_int64()) {
              return GetJsonType(v);
            }

            return conversion_failure(static_cast<std::int32_t>(v.as_int64()));
          }
      };

      template <>
      struct FromJson<std::uint32_t> {
          using conversion_failure = error::maybe_failed<std::uint32_t, JsonType>;

          inline FromJson() = default;

          conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_uint64()) {
              return GetJsonType(v);
            }

            return conversion_failure(static_cast<std::uint32_t>(v.as_uint64()));
          }
      };

      /**
       * @brief Json value to String
       *
       * Converts a json value to a string, if the provided content of the json::value was a string.
       */
      template <>
      struct FromJson<std::string> {
          using conversion_failure = error::maybe_failed<std::string, JsonType>;

          inline FromJson() = default;

          conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_string()) {
              return GetJsonType(v);
            }

            return conversion_failure(v.as_string().c_str());
          }
      };
    }
  }
}
