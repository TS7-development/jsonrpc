#include <cstdint>
#include <iostream>

#include <boost/json.hpp>

#include <jsonrpc/request.hpp>
#include <jsonrpc/request_handler.hpp>

namespace ts7 {
  namespace jsonrpc_examples {
    namespace request_exexution {
      using RequestFailure = ts7::jsonrpc::RequestHandler<std::int32_t, std::int32_t, std::int32_t, std::int32_t>::maybe_failed;

      /**
       * @brief Test functions
       *
       * Simple function to test, if request handling is working.
       *
       * @param a Left hand side operand for an integer addition.
       * @param b Right hand side operand for and integer addition.
       *
       * @return Returns the sum of a & b.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      RequestFailure testFunc(const std::int32_t& id, std::int32_t a, std::int32_t b) {
        std::cout << id << ": " << a << " + " << b << " = " << (a+b) << std::endl;
        return a + b;
      }
    }
  }
}

int main()
{ 
  // Create a json rpc request
  ts7::jsonrpc::Request<ts7::jsonrpc::RequestID<std::int32_t>, std::int32_t, std::int32_t> generator(
    "testFunc",
    "a",
    "b"
  );

  // Create a json rpc handler
  ts7::jsonrpc::RequestHandler<std::int32_t, std::int32_t, std::int32_t, std::int32_t> handler(
    ts7::jsonrpc_examples::request_exexution::testFunc,
    "a",
    "b"
  );

  boost::json::object request = generator(3, 7);

  // Dump the request and the corresponding result
  std::cout << "<- " << request << std::endl;
  auto c = handler(request);
  std::cout << "-> " << static_cast<std::int32_t>(c) << std::endl;

  return 0;
}
