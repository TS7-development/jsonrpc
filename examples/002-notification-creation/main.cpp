#include <cstdint>
#include <iostream>

#include <jsonrpc/notification.hpp>
#include <jsonrpc/notification_handler.hpp>

using NotificationFailure = ts7::jsonrpc::util::maybe_failed<void, ts7::jsonrpc::util::ErrorCode>;

// Real handler to execute when Notification received
NotificationFailure test_callback(std::uint32_t code, std::string message) {
  std::cout << "code: " << code << std::endl;
  std::cout << "message: " << message << std::endl;

  return NotificationFailure();
}

int main()
{
  // Notification
  std::cout << "### Creation ###" << std::endl;
  ts7::jsonrpc::Notification<std::uint32_t, std::string> test(
        "test",       // method name
        "code",       // 1st parameter std::uint32_t
        "message"     // 2nd parameter std::string
  );

  boost::json::object notification = test(512, "Hello World");
  std::cout << notification << std::endl;
  std::cout << std::endl;

  // Notification handling
  std::cout << "### Handling ###" << std::endl;
  ts7::jsonrpc::NotificationHandler<std::uint32_t, std::string> testHandler(
        test_callback,
        "code",
        "message"
  );

  testHandler(notification);
  std::cout << std::endl;

  return 0;
}
