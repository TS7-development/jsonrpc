#include "variadic-members.h"

#include <iostream>
#include <tuple>
#include <string>
#include <functional>
#include <boost/json.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace variadic_member {
      /// Converts from JSON to T
      template <typename T>
      struct FromJson {};

      /// Converts from JSON to int
      template <>
      struct FromJson<int> {
        FromJson(const boost::json::value& v)
          : v(v)
        {}

        operator int() const {
          return v.as_int64();
        }

        const boost::json::value& v;
      };


      /**
       * @brief Function
       *
       * Represents a function that takes any amount of parameter.
       *
       * @tparam TRet Data type of the return type.
       * @tparam TArgs Parameter list of the function.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename TRet, typename... TArgs>
      struct MyFn {
          /**
           * @brief Parameter
           *
           * Represents a parameter of the function.
           *
           * @tparam U Data type of the parameter.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          template <typename U>
          struct Parameter {
              /// Constructs paramter by its name
              Parameter(const std::string& name)
                : name(name)
              {}

              /// Parses the parameter from the JSON object
              U get(const boost::json::object& o) const {
                return FromJson<U>(o.at(name));
              }

              /// Name of the parameter
              std::string name;
          };

          /// Type definition for the function
          using fn_t = std::function<TRet(TArgs...)>;

          /// Type definition of the parameter tuple
          using tuple_t = std::tuple<Parameter<TArgs>...>;

          /**
           * @brief constructor
           *
           * Constructs the function by a function pointer and a list parameter.
           *
           * @param fn Function pointer to be executed after parsing a JSON object.
           * @param args List of parameter that need to be parsed.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          MyFn(fn_t fn, const tuple_t& args)
            : fn(fn),
              args(args)
          {}

          /**
           * @brief Function pointer execution
           *
           * Parses all parameter and executes the stored function pointer.
           *
           * @tparam Is Index sequence.
           *
           * @param o Const reference to the JSON object that shall be parsed.
           *
           * @return Returns the result of the stored function pointer.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          template <std::size_t... Is>
          TRet apply(const boost::json::object& o, std::index_sequence<Is...>) {
             return fn( std::get<Is>(args).get(o) ... );
          }

          /**
           * @brief Function call operator
           *
           * Executes the stored function pointer and provides the parameter provided by a JSON object.
           *
           * @param o JSON object that contains the parameter.
           *
           * @return Returns the result of the stored function pointer.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          TRet operator()(const boost::json::object& o) {
            return apply(o, std::make_index_sequence<std::tuple_size<tuple_t>::value>{});
          }

          /// Function pointer to execute
          fn_t fn;

          /// Tuple of parameter
          tuple_t args;
      };

      /// Dummy function that sums up 1 integer
      int sum1(int a) {
        return a;
      }

      /// Dummy function that sums up 2 integers
      int sum2(int a, int b) {
        return a + b;
      }

      /// Dummy function that sums up 3 integer
      int sum3(int a, int b, int c) {
        return a + b + c;
      }

      /// Dummy function that sums to operands
      template <typename T1, typename T2>
      int sumN(T1 t1, T2 t2) {
        return t1 + t2;
      }

      /// DUmmy function that sums any amount of operands
      template <typename T, typename... TArgs>
      int sumN(T t, TArgs... args) {
        return t + sumN(args...);
      }

      /**
       * @brief Function Call example
       *
       * Demonstrates the usage of the \p MyFn structure with multiple different functions.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      void functionCall() {
        {
          MyFn<int, int> fn(sum1, std::make_tuple(
            MyFn<int, int>::Parameter<int>("a")
          ));

          boost::json::object parameter;
          parameter["a"] = 3;
          parameter["b"] = 7;
          parameter["c"] = 10;
          parameter["d"] = 13;

          int result = fn(parameter);
          std::cout << "result = " << result << std::endl;
        }

        {
          MyFn<int, int, int> fn(sum2, std::make_tuple(
            MyFn<int, int, int>::Parameter<int>("a"),
            MyFn<int, int, int>::Parameter<int>("b")
          ));

          boost::json::object parameter;
          parameter["a"] = 3;
          parameter["b"] = 7;
          parameter["c"] = 10;
          parameter["d"] = 13;

          int result = fn(parameter);
          std::cout << "result = " << result << std::endl;
        }

        {
          MyFn<int, int, int, int> fn(sum3, std::make_tuple(
            MyFn<int, int, int, int>::Parameter<int>("a"),
            MyFn<int, int, int, int>::Parameter<int>("b"),
            MyFn<int, int, int, int>::Parameter<int>("c")
          ));

          boost::json::object parameter;
          parameter["a"] = 3;
          parameter["b"] = 7;
          parameter["c"] = 10;
          parameter["d"] = 13;

          int result = fn(parameter);
          std::cout << "result = " << result << std::endl;
        }

        {
          MyFn<int, int, int, int> fn(sumN<int, int, int>, std::make_tuple(
            MyFn<int, int, int, int>::Parameter<int>("a"),
            MyFn<int, int, int, int>::Parameter<int>("b"),
            MyFn<int, int, int, int>::Parameter<int>("c")
          ));

          boost::json::object parameter;
          parameter["a"] = 3;
          parameter["b"] = 7;
          parameter["c"] = 10;
          parameter["d"] = 13;

          int result = fn(parameter);
          std::cout << "result = " << result << std::endl;
        }
      }
    }
  }
}
