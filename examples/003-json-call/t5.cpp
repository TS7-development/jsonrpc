#include <iostream>

#include "test.h"
#include "t5.h"

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace json_call {
        std::ostream& operator<<(std::ostream& os, const Vector3d& v) {
          return os << "{" << v.x << "," << v.y << "," << v.z << "}";
        }

        Vector3d operator+(const Vector3d& lhs, const Vector3d& rhs) {
          return Vector3d{
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z
          };
        }

        void example5(const boost::json::object& test3) {
          using Tv5 = Test_v5<Vector3d, const Vector3d&, const Vector3d&>;
          Tv5 t5(
            [](const Vector3d& lhs, const Vector3d& rhs) -> Vector3d {
              Vector3d r = lhs + rhs;
              std::cout << "r = " << lhs << " + " << rhs << " = " << r << std::endl;
              return r;
            },
            "a",
            "b"
          );

          t5(test3);
        }
      }
    }
  }
}
