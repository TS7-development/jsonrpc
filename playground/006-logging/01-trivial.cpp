#include "logging.h"

#include <iostream>

#include <boost/log/trivial.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace logging {
      /**
       * @brief Trivial boost logging
       *
       * This example uses trivial boost looging to demonstrate how to use that.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      void trivial() {
        std::cout << "##### " << __PRETTY_FUNCTION__ << " #####" << std::endl;

        BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
        BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
        BOOST_LOG_TRIVIAL(info) << "An info severity message";
        BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
        BOOST_LOG_TRIVIAL(error) << "An error severity message";
        BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

        std::cout << std::endl;
      }
    }
  }
}
