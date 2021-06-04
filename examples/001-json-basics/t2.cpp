#include <iostream>

#include <boost/json.hpp>

void t2(boost::json::object& request) {
  boost::json::object& params_ref = request["params"].as_object();
  params_ref["value1"] = 14;
  std::cout << request << std::endl;
  std::cout << "Object referencing: " << ((14 == request["params"].as_object()["value1"]) ? "Available" : "Not supported") << std::endl;
}
