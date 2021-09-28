#pragma once

#include <string>

#include "request.hpp"
#include "error/error.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    /**
     * @brief Error message
     *
     * Represents an error message generator.
     *
     * @note According to the specification an error does not contain the
     * method name. If the method name is wished, it can be enabled with
     * the define TS7_JSONRPC_SUPPORT_RESPONSE_METHOD_NAME.
     *
     * @tparam TId Data type of the id field.
     *
     * @since 1.0
     *
     * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
     */
    template <typename TId>
    class Error {
      public:
        /// default constructor
        inline Error() = default;

#ifndef TS7_JSONRPC_SUPPORT_RESPONSE_METHOD_NAME
        /**
         * @brief JSON object generation
         *
         * Generates the error JSON object by the provided \p id and error \p code.
         *
         * @param id The id of the request, that has failed and requires an error response.
         * @param code The error code that shall be responded.
         *
         * @return Returns the final error message JSON object.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        inline boost::json::object operator()(const TId& id, const error::ErrorCode& code) const {
          boost::json::object o;
          o["jsonrpc"] = "2.0";
          o["id"] = util::AsJson<TId>(id);
          const boost::json::object e = code;
          o["error"] = e;

          return o;
        }
#else
        /**
         * @brief JSON object generation
         *
         * Generates the error JSON object by the provided \p id and error \p code.
         *
         * @param id The id of the request, that has failed and requires an error response.
         * @param method The name of the method that failed.
         * @param code The error code that shall be responded.
         *
         * @return Returns the final error message JSON object.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        inline boost::json::object operator()(const TId& id, const std::string& method, const error::ErrorCode& code) const {
          boost::json::object o;
          o["jsonrpc"] = "2.0";
          o["method"] = method;
          o["id"] = util::AsJson<TId>(id);
          o["error"] = static_cast<boost::json::object>(code);

          return o;
        }
#endif
    };
  }
}
