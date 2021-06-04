#include <iostream>

#include <boost/json.hpp>
#include <boost/json/src.hpp>

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


int main()
{
    boost::json::object test;
    test["message"] = "vector demo";
    test["data"] = AsJson{Vector3d{1, 2, 3}};

    std::cout << test << std::endl;

    return 0;
}
