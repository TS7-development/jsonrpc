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
          constexpr inline FromJson() {
            // Source: https://artificial-mind.net/blog/2020/10/03/always-false
            static_assert(always_false<T>, "Unsupported type");
          }
      };

      /**
       * @brief bool from JSON
       *
       * Converts a boost::json::value to a bool.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<bool> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<bool, JsonType>;

          /// default constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::int32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong type, the actual type will be returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_bool()) {
              return GetJsonType(v);
            }

            return conversion_failure(v.as_bool());
          }
      };

      /**
       * @brief std::int32_t from JSON
       *
       * Converts a boost::json::value to a std::int32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<std::int8_t> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::int8_t, JsonType>;

          /// default constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::int32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong type, the actual type will be returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_int64()) {
              return GetJsonType(v);
            }

            return conversion_failure(static_cast<std::int8_t>(v.as_int64()));
          }
      };

      /**
       * @brief std::int32_t from JSON
       *
       * Converts a boost::json::value to a std::int32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<std::int16_t> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::int16_t, JsonType>;

          /// default constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::int32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong type, the actual type will be returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_int64()) {
              return GetJsonType(v);
            }

            return conversion_failure(static_cast<std::int16_t>(v.as_int64()));
          }
      };

      /**
       * @brief std::int32_t from JSON
       *
       * Converts a boost::json::value to a std::int32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<std::int32_t> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::int32_t, JsonType>;

          /// default constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::int32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong type, the actual type will be returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_int64()) {
              return GetJsonType(v);
            }

            return conversion_failure(static_cast<std::int32_t>(v.as_int64()));
          }
      };

      /**
       * @brief std::int32_t from JSON
       *
       * Converts a boost::json::value to a std::int32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<std::int64_t> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::int64_t, JsonType>;

          /// default constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::int32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong type, the actual type will be returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_int64()) {
              return GetJsonType(v);
            }

            return conversion_failure(v.as_int64());
          }
      };

      /**
       * @brief std::uint32_t from JSON
       *
       * Converts a boost::json::value to a std::uint32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<std::uint8_t> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::uint8_t, JsonType>;

          /// constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::uint32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong data type, the actual data type is returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if ( !v.is_uint64() && !v.is_int64() ) {
              return GetJsonType(v);
            }

            if ( v.is_int64() ) {
              return conversion_failure(static_cast<std::uint16_t>(v.as_int64()));
            }

            return conversion_failure(static_cast<std::uint8_t>(v.as_uint64()));
          }
      };

      /**
       * @brief std::uint32_t from JSON
       *
       * Converts a boost::json::value to a std::uint32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<std::uint16_t> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::uint16_t, JsonType>;

          /// constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::uint32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong data type, the actual data type is returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if ( !v.is_uint64() && !v.is_int64() ) {
              return GetJsonType(v);
            }

            if ( v.is_int64() ) {
              return conversion_failure(static_cast<std::uint16_t>(v.as_int64()));
            }

            return conversion_failure(static_cast<std::uint16_t>(v.as_uint64()));
          }
      };

      /**
       * @brief std::uint32_t from JSON
       *
       * Converts a boost::json::value to a std::uint32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<std::uint32_t> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::uint32_t, JsonType>;

          /// constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::uint32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong data type, the actual data type is returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if ( !v.is_uint64() && !v.is_int64() ) {
              return GetJsonType(v);
            }

            if ( v.is_int64() ) {
              return conversion_failure(static_cast<std::uint16_t>(v.as_int64()));
            }

            return conversion_failure(static_cast<std::uint32_t>(v.as_uint64()));
          }
      };

      /**
       * @brief std::uint32_t from JSON
       *
       * Converts a boost::json::value to a std::uint32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<std::uint64_t> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::uint64_t, JsonType>;

          /// constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::uint32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong data type, the actual data type is returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if ( !v.is_uint64() && !v.is_int64() ) {
              return GetJsonType(v);
            }

            if ( v.is_int64() ) {
              return conversion_failure(static_cast<std::uint16_t>(v.as_int64()));
            }

            return conversion_failure(v.as_uint64());
          }
      };

      /**
       * @brief std::uint32_t from JSON
       *
       * Converts a boost::json::value to a std::uint32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<float> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<float, JsonType>;

          /// constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::uint32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong data type, the actual data type is returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if ( !v.is_double() ) {
              return GetJsonType(v);
            }

            return conversion_failure(v.as_double());
          }
      };

      /**
       * @brief std::uint32_t from JSON
       *
       * Converts a boost::json::value to a std::uint32_t.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<double> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<double, JsonType>;

          /// constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::uint32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong data type, the actual data type is returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if ( !v.is_double() ) {
              return GetJsonType(v);
            }

            return conversion_failure(v.as_double());
          }
      };

      /**
       * @brief Json value to String
       *
       * Converts a json value to a string, if the provided content of
       * the boost::json::value is a string.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct FromJson<std::string> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::string, JsonType>;

          /// default constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::string.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value. If the provided value is
           * not a string, its actual type will be returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_string()) {
              return GetJsonType(v);
            }

            return conversion_failure(v.as_string().c_str());
          }
      };

      /**
       * @brief bool from JSON
       *
       * Converts a boost::json::value to a bool.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename T>
      struct FromJson<std::vector<T>> {
          /// Conversion failure data type
          using conversion_failure = error::maybe_failed<std::vector<T>, JsonType>;

          /// default constructor
          inline FromJson() = default;

          /**
           * @brief Conversion
           *
           * Converts the boost::json::value to a std::int32_t.
           *
           * @param v The value that shall be converted.
           *
           * @return Returns the converted value, if successful. If the value
           * has the wrong type, the actual type will be returned.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline conversion_failure operator()(const boost::json::value& v) const {
            if (!v.is_array()) {
              return GetJsonType(v);
            }

            const boost::json::array& a = v.as_array();

            std::vector<T> data;
            data.reserve(a.size());

            FromJson<T> conv;
            for(const boost::json::value& sv : a) {
              typename FromJson<T>::conversion_failure cf = conv(sv);
              if ( !cf ) {
                /// @todo How to return that error?!?
                return GetJsonType(sv);
              }

              data.push_back(cf.getSuccess());
            }

            return conversion_failure(data);
          }
      };
    }
  }
}
