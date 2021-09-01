#include "ini-files.h"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace ini {
      namespace v1 {
        /**
         * @brief Dumb config parameter
         *
         * Reads and dumbs a configuration value.
         *
         * @tparam T Data type of the configuration value.
         *
         * @param pt Property tree that contains the read configuration values.
         * @param identifier Path to identify the configuration value.
         * @param defaultValue Default value, in case the configuration value can't be found.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename T>
        static inline void ini_dumb(const boost::property_tree::ptree& pt, const char* identifier, T defaultValue = T()) {
          std::cout << identifier << " = "<< pt.get<T>(identifier, defaultValue) << std::endl;
        }

        /**
         * @brief Simple INI parsing
         *
         * Demonstrates how to simply parse an INI file.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        void simple() {
          std::cout << "##### " << __PRETTY_FUNCTION__ << " #####" << std::endl;

          boost::property_tree::ptree pt;

          if (boost::filesystem::exists("test.ini")) {
          boost::property_tree::ini_parser::read_ini("test.ini", pt);
          }
          else {
            std::cout << "test.ini does not exists" << std::endl;
          }

          ini_dumb<std::string>(pt, "Section1.Str", "Hello World");
          ini_dumb<int>(pt, "Section1.Int", 7);
          ini_dumb<std::string>(pt, "Section1.Missing", "You will miss me badly");
          ini_dumb<std::string>(pt, "Missing.Str", "Shame on you, missing me as well");
        }
      }
    }
  }
}
