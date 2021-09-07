#include <cstdint>
#include <iostream>

#include <jsonrpc/module.hpp>
#include <jsonrpc/procedure.hpp>

namespace ts7 {
  namespace jsonrpc_examples {
    namespace bind {
      class Math : public ts7::jsonrpc::Module<std::int32_t> {
        public:
          using Module = ts7::jsonrpc::Module<std::int32_t>;
          using RequestID = ts7::jsonrpc::RequestID<id_t>;
          using Request = ts7::jsonrpc::Request<RequestID, std::int32_t, std::int32_t>;
          using Procedure = ts7::jsonrpc::Procedure<id_t, std::int32_t, std::int32_t, std::int32_t>;

          inline Math() {
            procedures["math.sum"] = Procedure(std::bind(&Math::sum, this, std::placeholders::_1, std::placeholders::_2), "a", "b");
            procedures["math.difference"] = Procedure(std::bind(&Math::difference, this, std::placeholders::_1, std::placeholders::_2), "a", "b");
            procedures["math.multiply"] = Procedure(std::bind(&Math::multiply, this, std::placeholders::_1, std::placeholders::_2), "a", "b");
            procedures["math.divide"] = Procedure(std::bind(&Math::divide, this, std::placeholders::_1, std::placeholders::_2), "a", "b");
          }

          inline void request_sum(std::int32_t a, std::int32_t b) {
            test(sum_request, a, b);
          }

          inline void request_difference(std::int32_t a, std::int32_t b) {
            test(difference_request, a, b);
          }

          inline void request_multiply(std::int32_t a, std::int32_t b) {
            test(multiply_request, a, b);
          }

          inline void request_divide(std::int32_t a, std::int32_t b) {
            test(divide_request, a, b);
          }
        protected:
          template <typename... TArgs>
          void test(const Request& request, TArgs... args) {
            boost::json::object in = request(args...);
            std::cout << "<- " << in << std::endl;

            boost::json::object out = Module::operator()(in);
            std::cout << "-> " << out << std::endl << std::endl;
          }

          std::int32_t sum(std::int32_t a, std::int32_t b) {
            return a + b;
          }

          std::int32_t difference(std::int32_t a, std::int32_t b) {
            return a - b;
          }

          std::int32_t multiply(std::int32_t a, std::int32_t b) {
            return a * b;
          }

          std::int32_t divide(std::int32_t a, std::int32_t b) {
            if (0 == b) {
              throw ts7::jsonrpc::error::Exception(static_cast<std::int32_t>(ts7::jsonrpc::error::ErrorCodes::INTERNAL_ERROR), "Division by zero");
            }
            return a / b;
          }

        protected:
          Request sum_request = Request("math.sum", "a", "b");
          Request difference_request = Request("math.difference", "a", "b");
          Request multiply_request = Request("math.multiply", "a", "b");
          Request divide_request = Request("math.divide", "a", "b");
      };
    }
  }
}

int main()
{
  ts7::jsonrpc_examples::bind::Math math;
  math.request_sum(3, 7);
  math.request_difference(3, 7);
  math.request_multiply(3, 7);
  math.request_divide(21, 7);
  math.request_divide(21, 0);

  return 0;
}
