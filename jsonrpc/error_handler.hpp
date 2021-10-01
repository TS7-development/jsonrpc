#pragma once

#include <functional>
#include <string>

#include "request.hpp"
#include "error/error.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    /**
     * @brief Error handler
     *
     * Represents a class that handles received error repsonses on requests.
     *
     * @note Check the JSON-RPC specification for more detailed information
     * on the error responses.
     *
     * @tparam TId Data type of the id field.
     * @tparam TData Data type of the data field, that contains additional information.
     *
     * @since 1.0
     *
     * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
     */
    template <typename TId, typename TData>
    class ErrorHandler {
      public:
        /// Error callback data type
        using callback_t = std::function<void(const TId&, std::int32_t, const std::string&, const TData&)>;
        /// Maybe failed return type (contains only error information)
        using maybe_failed = error::maybe_failed<void, error::ErrorCode>;
        /// Maybe failed for the specification (jsonrpc field)
        using spec_failure = error::maybe_failed<std::string, error::ErrorCode>;
        /// Maybe failed for the id (id field)
        using id_failure = error::maybe_failed<TId, error::ErrorCode>;

        /**
         * @brief constructor
         *
         * Creates an error handler by a provided callback.
         *
         * @param callback
         */
        constexpr explicit inline ErrorHandler(callback_t callback)
          : callback(callback)
        {}

        maybe_failed operator()(const boost::json::object& e) {
          spec_failure jsonrpc = checkJsonrpc(e);
          if (!jsonrpc) {
            return static_cast<error::ErrorCode>(jsonrpc);
          }

          id_failure id = checkJsonrpc(e);
          if (!id) {
            return static_cast<error::ErrorCode>(id);
          }

          if (!e.contains("error")) {
            return error::ErrorMissing();
          }

          const boost::json::value& error = e.at("error");
          if (!error.is_object()) {
            return error::ErrorNotAnObject(util::GetJsonType(error));
          }

          const boost::json::object& errorObj = error.as_object();
          if (!errorObj.contains("code")) {
            return error::ErrorCodeMissing();
          }

          const boost::json::value& errorCode = errorObj.at("code");
          if (!errorCode.is_number()) {
            return error::ErrorCodeNotANumber(util::GetJsonType(errorCode));
          }

          if (!errorObj.contains("message")) {
            return error::ErrorMessageMissing();
          }

          const boost::json::value& errorMessage = errorObj.at("message");
          if (!errorMessage.is_string()) {
            return error::ErrorMessageNotAString(util::GetJsonType(errorMessage));
          }

          if (!callback) {
            return error::ErrorCallbackMissing();
          }

          util::FromJson<TId> id_value;
          util::FromJson<std::int32_t> code;
          util::FromJson<std::string> message;
          util::FromJson<TData> data;

          callback(id_value(id), code(errorCode), message(errorMessage), data(errorObj.at("data")));
        }

      protected:
         spec_failure checkJsonrpc(const boost::json::object& e) {
          if (!e.contains("jsonrpc")) {
            return error::JsonrpcMissing();
          }

          const boost::json::value& jsonrpc = e.at("jsonrpc");
          if (!jsonrpc.is_string()) {
            return error::JsonrpcNotAString(util::GetJsonType(jsonrpc));
          }

          const std::string value = jsonrpc.as_string().c_str();
          if ("2.0" != value) {
            return error::JsonrpcUnknownSpecification(value);
          }

          return value;
        }

        id_failure checkId(const boost::json::object& e) {
          if (!e.contains("id")) {
            return error::IdMissing();
          }

          const boost::json::value& id = e.at("id");
          util::JsonType actual = util::GetJsonType(id);
          if (util::AsJson<TId>::IsType(actual)) {
            return error::IdWrongType<TId>(actual);
          }

          util::FromJson<TId> value;
          return value(id);
        }

        callback_t callback;
    };

    /**
     * @brief Error handler
     *
     * Represents a class that handles received error repsonses on requests.
     *
     * @note Check the JSON-RPC specification for more detailed information
     * on the error responses.
     *
     * @tparam TId Data type of the id field.
     * @tparam TData Data type of the data field, that contains additional information.
     *
     * @since 1.0
     *
     * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
     */
    template <typename TId>
    class ErrorHandler<TId, void> {
      public:
        /// Error callback data type
        using callback_t = std::function<void(const TId&, std::int32_t, const std::string&)>;
        /// Maybe failed return type (contains only error information)
        using maybe_failed = error::maybe_failed<void, error::ErrorCode>;
        /// Maybe failed for the specification (jsonrpc field)
        using spec_failure = error::maybe_failed<std::string, error::ErrorCode>;
        /// Maybe failed for the id (id field)
        using id_failure = error::maybe_failed<TId, error::ErrorCode>;

        /**
         * @brief constructor
         *
         * Creates an error handler by a provided callback.
         *
         * @param callback
         */
        constexpr explicit inline ErrorHandler(callback_t callback)
          : callback(callback)
        {}

        maybe_failed operator()(const boost::json::object& e) {
          spec_failure jsonrpc = checkJsonrpc(e);
          if (!jsonrpc) {
            return static_cast<error::ErrorCode>(jsonrpc);
          }

          id_failure id = checkId(e);
          if (!id) {
            return static_cast<error::ErrorCode>(id);
          }

          if (!e.contains("error")) {
            return error::ErrorMissing();
          }

          const boost::json::value& error = e.at("error");
          if (!error.is_object()) {
            return error::ErrorNotAnObject(util::GetJsonType(error));
          }

          const boost::json::object& errorObj = error.as_object();
          if (!errorObj.contains("code")) {
            return error::ErrorCodeMissing();
          }

          const boost::json::value& errorCode = errorObj.at("code");
          if (!errorCode.is_number()) {
            return error::ErrorCodeNotANumber(util::GetJsonType(errorCode));
          }

          if (!errorObj.contains("message")) {
            return error::ErrorMessageMissing();
          }

          const boost::json::value& errorMessage = errorObj.at("message");
          if (!errorMessage.is_string()) {
            return error::ErrorMessageNotAString(util::GetJsonType(errorMessage));
          }

          if (!callback) {
            return error::ErrorCallbackMissing();
          }

          util::FromJson<std::int32_t> code;
          util::FromJson<std::string> message;

          callback(static_cast<TId>(id), code(errorCode), message(errorMessage));
          return maybe_failed();
        }

      protected:
         spec_failure checkJsonrpc(const boost::json::object& e) {
          if (!e.contains("jsonrpc")) {
            return error::JsonrpcMissing();
          }

          const boost::json::value& jsonrpc = e.at("jsonrpc");
          if (!jsonrpc.is_string()) {
            return error::JsonrpcNotAString(util::GetJsonType(jsonrpc));
          }

          const std::string value = jsonrpc.as_string().c_str();
          if ("2.0" != value) {
            return error::JsonrpcUnknownSpecification(value);
          }

          return value;
        }

        id_failure checkId(const boost::json::object& e) {
          if (!e.contains("id")) {
            return error::IdMissing();
          }

          const boost::json::value& id = e.at("id");
          util::JsonType actual = util::GetJsonType(id);
          if (util::AsJson<TId>::IsType(actual)) {
            return error::IdWrongType<TId>(actual);
          }

          util::FromJson<TId> value;
          return static_cast<TId>(value(id));
        }

        callback_t callback;
    };
  }
}
