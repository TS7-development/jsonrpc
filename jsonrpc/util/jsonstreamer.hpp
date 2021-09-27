#pragma once

#include <optional>
#include <string>

#include <boost/json.hpp>

#include "fsm.hpp"

namespace ts7 {
  namespace jsonrpc {
    namespace util {
      struct JsonStreamer {
        inline JsonStreamer& operator+=(const std::string& s) {
          data += s;
          return *this;
        }

        inline std::string& getData() {
          return data;
        }

        inline const std::string& getData() const {
          return data;
        }

        std::optional<boost::json::value> getNextChunk() {
          Interface interface(this);
          return std::optional<boost::json::value>();
        }

      protected:
        struct Interface {
          inline Interface(JsonStreamer* js)
            : parent(js),
              fsm(this),
              start(end()),
              actual(begin())
          {}

          std::string::iterator begin() {
            return parent->getData().begin();
          }

          std::string::iterator end() {
            return parent->getData().end();
          }

          inline void push(State<Interface>* state) {
            fsm.pushState(state);
          }

          inline void pop() {
            fsm.popState();
          }

          inline State<Interface>* current() const {
            return fsm.current();
          }

          inline std::size_t size() const {
            return fsm.size();
          }

          Interface& operator++() {
            ++actual;
            return *this;
          }

          inline bool operator==(const std::string::iterator& it) const {
            return it == actual;
          }

          inline bool operator==(char c) const {
            return *actual == c;
          }

          inline bool operator==(State<Interface>* state) const {
            return current() == state;
          }

          inline bool found() const {
            /// @todo Implement
            return false;
          }

          inline bool failed() const {
            /// @todo Implement
            return false;
          }

          JsonStreamer* parent;
          StackingFiniteStateMachine<Interface> fsm;
          std::string::iterator start;
          std::string::iterator actual;
        };

        struct Object;
        struct Array;
        struct Number;
        struct String;
        struct Bool;
        struct Key;

        struct Object : public DerivedState<Interface, Object> {
          virtual void onUpdate(Interface* i) const {
          }
        };

        std::string data;
      };
    }
  }
}
