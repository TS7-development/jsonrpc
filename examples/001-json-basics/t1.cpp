#include <iostream>

#include <boost/json.hpp>

void t1(boost::json::object& request, boost::json::object& params) {
  params["value1"] = 7;
  std::cout << request << std::endl;
  std::cout << "Object referencing: " << ((7 == request["params"].as_object()["value1"]) ? "Available" : "Not supported") << std::endl;
}
