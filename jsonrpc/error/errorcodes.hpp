#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <experimental/source_location>

#include <boost/format.hpp>
#include <boost/json.hpp>

#include "../util/asjson.hpp"
#include "../util/jsontype.hpp"

namespace ts7 {
  namespace jsonrpc {
    namespace error {
      /**
       * @brief List of errors
       *
       * List of errors that can occur within the library.
       *
       * @note Everything within the library uses std::int32_t, so that it
       * is extendable by applications.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      enum class ErrorCodes : std::int32_t {
        /// Parsing the jsonrpc failed
        PARSE_ERROR = -32700,

        /// The request is invalid, this can contain sub reasons
        INVALID_REQUEST = -32600,

        /// Method is not registered / known
        METHOD_NOT_FOUND = -32601,

        /// Used whenever an invalid parameter was found, can contain sub reasons
        INVALID_PARAMS = -32602,

        /// Any internal error
        INTERNAL_ERROR = -32603,

        /// Unknown error
        UNKNOWN_ERROR = 0,

        /// Field jsonrpc is not present
        JSONRPC_MISSING = 1,

        /// Field jsonrpc is not a string, but exists
        JSONRPC_NOT_A_STRING,

        /// Field jsonrpc contains any value unequal to "2.0"
        JSONRPC_UNKNOWN_SPECIFICATION,

        /// Field method is not present
        METHOD_MISSING,

        /// Field method is not a string, but exists
        METHOD_NOT_A_STRING,

        /// Field id is not present
        ID_MISSING,

        /// Field id has a type that differs from the expected one
        ID_WRONG_TYPE,

        /// Field params is not present
        PARAMS_MISSING,

        /// Field params is not an object, but exists
        PARAMS_NOT_AN_OBJECT,

        /// A parameter is not present
        PARAMETER_MISSING,

        /// A parameter exists, but is has an unexpected type
        PARAMETER_WRONG_TYPE,

        /// A parameter value is missing
        PARAMETER_VALUE_MISSING,

        /// Field error is missing
        ERROR_MISSING,

        /// Field error is not an object
        ERROR_NOT_AN_OBJECT,

        /// Field code within error is missing
        ERROR_CODE_MISSING,

        /// FIeld code within error is not a number
        ERROR_CODE_NOT_A_NUMBER,

        /// Field message within error is missing
        ERROR_MESSAGE_MISSING,

        /// Field message within error is not a string
        ERROR_MESSAGE_NOT_A_STRING,

        /// There was no callback stored in the error handler
        ERROR_CALLBACK_MISSING,

        /// Field result is missing
        RESULT_MISSING,

        /// Field result has the wrong type
        RESULT_WRONG_TYPE,
      };

      /// Conversion from ErrorCode to std::int32_t
      inline constexpr std::int32_t Code(ErrorCodes code) {
        return static_cast<std::int32_t>(code);
      }

      /**
       * @brief Error code
       *
       * Represents everything that is required to create a JSON-RPC error
       * message. This includes the code, the message and any additional data.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      class ErrorCode {
        public:
          /**
           * @brief constructor
           *
           * Default constructor which should never be used. Required to exist
           * for \p util::maybe_failed.
           *
           * @note This should be the only way to create an \p ErrorCodes::UNKNOWN_ERROR.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline ErrorCode()
            : code(static_cast<std::int32_t>(ErrorCodes::UNKNOWN_ERROR)),
              message("Unknown error")
          {}

          /**
           * @brief constructor
           *
           * Creates an error with the provided error code as well as the message.
           *
           * @param code The error code that shall be provided.
           * @param message The corresponding message to the error code.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline ErrorCode(std::int32_t code, std::string&& message)
            : code(code),
              message(std::move(message))
          {}

          /**
           * @brief constructor
           *
           * Creates an error with code, message and a JSON object that
           * shall be provided as data field.
           *
           * @param code The error code.
           * @param message The corresponding message to the error code.
           * @param data Additional data that shall be provided with the error.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline ErrorCode(std::int32_t code, std::string&& message, const boost::json::value& data)
            : code(code),
              message(std::move(message)),
              data(data)
          {}

          /**
           * @brief constructor
           *
           * Creates an error with a code and the corresponding
           * message. Additional data can be added as pairs of key and value.
           *
           * @note The parameter args needs to be a list of two always. The key
           * always needs to be a string and the value can be any type as long
           * as it can be converted to a JSON value by \p util::AsJson.
           *
           * @tparam TArgs List of parameter data types.
           *
           * @param code The error of the error.
           * @param message The corresponding error message.
           * @param args List of keys and values to be stored as additional data.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          template <typename... TArgs>
          inline ErrorCode(std::int32_t code, std::string&& message, TArgs... args)
            : code(code),
              message(std::move(message))
          {
            boost::json::object dataObject;
            apply_args(dataObject, args...);

            data = dataObject;
          }

          void addData(const std::string& field, const boost::json::value& value) {
            if (!data.is_object()) {
              data = boost::json::object();
            }

            boost::json::object& d = data.as_object();
            d[field] = value;
          }

          /**
           * @brief Convert to JSON object
           *
           * Converts the \p ErrorCode object to a JSON object.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline operator boost::json::object() const {
            boost::json::object o;
            o["code"] = code;
            o["message"] = message;

            if (!data.is_null()) {
              o["data"] = data;
            }

            return o;
          }

          /// Error code cast operator
          inline operator std::int32_t() const {
            return code;
          }

          /// Error message cast operator
          inline operator std::string() const {
            return message;
          }

          /**
           * @brief Missing field
           *
           * Factory method to create a missing field \p ErrorCode.
           *
           * @note This adds the fieldnam as additional information to the \p ErrorCode object.
           *
           * @param code The error code that needs to be stored.
           * @param field The name of the field that is missing.
           * @param format The format specifier for the error message.
           *
           * @return Returns the created \p ErrorCode.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          static inline ErrorCode MissingField(std::int32_t code, const std::string& field, const std::string& format = "Missing field \"%1%\"") {
            return ErrorCode(code, (boost::format(format) % field).str(), "name", field);
          }

          /**
           * @brief Wrong type
           *
           * Factory method to create an \p ErrorCode, if a field or
           * parameter has an unexpected data type.
           *
           * @note Adds the fieldname, its actual data type and the expected
           * data type as additional information to the \p ErrorCode.
           *
           * @param code The error code.
           * @param field The name of the field or parameter.
           * @param actual The actual data type of the field.
           * @param expected The expected data type of the field.
           * @param format The format specifier to generate the error message.
           *
           * @return Returns the created \p ErrorCode.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          static inline ErrorCode WrongType(std::int32_t code, const std::string& field, util::JsonType actual, util::JsonType expected, const std::string& format = "Field \"%1%\" is of type \"%2%\", expected \"%3%\"") {
            return ErrorCode(
              code,
              (boost::format(format) % field % GetJsonTypeName(actual) % GetJsonTypeName(expected)).str(),
              "name", field,
              "actual", GetJsonTypeName(actual),
              "expected", GetJsonTypeName(expected)
            );
          }

          /**
           * @brief Missing parameter
           *
           * Creates an \p ErrorCode, whenever a parameter is missing.
           *
           * @note This calls internally \p MissingField factory method.
           *
           * @param code The provided error code.
           * @param field The name of the parameter, that is missing.
           *
           * @return Returns the created \p ErrorCode.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          static inline ErrorCode MissingParameter(std::int32_t code, const std::string& field) {
            return MissingField(code, field, "Missing parameter \"%1%\"");
          }

          /**
           * @brief Wrong parameter type
           *
           * Factory method to create an \p ErrorCode object, whenever an
           * unexpected parameter type is found.
           *
           * @note This adds the fieldname, its actual data type and the
           * expected data type as additional information. This calls
           * internally \p WrongType.
           *
           * @param code The error code provided.
           * @param field The parameter name.
           * @param actual Its actual data type.
           * @param expected The expected data type.
           *
           * @return Returns the created \p ErrorCode object.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          static inline ErrorCode WrongParameterType(std::int32_t code, const std::string& field, util::JsonType actual, util::JsonType expected) {
            return WrongType(code, field, actual, expected, "Parameter \"%1%\" is of type \"%2%\", expected \"%3%\"");
          }

        protected:
          /// The error code
          std::int32_t code;

          /// The error message
          std::string message;

          /// Additional information. Those are optional and can be omitted.
          boost::json::value data;

          /**
           * @brief Apply json entry
           *
           * Adds additional information to the JSON object.
           *
           * @note This function is only called for the very last key value pair.
           *
           * @tparam TData The data type that shall be added.
           *
           * @param o The JSON object that shall be extended by the additional information.
           * @param name The key of the entry.
           * @param value The value that shall be added.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          template <typename TData>
          inline void apply(boost::json::object& o, const std::string& name, const TData& value) {
            o[name] = util::AsJson<TData>(value);
          }

          /**
           * @brief Apply json entry
           *
           * Adds additional information to the JSON object.
           *
           * @note This function is  called for every key value pair, except the very last one.
           *
           * @tparam TData The data type that shall be added.
           * @tparam TArgs The list of key value pairs that shall be added.
           *
           * @param o The JSON object that shall be extended by the additional information.
           * @param name The key of the entry.
           * @param value The value that shall be added.
           * @param agrs The list of remaining key value pairs that shall be added.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          template <typename TData, typename... TArgs>
          inline void apply(boost::json::object& o, const std::string& name, const TData& value, TArgs... args) {
            o[name] = value;
            apply(o, args...);
          }

          /**
           * @brief Apply json entries
           *
           * Adds additional information to the JSON object.
           *
           * @tparam TArgs The list of key value pairs that shall be added.
           *
           * @param o The JSON object that shall be extended by the additional information.
           * @param agrs The list of key value pairs that shall be added.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          template <typename... TArgs>
          inline void apply_args(boost::json::object& o, TArgs... args) {
            apply(o, args...);
          }
      };

      /**
       * @brief Parse error
       *
       * Factory method to create a parse error.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ParseError() {
        return ErrorCode(Code(ErrorCodes::PARSE_ERROR), "Parse error");
      }

      /**
       * @brief Invalid request
       *
       * Factory method to create an invalid request error. This always contains another
       * error that made the request invalid.
       *
       * @param data The \p ErrorCode that triggered the invalid request.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode InvalidRequest(const ErrorCode& data) {
        const boost::json::object o = data;
        return ErrorCode(Code(ErrorCodes::INVALID_REQUEST), "Invalid request", boost::json::value(o));
      }

      /**
       * @brief Method not found
       *
       * Factroy method to create an error, if the requested method is not
       * exposed to the server/client.
       *
       * @param name Name of the method that could not be found.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode MethodNotFound(const std::string& name) {
        return ErrorCode(Code(ErrorCodes::METHOD_NOT_FOUND), (boost::format("Method \"%1%\" not found") % name).str(), "method", name);
      }

      /**
       * @brief Invalid params
       *
       * Factory method to create an error, if at least one of the parameter
       * were invalid for any reason.
       *
       * @param data \p ErrorCode that caused the invalid params error.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode InvalidParams(const ErrorCode& data) {
        const boost::json::object o = data;
        return ErrorCode(Code(ErrorCodes::INVALID_PARAMS), "Invalid parameter", boost::json::value(o));
      }

      /**
       * @brief Internal error
       *
       * Factory method to create any internal error, whenever it has occured.
       *
       * @tparam TArgs List of key value pair types provided for additional information.
       *
       * @param args List of key value pair entries that shall be added as
       * additional information to the \p ErrorCode.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename... TArgs>
      [[maybe_unused]] static inline ErrorCode InternalError(TArgs... args) {
        return ErrorCode(Code(ErrorCodes::INTERNAL_ERROR), "Internal Error", args...);
      }

      /**
       * @brief Jsonrpc missing
       *
       * Factory method to create an error, if the jsonrpc field is missing.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode JsonrpcMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::JSONRPC_MISSING), "jsonrpc");
      }

      /**
       * @brief Jsonrpc not a string
       *
       * Factory method that creates an \p ErrorCode, if the jsonrpc field
       * exists but has an unexpected data type that is not string.
       *
       * @param t The actual data type of the jsonrpc field.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode JsonrpcNotAString(util::JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::JSONRPC_NOT_A_STRING), "jsonrpc", t, util::JsonType::STRING);
      }

      /**
       * @brief Jsonrpc unknown specification
       *
       * Factory method to create an \p ErrorCode, if the jsonrpc field
       * exists and is a string, but has an unexpected value that is
       * not "2.0".
       *
       * @param value The value that got found.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode JsonrpcUnknownSpecification(const std::string& value) {
        return ErrorCode(
          Code(ErrorCodes::JSONRPC_UNKNOWN_SPECIFICATION),
          (boost::format("Unknown JSON-RPC specification \"%s\", exptected \"2.0\"") % value).str()
        );
      }

      /**
       * @brief Method missing
       *
       * Factory method to create an \p ErrorCode, if the method field
       * is missing.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode MethodMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::METHOD_MISSING), "method");
      }

      /**
       * @brief Method not a string
       *
       * Factory method to create an \p ErrorCode, if the the method field
       * exists but is not a string.
       *
       * @param t The actual data type of the method field.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode MethodNotAString(util::JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::METHOD_NOT_A_STRING), "method", t, util::JsonType::STRING);
      }

      /**
       * @brief ID missing
       *
       * Factory method to create an \p ErrorCode, if the id field is missing.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode IdMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::ID_MISSING), "id");
      }

      /**
       * @brief ID wrong type
       *
       * Factory method to create an \p ErrorCode, if the id field
       * exists, but has an unexpected data type.
       *
       * @tparam TId Expected data type of the id field.
       *
       * @param t Actual data type of the found id field.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename TId>
      [[maybe_unused]] static inline ErrorCode IdWrongType(util::JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::ID_WRONG_TYPE), "id", t, util::AsJson<TId>::type);
      }

      /**
       * @brief Params missing
       *
       * Factory method to create an \p ErrorCode, if the the params
       * field is missing.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ParamsMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::PARAMS_MISSING), "params");
      }

      /**
       * @brief Params not an object
       *
       * Factory method to create an \p ErrorCode, if the params object is
       * not an object.
       *
       * @param t The actual data type of the params field.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ParamsNotAnObject(util::JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::PARAMS_NOT_AN_OBJECT), "params", t, util::JsonType::OBJECT);
      }

      /**
       * @brief Parameter missing
       *
       * Factory method to create an \p ErrorCode, if at least one parameter
       * is mising.
       *
       * @param name The name of the parameter that is missing.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ParameterMissing(const std::string& name) {
        return ErrorCode::MissingParameter(Code(ErrorCodes::PARAMETER_MISSING), name);
      }

      /**
       * @brief Parameter wrong type
       *
       * Factory method to create an \p ErrorCode, if the desired parameter
       * exists, but has an unexpected data type.
       *
       * @param name The name of the parameter.
       * @param actual The parameters actual data type.
       * @param expected The expected data type of the parameter.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ParameterWrongType(const std::string& name, util::JsonType actual, util::JsonType expected) {
        return ErrorCode::WrongParameterType(Code(ErrorCodes::PARAMETER_WRONG_TYPE), name, actual, expected);
      }

      /**
       * @brief Parameter value missing
       *
       * Factory method to create an \p ErrorCode, if the there was no value
       * and no default value provided for a parameter.
       *
       * @param name The name of the parameter.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ParameterValueuMissing(const std::string& name) {
        return ErrorCode(Code(ErrorCodes::PARAMETER_VALUE_MISSING), (boost::format("No value or default value provided for parameter \"%1%\"") % name).str());
      }

      /**
       * @brief Error missing
       *
       * Factory method to create an \p ErrorCode, if the error field is missing.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ErrorMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::ERROR_MISSING), "error");
      }

      /**
       * @brief Error not an object
       *
       * Factory method to create an \p ErrorCode, if the the error field
       * exists but is not an object.
       *
       * @param t The actual data type of the error field.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ErrorNotAnObject(util::JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::ERROR_NOT_AN_OBJECT), "error", t, util::JsonType::OBJECT);
      }

      /**
       * @brief Error code missing
       *
       * Factory method to create an \p ErrorCode, if the code field within
       * the error object does not exist.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ErrorCodeMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::ERROR_CODE_MISSING), "code");
      }

      /**
       * @brief Error code not a number
       *
       * Factory method to create an \p ErrorCode, if the code field
       * within the error object is not a number.
       *
       * @param t The actual data type of the error code.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ErrorCodeNotANumber(util::JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::ERROR_CODE_NOT_A_NUMBER), "code", t, util::JsonType::NUMBER);
      }

      /**
       * @brief Error message missing
       *
       * Factory method to create an \p ErrorCode, if the message field wihin
       * the error object is missing.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ErrorMessageMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::ERROR_MESSAGE_MISSING), "message");
      }

      /**
       * @brief Error message not a string
       *
       * Factory method to create an \p ErrorCode, if the message field
       * within the error object exists but is not a string.
       *
       * @param t The actual data type of the message field.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ErrorMessageNotAString(util::JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::ERROR_MESSAGE_NOT_A_STRING), "message", t, util::JsonType::STRING);
      }

      /**
       * @brief Error callback missing
       *
       * Factory method to create an \p ErrorCode, if there is no callback
       * provided for the error handler.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ErrorCallbackMissing() {
        return ErrorCode(Code(ErrorCodes::ERROR_CALLBACK_MISSING), "Error callback missing");
      }

      /**
       * @brief Result missing
       *
       * Factory method to create an \p ErrorCode, if the result field
       * is missing.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ResultMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::RESULT_MISSING), "result");
      }

      /**
       * @brief Result wrong type
       *
       * Factory method to create an \p ErrorCode, if the result field
       * exists but has an unexpected data type.
       *
       * @param actual The actual data type of the result field.
       * @param expected The expected data type of the result field.
       *
       * @return Returns the created \p ErrorCode.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      [[maybe_unused]] static inline ErrorCode ResultWrongType(util::JsonType actual, util::JsonType expected = util::JsonType::OBJECT) {
        return ErrorCode::WrongType(Code(ErrorCodes::RESULT_WRONG_TYPE), "result", actual, expected);
      }

      struct Exception : public std::runtime_error {
        using source_location = std::experimental::source_location;

        inline Exception(std::int32_t code, std::string&& message, const source_location& location = source_location::current())
          : std::runtime_error(message),
            ec(code, std::move(message), "location", location)
        {}

        inline Exception(std::int32_t code, std::string&& message, const boost::json::value& data, const source_location& location = source_location::current())
          : std::runtime_error(message),
            ec(code, std::move(message), data)
        {
          ec.addData("location", util::AsJson<source_location>(location));
        }

        template <typename... TArgs>
        inline Exception(std::int32_t code, std::string&& message, const source_location& location, TArgs... args)
          : std::runtime_error(message),
            ec(code, std::move(message), "location", location, args...)
        {}

        template <typename... TArgs>
        inline Exception(std::int32_t code, std::string&& message, TArgs... args)
          : std::runtime_error(message),
            ec(code, std::move(message), args...)
        {}

        inline operator boost::json::object() const {
          return ec;
        }

        ErrorCode ec;
      };
    }
  }
}
