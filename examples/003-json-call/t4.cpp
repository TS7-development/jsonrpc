#include "test.h"
#include "t4.h"

void example4(const boost::json::object& test, const boost::json::object& test2) {
  using Tv4 = Test_v4<void, const std::string&, int>;
  Tv4 t4(testFunc, "value1", Tv4::OptionalParameter<int>("value2", 0));
  t4(test);
  t4(test2);
}
