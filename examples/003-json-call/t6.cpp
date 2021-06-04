#include "test.h"

#include <iostream>

template <typename TReturn, typename T1, typename T2>
struct Test_v6 {
    using callback_t = std::function<TReturn(T1, T2)>;

    template <typename U>
    using remove_cref = typename std::remove_const<typename std::remove_reference<U>::type>::type;

    template <typename U>
    struct Parameter {
        constexpr inline explicit Parameter(const std::string& name, bool hasDefault = false, const U& defaultValue = remove_cref<U>())
          : name(name),
            hasDefault(hasDefault),
            defaultValue(defaultValue)
        {}

        remove_cref<U> operator()(const boost::json::object& o) {
          FromJson<remove_cref<U>> v;
          if (o.contains(name)) {
            return v(o.at(name));
          }

          if (hasDefault) {
            return defaultValue;
          }

          // This should throw an exception in theory
          static remove_cref<U> tmp;
          return tmp;
        }

        std::string name;
        bool hasDefault;
        U defaultValue;
    };

    template<typename U>
    struct OptionalParameter {
      constexpr inline explicit OptionalParameter(const std::string& name, const U& defaultValue = U())
        : name(name),
          defaultValue(defaultValue)
      {}

      operator Parameter<U>() const {
        return Parameter<U>(name, true, defaultValue);
      }

      std::string name;
      U defaultValue;
    };

    template <typename U1, typename U2>
    inline explicit Test_v6(callback_t fn, const U1& p1, const U2& p2)
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

struct Dummy {
    int x;
    int y;
    int z;
};

void example6([[maybe_unused]] const boost::json::object& test3) {
  using Tv6 = Test_v6<void, const Dummy&, const Dummy&>;

  Tv6 t6(
    [](const Dummy&, const Dummy&) {
      std::cout << "Dummy called" << std::endl;
    },
    "a",
    "b"
  );

#ifdef T6_CREATE_COMPILER_ERROR
  t6(test3);
#endif
}

