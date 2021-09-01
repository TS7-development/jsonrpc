#pragma once

#include <optional>
#include <string>

#include "error/error.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename U>
    struct Parameter {
        using datatype_t = util::remove_cref<U>;
        using maybe_failed = error::maybe_failed<datatype_t, error::ErrorCode>;
        /**
         * @brief constructor
         *
         * Creates a parameter with its type, name and the information if the parameter has a default value and what it is.
         *
         * @note Perhaps it would be a good idea to use std::optional for the default value.
         *
         * @param name Name of the parameter.
         * @param hasDefault If the true, the parameter has a default value. The parameter does not have a
         * default value, if it is false.
         * @param defaultValue The provided default value.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        constexpr inline explicit Parameter(const std::string& name, bool hasDefault = false, const U& defaultValue = util::remove_cref<U>())
          : name(name),
            hasDefault(hasDefault),
            defaultValue(defaultValue)
        {}

        /**
         * @brief Call operator
         *
         * Retrieves the required parameter from the json object, if the json object contains it. Otherwise
         * it is returning the default value, if the parameter has a default value.
         *
         * @todo If the object does not contain the parameter and has no default value, there should be
         * an error handling any how.
         *
         * @param o Json object that contains the provided parameter.
         *
         * @return Returns the retrieved value, if the parameter was present in the json object. If not the
         * default value will be returned, if available. Otherwise it will for now just return the default
         * constructor.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        maybe_failed load(const boost::json::object& o) const {
          util::FromJson<datatype_t> v;
          if (o.contains(name)) {
            // json object contains the parameter
            error::maybe_failed<datatype_t, util::JsonType> value = v(o.at(name));
            if (value) {
              // Succeeded: Paramater has correct type
              return static_cast<datatype_t>(value);
            }

            return error::ParameterWrongType(name, static_cast<util::JsonType>(value), util::AsJson<datatype_t>::type);
          }

          if (hasDefault) {
            // json object does not contain the parameter, but a default value was defined
            return defaultValue;
          }

          return maybe_failed(error::ParameterMissing(name));
        }

        maybe_failed store(boost::json::object& o, datatype_t value) const {
          o[name] = util::AsJson<datatype_t>(value);
          return value;

//          if (hasDefault) {
//            o[name] = util::AsJson<datatype_t>(defaultValue);
//            return maybe_failed(defaultValue);
//          }

//          return maybe_failed(util::ParameterValueMissing(name));
        }

        static constexpr inline Parameter Optional(const std::string& name, const util::remove_cref<U>& defaultValue) {
          return Parameter(name, true, defaultValue);
        }

        /// Parameter name
        std::string name;
        /// Is default value allowed
        bool hasDefault;
        /// Default value, that shall be used if \ref hasDefault is true
        datatype_t defaultValue;
    };
  }
}
