#include <iostream>

#include <boost/json.hpp>

/**
 * @brief Convert to string
 *
 * Simple test to see how to convert a JSON object to a string, that can be used for later transmission.
 *
 * @param request Reference to the request object.
 *
 * @since 1.0
 *
 * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
 */
void t3(boost::json::object& request) {
  std::stringstream ss;
  ss << request;
  std::string s = ss.str();
  std::cout << "As String: " << s << std::endl;
}
