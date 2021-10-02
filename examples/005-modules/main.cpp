#include <cstdint>
#include <iostream>

#include <jsonrpc/module.hpp>
#include <jsonrpc/procedure.hpp>
#include <jsonrpc/request.hpp>

using MathRequest = ts7::jsonrpc::Request<ts7::jsonrpc::RequestID<std::int32_t>, std::int32_t, std::int32_t>;
using MathProcedure = ts7::jsonrpc::Procedure<std::int32_t, std::int32_t, std::int32_t, std::int32_t>;
using MathModule = ts7::jsonrpc::Module<std::int32_t>;

namespace ts7 {
  namespace jsonrpc_examples {
    namespace modules {
      std::int32_t sum(std::int32_t a, std::int32_t b) {
        return a + b;
      }

      std::int32_t difference(std::int32_t a, std::int32_t b) {
        return a - b;
      }

      std::int32_t product(std::int32_t a, std::int32_t b) {
        return a * b;
      }

      std::int32_t div(std::int32_t a, std::int32_t b) {
        if (0 == b) {
          throw std::runtime_error("Division by zero");
        }
        return a / b;
      }

      template <typename... TArgs>
      void test(MathModule& module, const MathRequest& request, TArgs... args) {
        boost::json::object in = request(args...);
        std::cout << "<- " << in << std::endl;

        boost::json::value out = module(in);
        std::cout << "-> " << out << std::endl << std::endl;
      }
    }
  }
}

int main()
{
  MathRequest sum_request("sum", "a", "b");
  MathRequest difference_request("difference", "a", "b");
  MathRequest product_request("product", "a", "b");
  MathRequest div_request("divide", "a", "b");

  MathProcedure sum_procedure(ts7::jsonrpc_examples::modules::sum, "a", "b");
  MathProcedure difference_procedure(ts7::jsonrpc_examples::modules::difference, "a", "b");
  MathProcedure product_procedure(ts7::jsonrpc_examples::modules::product, "a", "b");
  MathProcedure div_procedure(ts7::jsonrpc_examples::modules::div, "a", "b");

  MathModule module;

  module.addRequest("sum", sum_procedure);
  module.addRequest("difference", difference_procedure);
  module.addRequest("product", product_procedure);
  module.addRequest("divide", div_procedure);


  ts7::jsonrpc_examples::modules::test(module, sum_request, 3, 7);
  ts7::jsonrpc_examples::modules::test(module, difference_request, 3, 7);
  ts7::jsonrpc_examples::modules::test(module, product_request, 3, 7);
  ts7::jsonrpc_examples::modules::test(module, div_request, 21, 7);
  ts7::jsonrpc_examples::modules::test(module, div_request, 21, 0);

  return 0;
}
