#pragma once

#include "error.hpp"
#include "response.hpp"
#include "request_handler.hpp"

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
              catch(std::exception& e) {
                return error::ErrorCode(static_cast<std::int32_t>(error::ErrorCodes::INTERNAL_ERROR), e.what());
              }
            }, args...)
        {}

        boost::json::object operator()(const boost::json::object& request) {
          TId id;

          handler_failure state = handler(request, id);
          if (state) {
            TRet result = static_cast<TRet>(state);
            return response(id, result);
          }
          else {
            const error::ErrorCode ec = static_cast<error::ErrorCode>(state);
            return error(id, ec);
          }
        }

      protected:
        RequestHandler<TId, TRet, TArgs...> handler;
        Response<TId, TRet> response;
        Error<TId> error;
    };
  }
}
