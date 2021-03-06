#pragma once

#include "error.hpp"
#include "response.hpp"
#include "request_handler.hpp"
#include "notification_handler.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId, typename TRet, typename... TArgs>
    class Procedure {
      public:
        using callback_t = std::function<TRet(TArgs...)>;
        using handler_failure = typename RequestHandler<TId, TRet, TArgs...>::maybe_failed;

        template <typename... UArgs>
        Procedure(callback_t callback, UArgs... args)
          : handler([callback](const TId&, TArgs... args) -> handler_failure {
              try {
                return callback(args...);
              }
              catch(error::Exception& e) {
                return e.ec;
              }
              catch(std::exception& e) {
                return error::ErrorCode(static_cast<std::int32_t>(error::ErrorCodes::INTERNAL_ERROR), e.what());
              }
            }, args...)
        {}

        boost::json::value operator()(const boost::json::object& request) {
          TId id;

          handler_failure state = handler(request, id);
          if (state) {
            TRet result = state.getSuccess();
            return response(id, result);
          }
          else {
            const error::ErrorCode ec = state.getFailed();
            return error(id, ec);
          }
        }

      protected:
        RequestHandler<TId, TRet, TArgs...> handler;
        Response<TId, TRet> response;
        Error<TId> error;
    };

    template <typename TId, typename... TArgs>
    class Procedure<TId, void, TArgs...> {
      public:
        using callback_t = std::function<void(TArgs...)>;
        using handler_failure = typename RequestHandler<TId, void, TArgs...>::maybe_failed;

        template <typename... UArgs>
        Procedure(callback_t callback, UArgs... args)
          : handler([callback](const TId&, TArgs... args) -> handler_failure {
              try {
                callback(args...);
                return handler_failure();
              }
              catch(error::Exception& e) {
                return e.ec;
              }
              catch(std::exception& e) {
                return error::ErrorCode(static_cast<std::int32_t>(error::ErrorCodes::INTERNAL_ERROR), e.what());
              }
            }, args...)
        {}

        boost::json::value operator()(const boost::json::object& request) {
          TId id;

          handler_failure state = handler(request, id);
          if (state) {
            return response(id);
          }
          else {
            const error::ErrorCode ec = state.getFailed();
            return error(id, ec);
          }
        }

      protected:
        RequestHandler<TId, void, TArgs...> handler;
        Response<TId, void> response;
        Error<TId> error;
    };

    template <typename... TArgs>
    class NotificationProcedure {
      public:
        using callback_t = std::function<void(TArgs...)>;
        using handler_failure = typename NotificationHandler<TArgs...>::maybe_failed;

        template <typename... UArgs>
        NotificationProcedure(callback_t callback, UArgs... args)
          : handler([callback](TArgs... args) -> handler_failure {
              callback(args...);
              return handler_failure();
            }, Parameter<TArgs>(args)...)
        {}

        boost::json::value operator()(const boost::json::object& notification) {
          handler_failure succeeded = handler(notification);
          if ( !succeeded ) {
            const error::ErrorCode& ec = succeeded.getFailed();
            throw error::Exception(static_cast<std::int32_t>(ec), static_cast<std::string>(ec));
          }

          return boost::json::value();
        }

      protected:
        NotificationHandler<TArgs...> handler;
    };
  }
}
