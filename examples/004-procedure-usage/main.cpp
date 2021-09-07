#include <cstdint>
#include <iostream>

#include <jsonrpc/request.hpp>
#include <jsonrpc/procedure.hpp>

std::int32_t sum_success(std::int32_t a, std::int32_t b) {
  return a + b;
}

std::int32_t sum_failure(std::int32_t, std::int32_t) {
  throw std::runtime_error("Can't calculate the sum");
}

int main()
{
  // Request generator
  ts7::jsonrpc::Request<ts7::jsonrpc::RequestID<std::int32_t>, std::int32_t,  std::int32_t> generator("sum", "a", "b");
  boost::json::object request = generator(3, 7);

  // Success case
  ts7::jsonrpc::Procedure<std::int32_t, std::int32_t, std::int32_t, std::int32_t> success(
    sum_success,
    "a",
    "b"
  );

  std::cout << "<- " << request << std::endl;
  boost::json::object response = success(request);
  std::cout << "-> " << response << std::endl << std::endl;

  // Failure case
  ts7::jsonrpc::Procedure<std::int32_t, std::int32_t, std::int32_t, std::int32_t> failure(
    sum_failure,
    "a",
    "b"
  );

  std::cout << "<- " << request << std::endl;
  boost::json::object error = failure(request);
  std::cout << "-> " << error << std::endl << std::endl;

  return 0;
}
