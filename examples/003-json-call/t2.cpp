#include "test.h"

namespace ts7 {
  namespace jsonrpc {
    namespace examples {
      namespace json_call {
        template <typename TReturn, typename T1, typename T2>
        struct Test_v2 {
            using callback_t = std::function<TReturn(T1, T2)>;

            template <typename U>
            using remove_cref = typename std::remove_const<typename std::remove_reference<U>::type>::type;

            inline explicit Test_v2(callback_t fn)
              : fn(fn)
            {}

            TReturn operator()(const boost::json::object& o) {
              FromJson<remove_cref<T1>> v1;
              FromJson<remove_cref<T2>> v2;

              return fn(v1(o.at("value1")), v2(o.at("value2")));
            }

            callback_t fn;
        };

        void example2(const boost::json::object& test) {
          Test_v2<void, const std::string&, int> t2(testFunc);
          t2(test);
        }
      }
    }
  }
}
