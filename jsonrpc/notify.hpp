#pragma once

#include <functional>
#include <future>

#include "notification.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename... TArgs>
    class Notify {
      public:
        using notification_t = Notification<TArgs...>;
        using notification_action_t = std::function<void(const boost::json::object&)>;

        template <typename... UArgs>
        Notify(const std::string& method, notification_action_t notification_action, UArgs... args)
          : notification(method, args...),
            notification_action(notification_action)
        {}

        std::future<void> operator()(TArgs... args) {
          std::future<void> f = std::async(std::launch::async, [this, args...]() {
            // Generate and transmit request
            boost::json::object o = notification(args...);
            if ( notification_action ) {
              notification_action(o);
            }
          });

          return f;
        }

      protected:
        notification_t notification;
        notification_action_t notification_action;
    };
  }
}
