#include <iostream>
#include <chrono>
#include <string>

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace echo {
      /**
       * @brief TCP connection
       *
       * Class that represents a TCP connection.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
      {
        public:
          /// Type definition of a connection pointer
          typedef boost::shared_ptr<TcpConnection> Ptr;

          /**
           * @brief Create
           *
           * Creates a TCP connection with the provided IO context.
           *
           * @param io_context Provided IO context.
           *
           * @return Retuns a shared pointer to the created TCP connection.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          static Ptr Create(boost::asio::io_context& io_context)
          {
            return Ptr(new TcpConnection(io_context));
          }

          /**
           * @brief socket
           *
           * @return Returns the socket of the TCP connection.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          boost::asio::ip::tcp::socket& socket()
          {
            return sock;
          }

          /**
           * @brief echo
           *
           * Initiates the echo server.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          void echo()
          {
            std::memset(msg, 0, sizeof(msg));
            sock.async_read_some(
              boost::asio::buffer(msg, sizeof(msg)),
              boost::bind(
                &TcpConnection::handle_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
              )
            );
          }

        protected:
          /**
           * @brief constructor
           *
           * Protected constructor to force usage for the Create factory method.
           *
           * @param ctx Provided IO context for the socket creation.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          TcpConnection(boost::asio::io_context& ctx)
            : sock(ctx)
          {
          }

          /**
           * @brief read callback
           *
           * This function gets called, when there are data got read from the socket.
           *
           * @param error Error code of reading data.
           * @param bytes_transferred Amount of bytes received.
           *
           * @note This function call write on the socket, which will cause \p handle_write to be executed.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
            std::cout << "read: " << error << ", " << bytes_transferred << std::endl;
            if (!error && bytes_transferred > 0) {
              std::string_view vmsg(msg);
              std::cout << "<- " << msg << std::endl;
              if (vmsg.empty()) {
                std::strcpy(msg, "<empty>");
              }

              if (vmsg == ":quit") {
                exit(0);
              }

              boost::asio::async_write(sock, boost::asio::buffer(msg, bytes_transferred), boost::asio::transfer_all(),
                  boost::bind(&TcpConnection::handle_write, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            }
            else if (error) {
              std::cout << "read error: " << error.message() << std::endl;
              echo();
            }
          }

          /**
           * @brief write callback
           *
           * Callback, that is executed when data got written to the socket.
           *
           * @param error Error code, if an error occured while transferring the data.
           * @param bytes_transferred Amount of bytes transferred.
           *
           * @note This function will call \p echo to restart the echoing process.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
          {
            std::cout << "write: " << error << ", " << bytes_transferred << std::endl;
            if (!error && bytes_transferred > 0) {
              std::cout << "-> " << msg << std::endl;
              echo();
            }
            else if (error) {
              std::cout << "write error: " << error.message() << std::endl;
            }
          }

          /// Socket
          boost::asio::ip::tcp::socket sock;

          /// Message buffer for receiving
          char msg[1024] = {0};
      };

      /**
       * @brief TCP server
       *
       * Class that represents a TCP server that handles echo messages asynchronous.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      class TcpServer {
        public:
          /**
           * @brief constructor
           *
           * Creates a TCP server by a provided IO context.
           *
           * @param ctx IO context that shall be used by the server.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          TcpServer(boost::asio::io_context& ctx)
            : ctx(ctx),
              acceptor(ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 9300))
          {}

          /**
           * @brief Start accept
           *
           * Starts accepting clients on the TCP server.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          void startAccept() {
            std::cout << "Waiting for new client" << std::endl;

            TcpConnection::Ptr new_conn = TcpConnection::Create(ctx);
            acceptor.async_accept(
                  new_conn->socket(),
                  boost::bind(&TcpServer::handleAccept, this, new_conn, boost::asio::placeholders::error)
            );
          }

        protected:
          /**
           * @brief Handle new client
           *
           * Handles a newly accepted client to the server.
           *
           * @param conn The connection pointer to the accepted client.
           * @param error Error code, if accepting the client failed for any reason.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          void handleAccept(TcpConnection::Ptr conn, const boost::system::error_code& error) {
            if (!error) {
              std::cout << "Accepted new client" << std::endl;
              conn->echo();
              std::cout << "Finished echoing" << std::endl;
            }

            startAccept();
          }

          /// IO context of the server
          boost::asio::io_context& ctx;

          /// Acceptor for new connections
          boost::asio::ip::tcp::acceptor acceptor;
      };
    }
  }
}

// python3 test.py --tcp --quit "Hello World" "Hallo Welt"
int main()
{
  // Using namespace just for easier access
  using namespace ts7::jsonrpc_playground::echo;

  std::cout << "Echo server" << std::endl;

  try {
    // Start & run the TCP server
    boost::asio::io_context io_context;
    TcpServer echoServer(io_context);
    echoServer.startAccept();
    io_context.run();
  }
  catch(std::exception& e) {
    // Something failed for any reason
    std::cout << "Exception caught: " << e.what() << std::endl;
  }

  return 0;
}
