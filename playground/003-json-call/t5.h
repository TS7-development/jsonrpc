#pragma once

#include <string>

#include <boost/json.hpp>

#include "test.h"
#include "t4.h"

namespace ts7 {
  namespace jsonrpc {
    namespace playground {
      namespace json_call {
        /// Simple type for testing more complex parameters
        struct Vector3d {
            int x;
            int y;
            int z;
        };

        /**
         * @brief Convert to json
         *
         * Converts any type to a json.
         *
         * @note This version does not contain the static_assert. Check \ref FromJson, to see how this can be
         * achieved.
         *
         * @tparam T The type that shall be converted to a json value.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename T>
        struct AsJson {
          /**
           * @brief constructor
           *
           * Creates an instance that stores the const reference to the provided variable.
           *
           * @param ref Const reference that shall be used.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          constexpr inline explicit AsJson(const T& ref)
            : ref(ref)
          {}

          /// Stored const reference
          const T& ref;

          /**
           * @brief Cast operator
           *
           * Casts the provided const reference to a json value.
           *
           * @attention This operator is not implemented and will cause a linker error. For any type that shall be
           * supported a corresponding template specialization needs to be created.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          operator boost::json::value() const;
        };

        /**
         * @brief Vector3d to Json
         *
         * Provides a template specialization to convert a Vector3d to a json object.
         *
         * @note This is only required for easier assignment of the Vector3d for the test json object.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <>
        struct AsJson<Vector3d> {
          constexpr inline explicit AsJson(const Vector3d& ref)
            : ref(ref)
          {}

          /**
           * @brief Cast operator
           *
           * Converts the provided const reference to a Vector3d to a json object.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          operator boost::json::value() const {
            boost::json::object result;

            result["x"] = ref.x;
            result["y"] = ref.y;
            result["z"] = ref.z;

            return result;
          }

          /// Stored const reference
          const Vector3d& ref;
        };

        /**
         * @brief Convert from Json
         *
         * Converts a json object to a Vector3d.
         *
         * @attention This has no validation checks in this example to keep it as simple as possible.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <>
        struct FromJson<Vector3d> {
            Vector3d operator()(const boost::json::value& v) const {
              boost::json::object o = v.as_object();
              FromJson<int> x;
              FromJson<int> y;
              return Vector3d{
                // Using FromJson<int> specialization
                x(o["x"]),
                y(o["y"]),

                // Directly casting on its own, just to show off
                static_cast<int>(o["z"].as_int64())
              };
            }
        };

        /**
         * @brief Test version 5
         *
         * This type definition copies exactly the behaviour of \ref Test_v4.
         *
         * @note This test is only meant to try and demonstrate how to support a new type of json call back, still
         * limited to exatcly two parameters.
         *
         * @tparam TReturn Return type of the json callback.
         * @tparam T1 Type of the first parameter.
         * @tparam T2 Type of the second parameter.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename TReturn, typename T1, typename T2>
        using Test_v5 = Test_v4<TReturn, T1, T2>;
      }
    }
  }
}
