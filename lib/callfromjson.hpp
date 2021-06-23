#pragma once

#include <functional>
#include <string>

#include "fromjson.hpp"

namespace ts7 {
  namespace jsonrpc {
    /**
     * @brief Call from json object
     *
     * This class provides a call operator that takes a json object which contains the required parameter to execute
     * the provided callback. The result of the callback will then be returned as result of the whole call.
     *
     * @tparam TReturn Return type of the call back.
     * @tparam T1 Type of the first parameter.
     * @tparam T2 Type of the second parameter.
     *
     * @todo This class needs to support a variadic template, instead of \ref T1 and \ref T2.
     *
     * @since 1.0
     *
     * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
     */
    template <typename TReturn, typename T1, typename T2>
    struct CallFromJson {
        using return_t = TReturn;

        /// Callback type
        using callback_t = std::function<TReturn(T1, T2)>;

        /**
         * @brief Parameter
         *
         * Stores the type and name of a required parameter. In addition this class can store, if the the parameter
         * has a default value that shall be used, if the parameter is missing in the json object.
         *
         * @tparam U Type of the parameter.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename U>
        struct Parameter {
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
            constexpr inline explicit Parameter(const std::string& name, bool hasDefault = false, const U& defaultValue = remove_cref<U>())
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
            remove_cref<U> operator()(const boost::json::object& o) {
              FromJson<remove_cref<U>> v;
              if (o.contains(name)) {
                // json object contains the parameter
                return v(o.at(name));
              }

              if (hasDefault) {
                // json object does not contain the parameter, but a default value was defined
                return defaultValue;
              }

              // This should throw an exception in theory
              static remove_cref<U> tmp;
              return tmp;
            }

            /// Parameter name
            std::string name;
            /// Is default value allowed
            bool hasDefault;
            /// Default value, that shall be used if \ref hasDefault is true
            U defaultValue;
        };

        /**
         * @brief Optional parameter
         *
         * Helper class to provide an optional parameter.
         *
         * @note This will be created and casted to Parameter at compile time. So this should not waste any
         * runtime at all.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template<typename U>
        struct OptionalParameter {
          /**
           * @brief constructor
           *
           * Creates a optional parameter with it's name and a default value.
           *
           * @param name The parameter name.
           * @param defaultValue Default value, if the parameter is not present in the json object.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          constexpr inline explicit OptionalParameter(const std::string& name, const U& defaultValue = U())
            : name(name),
              defaultValue(defaultValue)
          {}

          /**
           * @brief Cast operator
           *
           * Casts the optional parameter to a parameter.
           *
           * @note This happens at compile time, if possible.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          constexpr operator Parameter<U>() const {
            return Parameter<U>(name, true, defaultValue);
          }

          /// Parameter name
          std::string name;

          /// Default value
          U defaultValue;
        };

        /**
         * @brief constructor
         *
         * Creates the test with the provided parameter.
         *
         * @tparam U1 Type info to create the first parameter. This can be a string (name), a \ref Parameter or an
         * \ref OptionalParameter.
         * @tparam U2 Type info to create the second parameter, which behaves like \p U1.
         *
         * @todo This should use a variadic template to support multiple amounts of arguments.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename U1, typename U2>
        inline explicit CallFromJson(callback_t fn, const U1& p1, const U2& p2)
          : fn(fn),
            p1(p1),
            p2(p2)
        {}

        /**
         * @brief Call operator
         *
         * Enables call operations on instances of this class. This will require a json object that internally
         * gets converted to the corresponding values from the json object for the parameters. Afterwards the
         * stored callback is executed with the retrieved parameter values.
         *
         * @param o The json object that provides the parameter.
         *
         * @return Returns whatever the callback returned.
         *
         * @todo This should use the variadic template of the class to determine which and how many parameter are
         * required.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@ŋooglemail.com>
         */
        TReturn operator()(const boost::json::object& o) {
          remove_cref<T1> t1 = p1(o);
          remove_cref<T2> t2 = p2(o);
          return fn(t1, t2);
        }

        /// Provided callback
        callback_t fn;

        /// @todo Create parameter based on the variadic class template.

        /// First parameter
        Parameter<T1> p1;

        /// Second parameter
        Parameter<T2> p2;
    };
  }
}
