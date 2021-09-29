#pragma once

#include <iostream>
#include <chrono>
#include <string>

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "../util/jsonstreamer.hpp"
#include "../module.hpp"

namespace ts7 {
  namespace jsonrpc {
    namespace com {
      /**
       * @brief TCP connection
       *
       * Class that represents a TCP connection.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename TId, typename TOwner>
      class TcpConnection : public boost::enable_shared_from_this<TcpConnection<TId, TOwner>>
      {
        public:
          /// Type definition of a connection pointer
          using Ptr = boost::shared_ptr<TcpConnection>;
          using Weak = boost::weak_ptr<TcpConnection>;
          using module_t = ts7::jsonrpc::Module<TId>;

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
          inline static Ptr Create(TOwner* owner, boost::asio::io_context& io_context, module_t* procedures) {
            return Ptr(new TcpConnection(owner, io_context, procedures));
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
          inline boost::asio::ip::tcp::socket& socket() {
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
          void waitForRequest() {

            std::memset(msg, 0, sizeof(msg));
            sock.async_read_some(
              boost::asio::buffer(msg, sizeof(msg)),
              boost::bind(
                &TcpConnection::handle_read,
                this->shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
              )
            );
          }

          inline std::size_t getID() const {
            return id;
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
          inline TcpConnection(TOwner* owner, boost::asio::io_context& ctx, ts7::jsonrpc::Module<std::int32_t>* procedures)
            : owner(owner),
              id(++nextID),
              sock(ctx),
              procedures(procedures)
          {}

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
            if (!error && bytes_transferred > 0) {
              BOOST_LOG_TRIVIAL(debug) << "<- " << msg << std::endl;

              streamer += msg;
              boost::json::value v = streamer.getNextChunk();

              if (v.is_object()) {
                handleRequest(v.as_object());
              }
              else if (v.is_array()) {
                handleRequests(v.as_array());
              }

              waitForRequest();
            }
            else if (error) {
              BOOST_LOG_TRIVIAL(error) << "read error: " << error.message() << " (" << error.value() << ")";
              waitForRequest();
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
          void handle_write(std::string data, const boost::system::error_code& error, size_t bytes_transferred) {
            if (!error && bytes_transferred > 0) {
              BOOST_LOG_TRIVIAL(debug) << "-> " << data;
            }
          }

          void handleRequests(const boost::json::array& a) {
            BOOST_LOG_TRIVIAL(debug) << "Handling batch job";
            for (boost::json::array::const_iterator it = a.begin(); it != a.end(); ++it) {
              boost::json::value sv = *it;
              if (sv.is_object()) {
                handleRequest(sv.as_object());
              }
            }
          }

          void handleRequest(const boost::json::object& o) {
            BOOST_LOG_TRIVIAL(debug) << "Handling job";
            std::future<void> f = std::async(std::launch::async, [this, o]() -> void {
              // o needs to be copied!, because std::async could take longer
              // than handleRequest(), which would end the lifetime of o
              // otherwise. This can be avoided by copying o.
              if (procedures) {
                owner->registerCall(this->shared_from_this());

                boost::json::object response = (*procedures)(o);

                std::stringstream ss;
                ss << response;
                std::string s  = ss.str();

                boost::asio::async_write(sock, boost::asio::buffer(s, s.length()), boost::asio::transfer_all(),
                    boost::bind(&TcpConnection::handle_write, this->shared_from_this(),
                      s,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));

                owner->releaseCall();
              }
            });

            owner->addCallFuture(std::move(f));
          }

          /// Next ID counter
          static std::size_t nextID;

          /// Owner
          TOwner* owner;

          /// Client ID
          std::size_t id;

          /// Socket
          boost::asio::ip::tcp::socket sock;

          /// Message buffer for receiving
          char msg[1024] = {0};

          /// JSON streamer
          ts7::jsonrpc::util::JsonStreamer streamer;

          /// Server RPC module
          module_t* procedures;
      };

      template <typename TId, typename TOwner>
      std::size_t TcpConnection<TId, TOwner>::nextID = 0;
    }
  }
}
