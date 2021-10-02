#include <cstdint>
#include <iostream>

#include <jsonrpc/request.hpp>
#include <jsonrpc/procedure.hpp>

namespace ts7 {
  namespace jsonrpc_examples {
    namespace procedure_usage {
      std::int32_t sum_success(std::int32_t a, std::int32_t b) {
        return a + b;
      }

      std::int32_t sum_error(std::int32_t, std::int32_t) {
        throw ts7::jsonrpc::error::Exception(static_cast<std::int32_t>(ts7::jsonrpc::error::ErrorCodes::INVALID_REQUEST), "Invalid Request");
      }

      std::int32_t sum_failure(std::int32_t, std::int32_t) {
        throw std::runtime_error("Can't calculate the sum");
      }
    }
  }
}

int main()
{
  // Request generator
  ts7::jsonrpc::Request<ts7::jsonrpc::RequestID<std::int32_t>, std::int32_t,  std::int32_t> generator("sum", "a", "b");
  boost::json::object request = generator(3, 7);

  // Success case
  ts7::jsonrpc::Procedure<std::int32_t, std::int32_t, std::int32_t, std::int32_t> success(
    ts7::jsonrpc_examples::procedure_usage::sum_success,
    "a",
    "b"
  );

  std::cout << "<- " << request << std::endl;
  boost::json::value response = success(request);
  std::cout << "-> " << response << std::endl << std::endl;

  // Exception case
  ts7::jsonrpc::Procedure<std::int32_t, std::int32_t, std::int32_t, std::int32_t> except(
    ts7::jsonrpc_examples::procedure_usage::sum_error,
    "a",
    "b"
  );

  std::cout << "<- " << request << std::endl;
  boost::json::value exception = except(request);
  std::cout << "-> " << exception << std::endl << std::endl;

  // Failure case
  ts7::jsonrpc::Procedure<std::int32_t, std::int32_t, std::int32_t, std::int32_t> failure(
    ts7::jsonrpc_examples::procedure_usage::sum_failure,
    "a",
    "b"
  );

  std::cout << "<- " << request << std::endl;
  boost::json::value error = failure(request);
  std::cout << "-> " << error << std::endl << std::endl;

  return 0;
}
