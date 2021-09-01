#include "ini-files.h"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/variant.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace ini {
      namespace v3 {
        /**
         * @brief Configuration
         *
         * Class that maps configuration values of a property tree to a named structure.
         *
         * @tparam TData Data type of the structure that shall be used to store the read values.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename TData>
        struct Configuration {
            /**
             * @brief Configuration entry
             *
             * Represents a single entry in the configuration.
             *
             * @tparam T Data type of the configuration entry.
             *
             * @param identifier Identifier of the configuration entry.
             * @param offset Pointer to the member variable that needs to be filled.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            template <typename T>
            struct Entry {
              inline Entry(const char* identifier, T TData::* offset)
                : identifier(identifier),
                  offset(offset),
                  defaultValue(T()),
                  hasDefault(false)
              {}

              /**
               * @brief constructor
               *
               * Creates a configuration entry without a default value.
               *
               * @param identifier Identifier of the entry.
               * @param offset Pointer to the structure data member.
               *
               * @since 1.0
               *
               * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
               */
              inline Entry(const char* identifier, T TData::* offset, const T& defaultValue)
                : identifier(identifier),
                  offset(offset),
                  defaultValue(defaultValue),
                  hasDefault(true)
              {}

              /// Copy constructor
              Entry(const Entry&) = default;

              /// Move constructor
              Entry(Entry&&) = default;

              /**
               * @brief read
               *
               * Reads the configuration value and stores in the corresponding member of the
               * provided data structure.
               *
               * @param pt Property tree that contains the entry.
               * @param data Reference to the data structure that shall be filled.
               *
               * @since 1.0
               *
               * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
               */
              inline void read(boost::property_tree::ptree& pt, TData& data) {
                if (hasDefault) {
                  data.*offset = pt.get<T>(identifier, defaultValue);
                }
                else {
                  data.*offset = pt.get<T>(identifier);
                }
              }

              /// Path the configuration entry
              const char* identifier;

              /// Pointer to the member that shall be filled
              T TData::* offset;

              /// Default value, if a default value was provided
              T defaultValue;

              /// True, if a default value exists. Otherwise it is False.
              const bool hasDefault;
            };

            /// Variant data type for configuration entries
            using variant_t = boost::variant<Entry<bool>, Entry<int>, Entry<float>, Entry<std::string>>;

            /**
             * @brief Read visitor
             *
             * Visits the configuration entry of the variant.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            struct read_visitor : public boost::static_visitor<> {
                /**
                 * @brief constructor
                 *
                 * Creates a read visitor with references to a property tree
                 * and the final data structure.
                 *
                 * @param pt Reference to a property tree to read from.
                 * @param data Reference to the data structure to read to.
                 *
                 * @since 1.0
                 *
                 * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
                 */
                inline read_visitor(boost::property_tree::ptree& pt, TData& data)
                  : pt(pt),
                    data(data)
                {}

                /**
                 * @brief Print call
                 *
                 * Calls the print() member function on the variant.
                 *
                 * @tparam T The original type of the stored variant.
                 *
                 * @since 1.0
                 *
                 * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
                 */
                template<typename  T>
                void operator()(T& t) const {
                  t.read(pt, data);
                }

                /// Property tree reference to read from
                boost::property_tree::ptree& pt;

                /// Reference to the target data structure
                TData& data;
            };

            /**
             * @brief Add entry
             *
             * Adds a new entry without a default value.
             *
             * @tparam T Data type of the configuration entry.
             *
             * @param identifier Identifier of the configuration entry.
             * @param offset Pointer to the data member.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            template <typename T>
            void add(const char* identifier, T TData::* offset) {
              add(Entry<T>(identifier, offset));
            }

            /**
             * @brief Add entry
             *
             * Adds a new entry with a default value.
             *
             * @tparam T Data type of the configuration entry.
             *
             * @param identifier Identifier of the configuration entry.
             * @param offset Pointer to the data member.
             * @param defaultValue Default value, if the configuration entry can't be found.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            template <typename T>
            void add(const char* identifier, T TData::* offset, const T& defaultValue) {
              add(Entry<T>(identifier, offset, defaultValue));
            }

            /**
             * @brief add entry
             *
             * Adds an entry.
             *
             * @tparam T Data type of the configuration value.
             *
             * @param e The entry that shall be created.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            template <typename T>
            inline void add(const Entry<T>& e) {
              values.push_back(e);
            }

            /**
             * @brief read
             *
             * Reads the INI file to a property tree and then transfers the values with
             * the entries to the data structure.
             *
             * @param filename Name of the file that shall be read.
             * @param data Data that shall be filed.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            void read(const char* filename, TData& data) {
              boost::property_tree::ptree pt;

              if (boost::filesystem::exists(filename)) {
                boost::property_tree::ini_parser::read_ini(filename, pt);
              }

              for (variant_t& v : values) {
                boost::apply_visitor(read_visitor(pt, data), v);
              }
            }

            /// List of configuration entries
            std::vector<variant_t> values;
        };

        /**
         * @brief Test struct
         *
         * Dummy structure to test reading integer and string.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        struct Test {
            std::string Str;
            int Int;
        };

        /**
         * @brief Read to struct
         *
         * Demonstrates how to read data from an INI file to a struct.
         *
         * @note This version is extended to support more types and to only have a
         * single vector by using a variant.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        void structuredVariant() {
          std::cout << "##### " << __PRETTY_FUNCTION__ << " #####" << std::endl;

          Test t1;
          t1.Str = "Hello World";
          t1.Int = 3;

          std::cout << "t1.Str=" << t1.Str << " (pre)" << std::endl;
          std::cout << "t1.Int=" << t1.Int << " (pre)" << std::endl;

          Configuration<Test> configuration;
          configuration.add("Str", &Test::Str, std::string("Bye bye"));
          configuration.add("Int", &Test::Int, 7);
          configuration.read("test.ini", t1);

          std::cout << "t1.Str=" << t1.Str << " (post)" << std::endl;
          std::cout << "t1.Int=" << t1.Int << " (post)" << std::endl;
        }
      }
    }
  }
}
