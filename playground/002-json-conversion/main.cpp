#include <iostream>

#include <boost/json.hpp>
#include <boost/json/src.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace json_conversion {
      /// Simple struct for testing
      struct Vector3d {
        int x;
        int y;
        int z;
      };

      /// Template class to convert any data type T to a json value
      template <typename T>
      struct AsJson {
        constexpr inline explicit AsJson(const T& ref)
          : ref(ref)
        {}

        const T& ref;

        operator boost::json::value() const;
      };

      /**
       * @brief Vector3d to JSON
       *
       * Template specialization for T = Vector3d conversion to a json object.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct AsJson<Vector3d> {
        /**
         * @brief constructor
         *
         * Stores a const reference to the Vector3d that shall be converted.
         *
         * @param ref Reference to the Vector3d that shall be converted.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline explicit AsJson(const Vector3d& ref)
          : ref(ref)
        {}

          /**
         * @brief json value cast
         *
         * Cast operator to be available as json value. This converts the stored
         * Vector3d reference as json object.
         */
        operator boost::json::value() const {
          boost::json::object result;

          result["x"] = ref.x;
          result["y"] = ref.y;
          result["z"] = ref.z;

          return result;
        }

        /// Const reference to the Vector3d that shall be converted
        const Vector3d& ref;
      };
    }
  }
}

/**
 * @brief JSON conversion
 *
 * Simple test that converts a Vector3d to be stored in json object. After
 * storing this is dumped to see and check the result.
 *
 * This has worked fine.
 *
 * @return Returns the exit code.
 */
int main()
{
  using namespace ts7::jsonrpc_playground::json_conversion;
  boost::json::object test;
  test["message"] = "vector demo";
  test["data"] = AsJson{Vector3d{1, 2, 3}};

  std::cout << test << std::endl;

  return 0;
}
