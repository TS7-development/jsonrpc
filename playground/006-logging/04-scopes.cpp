#include "logging.h"

#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions/formatters/named_scope.hpp>
#include <boost/log/support/date_time.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace logging {
      /**
       * @brief Scope example
       *
       * Demonstrates how to use scopes with the boost logging library. Also demonstrates how to setup the
       * message format.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      void scopes() {
        std::cout << "##### " << __PRETTY_FUNCTION__ << " #####" << std::endl;

        // Register common format placeholders
        boost::log::add_common_attributes();

        // Add console (default is no longer used, since there are other sinks)
        boost::log::add_console_log(
          std::cout,
          boost::log::keywords::format = boost::log::expressions::stream
            << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f")
            << " - "
            << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
            << ": "
            << boost::log::expressions::message
            << " ["
            << boost::log::expressions::format_named_scope("Scope", boost::log::keywords::format = "%f:%l")
            << "]"
        );

        // Set scope
        boost::log::core::get()->add_thread_attribute("Scope", boost::log::attributes::named_scope());
        BOOST_LOG_FUNCTION();

        // Start logging
        BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
        BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
        BOOST_LOG_TRIVIAL(info) << "An info severity message";
        BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
        BOOST_LOG_TRIVIAL(error) << "An error severity message";
        BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

        // Remove log files
        boost::log::core::get()->remove_all_sinks();

        std::cout << std::endl;
      }
    }
  }
}
