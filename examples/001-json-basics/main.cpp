#include <iostream>

#include <boost/json.hpp>
#include <boost/json/src.hpp>

void t1(boost::json::object& request, boost::json::object& params);
void t2(boost::json::object& request);
void t3(boost::json::object& request);

/**
 * @brief JSON basics
 *
 * Goal of this example is to figure, how to use the boost::json library. Most crucial information is the capability
 * of providing object references, to avoid constantly updating parents when an object has changed. This was
 * required in Qt 5.11 and was super annoying and has lead to a lot of avoidable errors.
 *
 * @since 1.0
 *
 * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
 *
 * @see t1, t2, t3
 *
 * @return Returns the application exit code.
 */
int main()
{
    // Create a basic json rpc request
    boost::json::object request;
    request["jsonrpc"] = "2.0";
    request["id"] = 1;
    request["method"] = "ts7.runExample";

    // add parameter to the request
    boost::json::object params;
    params["value1"] = 2;
    params["value2"] = "Hubba Bubba";

    request["params"] = params;

    // dump
    std::cout << request << std::endl;

    // run tests
    t1(request, params);
    t2(request);
    t3(request);

    return 0;
}
