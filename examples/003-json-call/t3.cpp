#include "test.h"

template <typename TReturn, typename T1, typename T2>
struct Test_v3 {
    using callback_t = std::function<TReturn(T1, T2)>;

    template <typename U>
    using remove_cref = typename std::remove_const<typename std::remove_reference<U>::type>::type;

    template <typename U>
    struct Parameter {
        constexpr inline explicit Parameter(const std::string& name)
          : name(name)
        {}

        remove_cref<U> operator()(const boost::json::object& o) {
          FromJson<remove_cref<U>> v;
          return v(o.at(name));
        }

        std::string name;
    };

    inline explicit Test_v3(callback_t fn, const std::string& p1, const std::string& p2)
      : fn(fn),
        p1(p1),
        p2(p2)
    {}

    TReturn operator()(const boost::json::object& o) {
      remove_cref<T1> t1 = p1(o);
      remove_cref<T2> t2 = p2(o);
      return fn(t1, t2);
    }

    callback_t fn;
    Parameter<T1> p1;
    Parameter<T2> p2;
};

void example3(const boost::json::object& test) {
  Test_v3<void, const std::string&, int> t3(testFunc, "value1", "value2");
  t3(test);
}
