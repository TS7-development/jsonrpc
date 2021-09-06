#include <cstdint>
#include <iostream>

#include <boost/json.hpp>

#include <jsonrpc/error.hpp>

int main()
{
  ts7::jsonrpc::error::ErrorCode ec = ts7::jsonrpc::error::ParameterWrongType("port", ts7::jsonrpc::util::JsonType::STRING, ts7::jsonrpc::util::JsonType::NUMBER);
  ts7::jsonrpc::Error<std::int32_t> e;
  boost::json::object o = e(7, ec);

  std::cout << o << std::endl;
  return 0;
}
