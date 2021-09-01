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

namespace ts7 {
  namespace jsonrpc_playground {
    namespace logging {
      /**
       * @brief Sink example
       *
       * Demonstrates how to add sinks to the boost logging core.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      void sink() {
        std::cout << "##### " << __PRETTY_FUNCTION__ << " #####" << std::endl;

        // Register common format placeholders
        boost::log::add_common_attributes();

        // Add single continous log file
        boost::log::add_file_log("simple.log");

        // Add daily rotated log file
        boost::log::add_file_log(
              boost::log::keywords::file_name = "rotation-%N.log",
              boost::log::keywords::open_mode = std::ios_base::app,
              boost::log::keywords::auto_flush = true,
              boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), // at midnight
              boost::log::keywords::format = "%TimeStamp% - %ProcessID%: %Message%"
        );

        // Add console (default is no longer used, since there are other sinks)
        boost::log::add_console_log(std::cout, boost::log::keywords::format = "%TimeStamp% - %Message%");

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
