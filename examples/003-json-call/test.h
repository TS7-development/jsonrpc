#pragma once

#include <string>

#include <boost/json.hpp>

void testFunc(const std::string& value1, int value2);

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

template< typename ... T>
constexpr bool always_false = false;

template <typename T>
struct FromJson {
    constexpr FromJson() {
      static_assert(always_false<T>, "Unsupported type for FromJson");
    }

    T operator()(const boost::json::value& v) const;
};

template <>
struct FromJson<std::string> {
    std::string operator()(const boost::json::value& v) const {
      return v.as_string().c_str();
    }
};

template <>
struct FromJson<int> {
    int operator()(const boost::json::value& v) const {
      return static_cast<int>(v.as_int64());
    }
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

void example1(const boost::json::object& test);
void example2(const boost::json::object& test);
void example3(const boost::json::object& test);
void example4(const boost::json::object& test, const boost::json::object& test2);
void example5(const boost::json::object& test3);
void example6(const boost::json::object& test3);
