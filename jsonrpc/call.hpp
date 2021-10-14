#pragma once

#include <condition_variable>
#include <future>
#include <mutex>

#include <boost/json.hpp>
#include <boost/log/trivial.hpp>

#include "notification.hpp"
#include "request.hpp"
#include "response_handler.hpp"
#include "error_handler.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId>
    class CallLog {
      public:
        struct AwaitResponse {
            inline explicit AwaitResponse(TId id)
              : id(id)
            {}

            TId id;
            bool available = false;
            boost::json::object response;
            std::condition_variable cv;
            std::mutex m;

            inline void wait() {
              std::unique_lock<std::mutex> lock(m);
              cv.wait(lock, [this]() { return available; });
            }

            inline void release(const boost::json::object& o) {
              std::unique_lock<std::mutex> lock(m);
              available = true;
              response = o;
              cv.notify_all();
            }
        };

        static boost::json::object Wait(const boost::json::object& request) {
          std::shared_ptr<AwaitResponse> condition = GetWaitRequest(request);
          if ( !condition ) {
            /// @todo What to do in this case? Should never happen
            return boost::json::object();
          }

          condition->wait();
          calls.erase(condition->id);
          return condition->response;
        }

        static void Release(const boost::json::object& response) {
          ts7::jsonrpc::util::FromJson<TId> conv;
          TId id = conv(response.at("id"));

          while( calls.find(id) == calls.end() ) {
            // Wait for the call to be added
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
          }

          std::shared_ptr<AwaitResponse> ptr = calls[id];
          ptr->release(response);
        }

      protected:
        static std::shared_ptr<AwaitResponse> GetWaitRequest(const boost::json::object& request) {
          ts7::jsonrpc::util::FromJson<TId> conv;
          TId id = conv(request.at("id"));

          if ( calls.find(id) == calls.end()) {
            calls[id] = std::shared_ptr<AwaitResponse>(new AwaitResponse{id});
          }

          return calls[id];
        }

        static inline std::string PointerAddress(const void* p) {
          std::stringstream ss;
          ss << p;
          return ss.str();
        }

        static std::map<TId, std::shared_ptr<AwaitResponse>> calls;
    };

    template <typename TId>
    std::map<TId, std::shared_ptr<typename CallLog<TId>::AwaitResponse>> CallLog<TId>::calls = std::map<TId, std::shared_ptr<AwaitResponse>>();

    template <typename TId, typename TRet, typename TErrorData, typename... TArgs>
    class Call {
      public:
        using request_t = Request<TId, TArgs...>;
        using request_action_t = std::function<void(const boost::json::object&)>;

        using response_t = ResponseHandler<typename TId::type, TRet>;
        using response_action_t = typename ResponseHandler<typename TId::type, TRet>::callback_t;

        using error_t = ErrorHandler<typename TId::type, TErrorData>;
        using error_action_t = typename ErrorHandler<typename TId::type, TErrorData>::callback_t;

        template <typename... UArgs>
        Call(const std::string& method, request_action_t request_action, response_action_t response_action, error_action_t error_action, UArgs... args)
          : request(method, args...),
            request_action(request_action),
            responseHandler(response_action),
            errorHandler(error_action)
        {}

        std::future<void> operator()(TArgs... args) {
          std::future<void> f = std::async(std::launch::async, [this, args...]() {
            // Generate and transmit request
            boost::json::object requestObject = request(args...);
            if ( request_action ) {
              request_action(requestObject);
            }

            // Block until a response is received
            boost::json::object responseObject = CallLog<typename TId::type>::Wait(requestObject);

            if ( responseObject.contains("result") ) {
              typename response_t::maybe_failed succeeded = responseHandler(responseObject);
              if ( !succeeded ) {
                BOOST_LOG_TRIVIAL(error) << "Response handling failed: " << static_cast<std::string>(succeeded.getFailed());
              }
            }
            else if ( responseObject.contains("error") ) {
              typename error_t::maybe_failed succeeded = errorHandler(responseObject);
              if ( !succeeded ) {
                BOOST_LOG_TRIVIAL(error) << "Error handling failed: " << static_cast<std::string>(succeeded.getFailed());

              }
            }
            else {
              throw std::runtime_error("Unknown response type");
            }
          });

          return f;
        }

      protected:
        request_t request;
        request_action_t request_action;
        response_t responseHandler;
        error_t errorHandler;
    };

    template <typename TId, typename TRet, typename TErrorData, typename... TArgs>
    class SyncCall : public Call<TId, TRet, TErrorData, TArgs...> {
      public:
        using request_t = Request<TId, TArgs...>;
        using request_action_t = std::function<void(const boost::json::object&)>;

        using response_t = ResponseHandler<typename TId::type, TRet>;
        using response_action_t = typename ResponseHandler<typename TId::type, TRet>::callback_t;

        using error_t = ErrorHandler<typename TId::type, TErrorData>;
        using error_action_t = typename ErrorHandler<typename TId::type, TErrorData>::callback_t;

        template <typename... UArgs>
        SyncCall(const std::string& method, request_action_t request_action, UArgs... args)
          : request(method, args...),
            request_action(request_action),
            responseHandler([this](const typename TId::type& id, const TRet& ret) -> void {
              responses[id] = ret;
            }),
            errorHandler([this](const typename TId::type& id, std::int32_t code, const std::string& message, const TErrorData& data) -> void {
              responses[id] = error::ErrorCode(code, message, data);
            })
        {}

        error::maybe_failed<TRet, error::ErrorCode> operator()(TArgs... args) {
          typename TId::type request_id;

          std::future<void> f = std::async(std::launch::async, [this, &request_id, args...]() {
            // Generate and transmit request
            boost::json::object requestObject = request(args...);

            util::FromJson<typename TId::type> id_conv;
            request_id = id_conv(request.at("id")).getSuccess();

            if ( request_action ) {
              request_action(requestObject);
            }

            // Block until a response is received
            boost::json::object responseObject = CallLog<typename TId::type>::Wait(requestObject);

            if ( responseObject.contains("result") ) {
              typename response_t::maybe_failed succeeded = responseHandler(responseObject);
              if ( !succeeded ) {
                BOOST_LOG_TRIVIAL(error) << "Response handling failed: " << static_cast<std::string>(succeeded.getFailed());
              }
            }
            else if ( responseObject.contains("error") ) {
              typename error_t::maybe_failed succeeded = errorHandler(responseObject);
              if ( !succeeded ) {
                BOOST_LOG_TRIVIAL(error) << "Error handling failed: " << static_cast<std::string>(succeeded.getFailed());

              }
            }
            else {
              throw std::runtime_error("Unknown response type");
            }
          });

          // Wait for async process to finish
          f.wait();

          // Get copy and remove entry
          error::maybe_failed<TRet, error::ErrorCode> result = responses[request_id];
          responses.erase(request_id);

          return result;
        }

      protected:
        request_t request;
        request_action_t request_action;
        response_t responseHandler;
        error_t errorHandler;
        std::map<typename TId::type, error::maybe_failed<TRet, error::ErrorCode>> responses;
    };

    template <typename TId, typename TRet, typename... TArgs>
    class SyncCall<TId, TRet, void, TArgs...> {
      public:
        using request_t = Request<TId, TArgs...>;
        using request_action_t = std::function<void(const boost::json::object&)>;

        using response_t = ResponseHandler<typename TId::type, TRet>;
        using response_action_t = typename ResponseHandler<typename TId::type, TRet>::callback_t;

        using error_t = ErrorHandler<typename TId::type, void>;
        using error_action_t = typename ErrorHandler<typename TId::type, void>::callback_t;

        template <typename... UArgs>
        SyncCall(const std::string& method, request_action_t request_action, UArgs... args)
          : request(method, args...),
            request_action(request_action),
            responseHandler([this](const typename TId::type& id, const TRet& ret) -> void {
              responses[id] = error::maybe_failed<TRet, error::ErrorCode>(ret);
            }),
            errorHandler([this](const typename TId::type& id, std::int32_t code, const std::string& message) -> void {
              responses[id] = error::maybe_failed<TRet, error::ErrorCode>(error::ErrorCode(code, std::string(message)));
            })
        {}

        error::maybe_failed<TRet, error::ErrorCode> operator()(TArgs... args) {
          typename TId::type request_id;

          if ( !request_action ) {
            // Avoid running a new thread for a call that can't be finished
            return error::NotYetImplemented();
          }

          std::future<void> f = std::async(std::launch::async, [this, &request_id, args...]() {
            // Generate and transmit request
            boost::json::object requestObject = request(args...);

            util::FromJson<typename TId::type> id_conv;
            request_id = id_conv(requestObject.at("id")).getSuccess();

            if ( request_action ) {
              request_action(requestObject);
            }

            // Block until a response is received
            boost::json::object responseObject = CallLog<typename TId::type>::Wait(requestObject);

            if ( responseObject.contains("result") ) {
              typename response_t::maybe_failed succeeded = responseHandler(responseObject);
              if ( !succeeded ) {
                BOOST_LOG_TRIVIAL(error) << "Response handling failed: " << static_cast<std::string>(succeeded.getFailed());
              }
            }
            else if ( responseObject.contains("error") ) {
              typename error_t::maybe_failed succeeded = errorHandler(responseObject);
              if ( !succeeded ) {
                BOOST_LOG_TRIVIAL(error) << "Error handling failed: " << static_cast<std::string>(succeeded.getFailed());

              }
            }
            else {
              throw std::runtime_error("Unknown response type");
            }
          });

          // Wait for async process to finish
          f.wait();

          // Get copy and remove entry
          error::maybe_failed<TRet, error::ErrorCode> result = responses[request_id];
          responses.erase(request_id);

          return result;
        }

        void setAction(request_action_t action) {
          request_action = action;
        }

        const std::string& getMethod() const {
          return request.getMethod();
        }

        void setMethod(const std::string& method) {
          request.setMethod(method);
        }

      protected:
        request_t request;
        request_action_t request_action;
        response_t responseHandler;
        error_t errorHandler;
        std::map<typename TId::type, error::maybe_failed<TRet, error::ErrorCode>> responses;
    };
  }
}
