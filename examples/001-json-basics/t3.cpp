#include <iostream>

#include <boost/json.hpp>

void t3(boost::json::object& request) {
  std::stringstream ss;
  ss << request;
  std::string s = ss.str();
  std::cout << "As String: " << s << std::endl;
}
