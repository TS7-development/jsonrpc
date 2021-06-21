#include <iostream>

#include "test.h"
#include "t5.h"

namespace ts7 {
  namespace jsonrpc_playground {
    namespace json_call {
      /**
       * @brief Vector3d output stream operator
       *
       * Enables output stream dumping of a Vector3d.
       *
       * @param os Reference to the used output stream.
       * @param v Const reference to the Vector3d, that shall be dumped.
       *
       * @return Returns the reference to the used output stream.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      std::ostream& operator<<(std::ostream& os, const Vector3d& v) {
        return os << "{" << v.x << "," << v.y << "," << v.z << "}";
      }

      /**
       * @brief operator +
       *
       * Implements simple vector addition.
       *
       * @param lhs Left-hand-side operand.
       * @param rhs Right-hand-side operand.
       *
       * @return Returns the computed result vector.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      Vector3d operator+(const Vector3d& lhs, const Vector3d& rhs) {
        return Vector3d{
          lhs.x + rhs.x,
          lhs.y + rhs.y,
          lhs.z + rhs.z
        };
      }

      /**
       * @brief example5
       *
       * Examples to demonstrate how to use a json call back with a different function signature. This also
       * demonstrates how to use more complex parameter than just a string and an int. In addition this
       * example does not use the \ref testFunc, instead it is providing a lambda that is performining the vector
       * addition.
       *
       * @param test3 Json object that contains the two sub objects "a" and "b". Each representing a Vector3d.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
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
