#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#include <boost/format.hpp>
#include <boost/json.hpp>

#include "../util/asjson.hpp"
#include "../util/jsontype.hpp"

namespace ts7 {
  namespace jsonrpc {
    namespace error {
      enum class ErrorCodes : std::int32_t {
        PARSE_ERROR = -32700,
        INVALID_REQUEST = -32600,
        METHOD_NOT_FOUND = -32601,
        INVALID_PARAMS = -32602,
        INTERNAL_ERROR = -32603,
        UNKNOWN_ERROR = 0,
        JSONRPC_MISSING = 1,
        JSONRPC_NOT_A_STRING,
        JSONRPC_UNKNOWN_SPECIFICATION,
        METHOD_MISSING,
        METHOD_NOT_A_STRING,
        ID_MISSING,
        ID_WRONG_TYPE,
        PARAMS_MISSING,
        PARAMS_NOT_AN_OBJECT,
        PARAMETER_MISSING,
        PARAMETER_WRONG_TYPE,
        PARAMETER_VALUE_MISSING,
        ERROR_MISSING,
        ERROR_NOT_AN_OBJECT,
        ERROR_CODE_MISSING,
        ERROR_CODE_NOT_A_NUMBER,
        ERROR_MESSAGE_MISSING,
        ERROR_MESSAGE_NOT_A_STRING,
        ERROR_CALLBACK_MISSING,
        RESULT_MISSING,
        RESULT_WRONG_TYPE,
      };

      constexpr std::int32_t Code(ErrorCodes code) {
        return static_cast<std::int32_t>(code);
      }

      class ErrorCode {
        public:
          inline ErrorCode()
            : code(static_cast<std::int32_t>(ErrorCodes::UNKNOWN_ERROR)),
              message("Unknown error")
          {}

          inline ErrorCode(std::int32_t code, std::string&& message)
            : code(code),
              message(std::move(message))
          {}

          inline ErrorCode(std::int32_t code, std::string&& message, const boost::json::value& data)
            : code(code),
              message(std::move(message)),
              data(data)
          {}

          template <typename... TArgs>
          inline ErrorCode(std::int32_t code, std::string&& message, TArgs... args)
            : code(code),
              message(std::move(message))
          {
            boost::json::object dataObject;
            apply_args(dataObject, args...);

            data = dataObject;
          }

          operator boost::json::value() const {
            return static_cast<boost::json::object>(*this);
          }

          operator boost::json::object() const {
            boost::json::object o;
            o["code"] = code;
            o["message"] = message;

            if (!data.is_null()) {
              o["data"] = data;
            }

            return o;
          }

          operator std::int32_t() const {
            return code;
          }

          operator std::string() const {
            return message;
          }

          static inline ErrorCode MissingField(std::int32_t code, const std::string& field, const std::string& format = "Missing field \"%1%\"") {
            return ErrorCode(code, (boost::format(format) % field).str(), "name", field);
          }

          static inline ErrorCode WrongType(std::int32_t code, const std::string& field, JsonType actual, JsonType expected, const std::string& format = "Field \"%1%\" is of type \"%2%\", expected \"%3%\"") {
            return ErrorCode(
              code,
              (boost::format(format) % field % GetJsonTypeName(actual) % GetJsonTypeName(expected)).str(),
              "name", field,
              "actual", GetJsonTypeName(actual),
              "expected", GetJsonTypeName(expected)
            );
          }

          static inline ErrorCode MissingParameter(std::int32_t code, const std::string& field) {
            return MissingField(code, field, "Missing parameter \"%1%\"");
          }

          static inline ErrorCode WrongParameterType(std::int32_t code, const std::string& field, JsonType actual, JsonType expected) {
            return WrongType(code, field, actual, expected, "Parameter \"%1%\" is of type \"%2%\", expected \"%3%\"");
          }

        protected:
          std::int32_t code;
          std::string message;
          boost::json::value data;

          template <typename TData>
          void apply(boost::json::object& o, const std::string& name, const TData& value) {
            o[name] = value;
          }

          template <typename TData, typename... TArgs>
          void apply(boost::json::object& o, const std::string& name, const TData& value, TArgs... args) {
            o[name] = value;
            apply(o, args...);
          }

          template <typename... TArgs>
          void apply_args(boost::json::object& o, TArgs... args) {
            apply(o, args...);
          }
      };

      [[maybe_unused]] static inline ErrorCode ParseError() {
        return ErrorCode(Code(ErrorCodes::PARSE_ERROR), "Parse error");
      }

      [[maybe_unused]] static inline ErrorCode InvalidRequest(const ErrorCode& data) {
        return ErrorCode(Code(ErrorCodes::INVALID_REQUEST), "Invalid request", static_cast<boost::json::value>(data));
      }

      [[maybe_unused]] static inline ErrorCode MethodNotFound(const std::string& name) {
        return ErrorCode(Code(ErrorCodes::METHOD_NOT_FOUND), (boost::format("Method \"%1%\" not found") % name).str(), "method", name);
      }

      [[maybe_unused]] static inline ErrorCode InvalidParams(const ErrorCode& data) {
        return ErrorCode(Code(ErrorCodes::INVALID_PARAMS), "Invalid parameter", static_cast<boost::json::value>(data));
      }

      template <typename... TArgs>
      [[maybe_unused]] static inline ErrorCode InternalError(TArgs... args) {
        return ErrorCode(Code(ErrorCodes::INTERNAL_ERROR), "Internal Error", args...);
      }

      [[maybe_unused]] static inline ErrorCode JsonrpcMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::JSONRPC_MISSING), "jsonrpc");
      }

      [[maybe_unused]] static inline ErrorCode JsonrpcNotAString(JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::JSONRPC_NOT_A_STRING), "jsonrpc", t, JsonType::STRING);
      }

      [[maybe_unused]] static inline ErrorCode JsonrpcUnknownSpecification(const std::string& value) {
        return ErrorCode(
          Code(ErrorCodes::JSONRPC_UNKNOWN_SPECIFICATION),
          (boost::format("Unknown JSON-RPC specification \"%s\", exptected \"2.0\"") % value).str()
        );
      }

      [[maybe_unused]] static inline ErrorCode MethodMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::METHOD_MISSING), "method");
      }

      [[maybe_unused]] static inline ErrorCode MethodNotAString(JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::METHOD_NOT_A_STRING), "method", t, JsonType::STRING);
      }

      [[maybe_unused]] static inline ErrorCode IdMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::ID_MISSING), "id");
      }

      template <typename TId>
      [[maybe_unused]] static inline ErrorCode IdWrongType(JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::ID_WRONG_TYPE), "id", t, AsJson<TId>::type);
      }

      [[maybe_unused]] static inline ErrorCode ParamsMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::PARAMS_MISSING), "params");
      }

      [[maybe_unused]] static inline ErrorCode ParamsNotAnObject(JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::PARAMS_NOT_AN_OBJECT), "params", t, JsonType::OBJECT);
      }

      [[maybe_unused]] static inline ErrorCode ParameterMissing(const std::string& name) {
        return ErrorCode::MissingParameter(Code(ErrorCodes::PARAMETER_MISSING), name);
      }

      [[maybe_unused]] static inline ErrorCode ParameterWrongType(const std::string& name, JsonType actual, JsonType expected) {
        return ErrorCode::WrongParameterType(Code(ErrorCodes::PARAMETER_WRONG_TYPE), name, actual, expected);
      }

      [[maybe_unused]] static inline ErrorCode ParameterValueMissing(const std::string& name) {
        return ErrorCode(Code(ErrorCodes::PARAMETER_VALUE_MISSING), (boost::format("No value or default value provided for parameter \"%1%\"") % name).str());
      }

      [[maybe_unused]] static inline ErrorCode ErrorMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::ERROR_MISSING), "error");
      }

      [[maybe_unused]] static inline ErrorCode ErrorNotAnObject(JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::ERROR_NOT_AN_OBJECT), "error", t, JsonType::OBJECT);
      }

      [[maybe_unused]] static inline ErrorCode ErrorCodeMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::ERROR_CODE_MISSING), "code");
      }

      [[maybe_unused]] static inline ErrorCode ErrorCodeNotANumber(JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::ERROR_CODE_NOT_A_NUMBER), "code", t, JsonType::NUMBER);
      }

      [[maybe_unused]] static inline ErrorCode ErrorMessageMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::ERROR_MESSAGE_MISSING), "message");
      }

      [[maybe_unused]] static inline ErrorCode ErrorMessageNotAString(JsonType t) {
        return ErrorCode::WrongType(Code(ErrorCodes::ERROR_MESSAGE_NOT_A_STRING), "message", t, JsonType::STRING);
      }

      [[maybe_unused]] static inline ErrorCode ErrorCallbackMissing() {
        return ErrorCode(Code(ErrorCodes::ERROR_CALLBACK_MISSING), "Error callback missing");
      }

      [[maybe_unused]] static inline ErrorCode ResultMissing() {
        return ErrorCode::MissingField(Code(ErrorCodes::RESULT_MISSING), "result");
      }

      [[maybe_unused]] static inline ErrorCode ResultWrongType(JsonType actual, JsonType expected = JsonType::OBJECT) {
        return ErrorCode::WrongType(Code(ErrorCodes::RESULT_WRONG_TYPE), "result", actual, expected);
      }
    }
  }
}
