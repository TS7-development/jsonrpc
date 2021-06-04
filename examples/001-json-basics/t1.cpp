#include <iostream>

#include <boost/json.hpp>

/**
 * @brief Basic object update
 *
 * Updating the originally create parameter object and check, if it will be updated in the request as well by
 * internally knowing it's parent.
 *
 * This is not the case, so unfortunately this hasn't worked.
 *
 * @param request The request object.
 * @param params The parameter object.
 *
 * @since 1.0
 *
 * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
 */
void t1(boost::json::object& request, boost::json::object& params) {
  params["value1"] = 7;
  std::cout << request << std::endl;
  std::cout << "Object referencing: " << ((7 == request["params"].as_object()["value1"]) ? "Available" : "Not supported") << std::endl;
}
