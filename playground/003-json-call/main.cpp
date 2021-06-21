#include <functional>
#include <iostream>
#include <boost/json.hpp>
#include <boost/json/src.hpp>

#include "test.h"
#include "t4.h"
#include "t5.h"

/**
 * @brief Json RPC call
 *
 * Goal of this test is to design a system, that is capable of storing converting a Json RPC request with the
 * parameter value1 (string) and value2 (int) to a matching C++ function. What shall be avoided is constantly
 * passing around Json RPC requests when calling sub request handlers. This was how the old system worked, which
 * was technically working but was brought some disadvantages. As side of the fact that it was always extra code
 * to create the request, the main problem was that there was no compiler checking for changing function signatures.
 * Every mismatch had to be found at runtime, mostly with an expensive amount of debugging time.
 *
 * For testing the function signature was forced to be just a 2 parameter function with string and int. Later
 * thanks to templates also other parameter types are possible. To see how the process went of designing it and being
 * able to understand it later, every design step was saved as own test.
 *
 * @return Returns the exit code of the example.
 */
int main()
{
  using namespace ts7::jsonrpc_playground::json_call;

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
