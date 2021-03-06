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
#include "../util/observer.hpp"

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
      template <typename TId, typename TOwner>
      class TcpServer {
        public:
          using module_t = ts7::jsonrpc::Module<TId>;

          // Events
          using server_started_t = util::Observer<std::uint16_t>;
          using new_client_accepted_t = util::Observer<typename TcpConnection<TId, TOwner>::Ptr>;

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
          inline TcpServer(TOwner* owner, boost::asio::io_context& ctx, uint16_t port = 9200)
            : owner(owner),
              ctx(ctx),
              acceptor(ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
          {
            BOOST_LOG_TRIVIAL(info) << "Listening on port " << port;
            server_started.notify(port);
          }

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

            typename TcpConnection<TId, TOwner>::Ptr new_conn = TcpConnection<TId, TOwner>::Create(owner, ctx, &procedures);

            acceptor.async_accept(
                  new_conn->socket(),
                  boost::bind(&TcpServer::handle_accept, this, new_conn, boost::asio::placeholders::error)
            );
          }

          template <typename T>
          inline void registerRequest(const std::string& name, const T& t) {
            procedures.addRequest(name, t);
          }

          template <typename T>
          inline void registerNotification(const std::string& name, const T& t) {
            procedures.addNotification(name, t);
          }

          template <typename T>
          inline void registerFallback(const T& t) {
            procedures.setFallback(t);
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
          inline void handle_accept(typename TcpConnection<TId, TOwner>::Ptr conn, const boost::system::error_code& error) {
            if (!error) {
              BOOST_LOG_TRIVIAL(info) << "Accepted new client";
              new_client_accepted.notify(conn);
              conn->waitForRequest();
            }

            startAccept();
          }

        public:
          /// Event Callbacks
          server_started_t server_started;
          new_client_accepted_t new_client_accepted;

        protected:
          /// Owner of the server
          TOwner* owner;

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
