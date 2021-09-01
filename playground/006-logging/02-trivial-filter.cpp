#include "logging.h"

#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace logging {
      /**
       * @brief Trivial filtering
       *
       * Demonstrates trivial serverity filtering.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      void trivialFilter() {
        std::cout << "##### " << __PRETTY_FUNCTION__ << " #####" << std::endl;

        // Setup Filter
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);

        // Log something
        BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
        BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
        BOOST_LOG_TRIVIAL(info) << "An info severity message";
        BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
        BOOST_LOG_TRIVIAL(error) << "An error severity message";
        BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

        // Restore filter
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::trace);

        std::cout << std::endl;
      }
    }
  }
}
