#include "test.h"

#include <iostream>

void testFunc(const std::string& value1, int value2) {
  std::cout << "v1=" << value1 << ", v2=" << value2 << std::endl;
}

struct Test_v1 {
    inline explicit Test_v1(std::function<void(const std::string&, int)> fn)
      : fn(fn)
    {}

    void operator()(const boost::json::object& o) {
      std::string v1 = o.at("value1").as_string().c_str();
      int v2 = o.at("value2").as_int64();

      fn(v1, v2);
    }

    std::function<void(const std::string&, int)> fn;
};

void example1(const boost::json::object& test) {
  Test_v1 t(testFunc);
  t(test);
}
