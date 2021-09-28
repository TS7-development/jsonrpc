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

        inline boost::json::value getNextChunk() {
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
          virtual inline void onUpdate(Interface* i) const;
        };

        struct Object : public DerivedState<Interface, Object> {
          virtual inline void onUpdate(Interface* i) const;
        };

        struct Key : public DerivedState<Interface, Key> {
          virtual inline void onUpdate(Interface* i) const;
        };

        struct Value : public DerivedState<Interface, Value> {
          virtual inline void onUpdate(Interface* i) const;
        };

        struct Array : public DerivedState<Interface, Array> {
          virtual inline void onEnter(Interface* i) const;
          virtual inline void onUpdate(Interface* i) const;
        };

        struct Null : public DerivedState<Interface, Null> {
          virtual inline void onEnter(Interface* i) const;
        };

        struct Number : public DerivedState<Interface, Number> {
          virtual inline void onEnter(Interface* i) const;
        };

        struct String : public DerivedState<Interface, String> {
          virtual inline void onUpdate(Interface* i) const;
        };

        struct Bool : public DerivedState<Interface, Bool> {
          virtual inline void onEnter(Interface* i) const;
        };

        struct Escape : public DerivedState<Interface, Escape> {
          virtual inline void onUpdate(Interface* i) const;
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

          inline std::string::iterator begin() {
            return parent->getData().begin();
          }

          inline std::string::iterator end() {
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

          inline Interface& operator++() {
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

      inline void JsonStreamer::Initial::onUpdate(Interface *i) const {
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

      inline void JsonStreamer::Object::onUpdate(Interface *i) const {
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

      inline void JsonStreamer::Array::onEnter(Interface *i) const {
        if (*i == '[' || *i ==',') {
          i->push(JsonStreamer::Value::Instance());
        }
      }

      inline void JsonStreamer::Array::onUpdate(Interface *i) const {
        if (*i == ']') {
          i->pop();
          return;
        }
      }

      inline void JsonStreamer::Value::onUpdate(Interface *i) const {
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

      inline void JsonStreamer::Key::onUpdate(Interface *i) const {
        if (*i == '\\') {
          i->push(JsonStreamer::Escape::Instance());
        }
        else if (*i == '"') {
          i->pop();
        }
      }

      inline void JsonStreamer::String::onUpdate(Interface *i) const {
        if (*i == '\\') {
          i->push(JsonStreamer::Escape::Instance());
        }
        if (*i == '"') {
          i->pop();
        }
      }

      inline void JsonStreamer::Number::onEnter(Interface *i) const {
        i->pop();
      }

      inline void JsonStreamer::Bool::onEnter(Interface *i) const {
        i->pop();
      }

      inline void JsonStreamer::Null::onEnter(Interface *i) const {
        i->pop();
      }

      inline void JsonStreamer::Escape::onUpdate(Interface *i) const {
        i->pop();
      }
    }
  }
}
