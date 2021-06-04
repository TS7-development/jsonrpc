#include <functional>
#include <iostream>
#include <boost/json.hpp>
#include <boost/json/src.hpp>

#include "test.h"

int main()
{
  boost::json::object test;
  test["value1"] = "Hallo Welt";
  test["value2"] = 7;

  example1(test);
  example2(test);
  example3(test);

  boost::json::object test2;
  test2["value1"] = "Hello World";

  example4(test, test2);

  boost::json::object test3;
  test3["a"] = AsJson{Vector3d{1,2,3}};
  test3["b"] = AsJson{Vector3d{4,5,6}};

  example5(test3);
  example6(test3);

  return 0;
}
