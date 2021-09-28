#pragma once

#include <iostream>
#include <chrono>
#include <cstdint>
#include <string>

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "../module.hpp"

#include "tcpconnection.hpp"

namespace ts7 {
  namespace jsonrpc {
    namespace com {
      /**
       * @brief TCP server
       *
       * Class that represents a TCP server that handles echo messages asynchronous.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename TId>
      class TcpServer {
        public:
          using module_t = ts7::jsonrpc::Module<TId>;

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
          inline TcpServer(boost::asio::io_context& ctx, uint16_t port = 9300)
            : ctx(ctx),
              acceptor(ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
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
          inline void startAccept() {
            BOOST_LOG_TRIVIAL(info) << "Waiting for new client";

            typename TcpConnection<TId>::Ptr new_conn = TcpConnection<TId>::Create(ctx, &procedures);
            acceptor.async_accept(
                  new_conn->socket(),
                  boost::bind(&TcpServer::handle_accept, this, new_conn, boost::asio::placeholders::error)
            );
          }

          template <typename T>
          inline void registerProcedure(const std::string& name, const T& t) {
            procedures[name] = t;
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
          inline void handle_accept(typename TcpConnection<TId>::Ptr conn, const boost::system::error_code& error) {
            if (!error) {
              BOOST_LOG_TRIVIAL(info) << "Accepted new client";
              conn->waitForRequest();
            }

            startAccept();
          }

          /// IO context of the server
          boost::asio::io_context& ctx;

          /// Acceptor for new connections
          boost::asio::ip::tcp::acceptor acceptor;

          /// List of registered procedures
          module_t procedures;
      };
    }
  }
}
