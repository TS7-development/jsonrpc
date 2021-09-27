#pragma once

#include <stack>

namespace ts7 {
  namespace jsonrpc {
    namespace util {
      template <typename T>
      struct State {
        virtual void onEnter(T*) const {}
        virtual void onUpdate(T*) const {}
        virtual void onLeave(T*) const {}
      };

      template <typename T, typename TDerived>
      struct DerivedState : public State<T> {
        static TDerived* Instance() {
          static TDerived state;
          return &state;
        }
      };

      template <typename T>
      class FiniteStateMachine {
        public:
          void changeState(State<T>* state) {
            if (nullptr != actual) {
              actual->onLeave(owner);
            }

            actual = state;
            actual->onEnter(owner);
          }

          void update() {
            if (nullptr != actual) {
              actual->onUpdate(owner);
            }
          }

          [[nodiscard]] inline State<T>* current() const {
            return actual;
          }

        protected:
          T* owner = nullptr;
          State<T>* actual = nullptr;
      };

      template <typename T>
      class StackingFiniteStateMachine {
        public:
          inline StackingFiniteStateMachine(T* owner)
            : owner(owner)
          {}

          void pushState(State<T>* state) {
            if (size() > 0) {
              State<T>* old = current();
              old->onLeave(owner);
            }

            stack.push_back(state);
            state->onEnter(owner);
          }

          void popState() {
            if (size() > 0) {
              State<T>* old = current();
              old->onLeave(owner);
              stack.pop();
            }

            if (size() > 0) {
              State<T>* actual = current();
              actual->onEnter(owner);
            }
          }

          void update() {
            if (size() > 0) {
              State<T>* actual = current();
              actual->onUpdate(owner);
            }
          }

          [[nodiscard]] inline std::size_t size() const {
            return stack.size();
          }

          [[nodiscard]] inline State<T>* current() const {
            return stack.top();
          }

        protected:
          T* owner = nullptr;
          std::stack<State<T>*> stack;
      };
    }
  }
}
