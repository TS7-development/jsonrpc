#pragma once

#include <string>

#include <boost/json.hpp>

#include "test.h"
#include "t4.h"

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace json_call {
        /// Simple type for testing more complex parameters
        struct Vector3d {
            int x;
            int y;
            int z;
        };

        template <typename T>
        struct AsJson {
          constexpr inline explicit AsJson(const T& ref)
            : ref(ref)
          {}

          const T& ref;

          operator boost::json::value() const;
        };

        template <>
        struct AsJson<Vector3d> {
          constexpr inline explicit AsJson(const Vector3d& ref)
            : ref(ref)
          {}

          operator boost::json::value() const {
            boost::json::object result;

            result["x"] = ref.x;
            result["y"] = ref.y;
            result["z"] = ref.z;

            return result;
          }

          const Vector3d& ref;
        };

        template <>
        struct FromJson<Vector3d> {
            Vector3d operator()(const boost::json::value& v) const {
              boost::json::object o = v.as_object();
              FromJson<int> x;
              FromJson<int> y;
              return Vector3d{
                x(o["x"]),
                y(o["y"]),
                static_cast<int>(o["z"].as_int64())
              };
            }
        };

        template <typename TReturn, typename T1, typename T2>
        using Test_v5 = Test_v4<TReturn, T1, T2>;
      }
    }
  }
}
