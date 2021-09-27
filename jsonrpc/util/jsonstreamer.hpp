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

        boost::json::value getNextChunk() {
          Interface interface(this);

          while(!interface.found() && !interface.failed()) {
            interface.update();
          }

          if (interface.failed()) {
            return boost::json::value();
          }

          std::string content(interface.start, interface.actual+1);
          return boost::json::parse(content);
        }

      protected:
        struct Interface;

        struct Initial : public DerivedState<Interface, Initial> {
          virtual void onUpdate(Interface* i) const;
        };

        struct Object : public DerivedState<Interface, Object> {
          virtual void onUpdate(Interface* i) const;
        };

        struct Key : public DerivedState<Interface, Key> {
          virtual void onUpdate(Interface* i) const;
        };

        struct Value : public DerivedState<Interface, Value> {
          virtual void onUpdate(Interface* i) const;
        };

        struct Array : public DerivedState<Interface, Array> {
          virtual void onUpdate(Interface* i) const;
        };

        struct Null : public DerivedState<Interface, Null> {
          virtual void onEnter(Interface* i) const;
        };

        struct Number : public DerivedState<Interface, Number> {
          virtual void onEnter(Interface* i) const;
        };

        struct String : public DerivedState<Interface, String> {
          virtual void onUpdate(Interface* i) const;
        };

        struct Bool : public DerivedState<Interface, Bool> {
          virtual void onEnter(Interface* i) const;
        };

        struct Escape : public DerivedState<Interface, Escape> {
          virtual void onUpdate(Interface* i) const;
        };

        struct Interface {
          inline Interface(JsonStreamer* js)
            : parent(js),
              fsm(this),
              start(end()),
              actual(begin())
          {
            fsm.pushState(Initial::Instance());
          }

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

          inline void saveStart() {
            start = actual;
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

          inline bool operator==(const std::string& value) {
            std::string compare = std::string(actual, actual+value.length());
            return value == compare;
          }

          inline bool operator==(const std::vector<char>& charList) {
            return std::find(charList.begin(), charList.end(), *actual) != charList.end();
          }

          inline bool operator==(State<Interface>* state) const {
            return current() == state;
          }

          inline bool found() const {
            return size() == 0 && start != parent->data.end() && actual != parent->data.end();
          }

          inline bool failed() const {
            return actual == parent->data.end();
          }

          inline void update() {
            fsm.update();
            if (fsm.size() > 0) {
              ++actual;
            }
          }

          JsonStreamer* parent;
          StackingFiniteStateMachine<Interface> fsm;
          std::string::iterator start;
          std::string::iterator actual;
        };

        std::string data;
      };

      void JsonStreamer::Initial::onUpdate(Interface *i) const {
        if (*i == '{') {
          i->pop();
          i->saveStart();
          i->push(JsonStreamer::Object::Instance());
        }
        else if(*i == '[') {
          i->pop();
          i->saveStart();
          i->push(JsonStreamer::Array::Instance());
        }
      }

      void JsonStreamer::Object::onUpdate(Interface *i) const {
        if (*i == '"') {
          i->push(JsonStreamer::Key::Instance());
        }
        else if (*i == ':') {
          i->push(JsonStreamer::Value::Instance());
        }
        else if (*i == '}') {
          i->pop();
        }
      }

      void JsonStreamer::Array::onUpdate(Interface *i) const {
        if (*i == ']') {
          i->pop();
          return;
        }

        i->push(JsonStreamer::Value::Instance());
      }

      void JsonStreamer::Value::onUpdate(Interface *i) const {
        if (*i == '{') {
          i->push(JsonStreamer::Object::Instance());
        }
        else if (*i == '[') {
          i->push(JsonStreamer::Array::Instance());
        }
        else if (*i == '"') {
          i->push(JsonStreamer::String::Instance());
        }
        else if (*i == std::vector<char>{'+','-', '.','0','1','2','3','4','5','6','7','8','9'}) {
          i->push(JsonStreamer::Number::Instance());
        }
        else if(*i == "true" || *i == "false") {
          i->push(JsonStreamer::Bool::Instance());
        }
        else if (*i == "null") {
          i->push(JsonStreamer::Null::Instance());
        }
        else if (*i == ',') {
          i->pop();
        }
        else if (*i == "}" || *i == "]") {
          i->pop();
          i->pop();
        }
      }

      void JsonStreamer::Key::onUpdate(Interface *i) const {
        if (*i == '"') {
          i->pop();
        }
      }

      void JsonStreamer::String::onUpdate(Interface *i) const {
        if (*i == '\\') {
          i->push(JsonStreamer::Escape::Instance());
        }
        if (*i == '"') {
          i->pop();
        }
      }

      void JsonStreamer::Number::onEnter(Interface *i) const {
        i->pop();
      }

      void JsonStreamer::Bool::onEnter(Interface *i) const {
        i->pop();
      }

      void JsonStreamer::Null::onEnter(Interface *i) const {
        i->pop();
      }

      void JsonStreamer::Escape::onUpdate(Interface *i) const {
        i->pop();
      }
    }
  }
}
