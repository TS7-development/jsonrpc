#include <iostream>

#include <boost/json.hpp>

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace json_basics {
        /**
         * @brief Requesting object reference
         *
         * This test requests an object reference to the parameter from the request object. Updating the received parameter
         * object will now update the internal object of the request, which leads as expected to automatically updating the
         * request, which makes it not necessary to re-assign the parameter object to the request object.
         *
         * This has worked perfectly as intended.
         *
         * @param request A reference to the request object.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        void t2(boost::json::object& request) {
          boost::json::object& params_ref = request["params"].as_object();
          params_ref["value1"] = 14;
          std::cout << request << std::endl;
          std::cout << "Object referencing: " << ((14 == request["params"].as_object()["value1"]) ? "Available" : "Not supported") << std::endl;
        }
      }
    }
  }
}
