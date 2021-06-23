#include <iostream>

#include <callfromjson.hpp>
#include <procedure.hpp>
#include <request.hpp>

namespace ts7 {
  namespace jsonrpc_examples {
    namespace request_exexution {
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
      int testFunc(int a, int b) {
        std::cout << a << " + " << b << " = " << (a+b) << std::endl;
        return a + b;
      }
    }
  }
}

int main()
{ 
  // Create a json rpc request
  ts7::jsonrpc::Request<int> request = ts7::jsonrpc::Request<int>::Create(
    "testFunc",
    1,
    "a", 3,
    "b", 7
  );

  // Create the procedure to handle the request
  auto p = ts7::jsonrpc::make_request_handler<int>(
    ts7::jsonrpc::CallFromJson<int, int, int>(
      ts7::jsonrpc_examples::request_exexution::testFunc,
      "a",
      "b"
    )
  );

  // Dump the request and the corresponding result
  std::cout << "<- " << request << std::endl;
  auto c = p.onRequest(request);
  std::cout << "-> " << c << std::endl;

  return 0;
}
