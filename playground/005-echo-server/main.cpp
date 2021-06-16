#include <iostream>
#include <chrono>
#include <string>

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
  public:
    typedef boost::shared_ptr<TcpConnection> Ptr;

    static Ptr Create(boost::asio::io_context& io_context)
    {
      return Ptr(new TcpConnection(io_context));
    }

    boost::asio::ip::tcp::socket& socket()
    {
      return sock;
    }

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
    TcpConnection(boost::asio::io_context& ctx)
      : sock(ctx)
    {
    }

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

    boost::asio::ip::tcp::socket sock;
    char msg[1024] = {0};
};

class TcpServer {
  public:
    TcpServer(boost::asio::io_context& ctx)
      : ctx(ctx),
        acceptor(ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 9300))
    {}

    void startAccept() {
      std::cout << "Waiting for new client" << std::endl;

      TcpConnection::Ptr new_conn = TcpConnection::Create(ctx);
      acceptor.async_accept(
            new_conn->socket(),
            boost::bind(&TcpServer::handleAccept, this, new_conn, boost::asio::placeholders::error)
      );
    }

  protected:
    void handleAccept(TcpConnection::Ptr conn, const boost::system::error_code& error) {
      if (!error) {
        std::cout << "Accepted new client" << std::endl;
        conn->echo();
        std::cout << "Finished echoing" << std::endl;
      }

      startAccept();
    }

    boost::asio::io_context& ctx;
    boost::asio::ip::tcp::acceptor acceptor;
};

// python3 test.py --tcp --quit "Hello World" "Hallo Welt"
int main()
{
  std::cout << "Echo server" << std::endl;

  try {
    boost::asio::io_context io_context;
    TcpServer echoServer(io_context);
    echoServer.startAccept();
    io_context.run();
  }
  catch(std::exception& e) {
    std::cout << "Exception caught: " << e.what() << std::endl;
  }

  return 0;
}
