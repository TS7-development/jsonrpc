#include <iostream>

#include <boost/json.hpp>
#include <boost/json/src.hpp>

void t1(boost::json::object& request, boost::json::object& params);
void t2(boost::json::object& request);
void t3(boost::json::object& request);

int main()
{
    boost::json::object request;
    request["jsonrpc"] = "2.0";
    request["id"] = 1;
    request["method"] = "ts7.runExample";

    boost::json::object params;
    params["value1"] = 2;
    params["value2"] = "Hubba Bubba";

    request["params"] = params;

    std::cout << request << std::endl;

    t1(request, params);
    t2(request);
    t3(request);

    return 0;
}
