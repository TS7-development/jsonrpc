#include "test.h"

#include <iostream>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace json_call {
      /**
       * @brief Demo Function
       *
       * Simple demonstration, that is just dumping the provided variables.
       *
       * @param value1 Just a string to be dumped.
       * @param value2 Just an integer to be dumped.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      void testFunc(const std::string& value1, int value2) {
        std::cout << "v1=" << value1 << ", v2=" << value2 << std::endl;
      }

      /**
       * @brief First version
       *
       * This is the first version of how to implement a function call with a json object, that will be converted
       * to a function call with the original function parameter (string, int).
       *
       * The Idea is to save a function pointer with the matching signature and provide a call operator with that
       * accepts a json object. In this example to keep it simple and focus on the idea the parameter amount, the
       * parameter types and the paramater names are fixed.
       *
       * This has worked quite good on the first glance.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      struct Test_v1 {
          /**
           * @brief constructor
           *
           * Constructor that just stores the provided function pointer. Needs to be called explicitly to avoid
           * uncontrolled implicit conversions.
           *
           * @param fn Function pointer that shall be stored.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline explicit Test_v1(std::function<void(const std::string&, int)> fn)
            : fn(fn)
          {}

          /**
           * @brief call operator
           *
           * Call operator that allows the object to act like a function call, that just takes a json object and
           * will collect all required entries to call the stored function pointer. In this case those are \p value1
           * as string and \p value 2 as int.
           *
           * @param o Json object that contains the call parameter.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          void operator()(const boost::json::object& o) {
            std::string v1 = o.at("value1").as_string().c_str();
            int v2 = o.at("value2").as_int64();

            fn(v1, v2);
          }

          /// Stored function pointer
          std::function<void(const std::string&, int)> fn;
      };

      /**
       * @brief example1
       *
       * Example that gets a read-only reference to the \p params object of a json rpc request. Then it creates a
       * json rpc request handler Test_v1 that stores testFunc as respective function pointer. This gets then
       * internally converted to the matching function call of testFunc, when the function call operator of the
       * Test_v1 instance is excecuted.
       *
       * @note In this example the parameter count, types and names are fixed to keep things simple.
       *
       * @param test The \p params object of a json rpc request.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      void example1(const boost::json::object& test) {
        Test_v1 t(testFunc);
        t(test);
      }
    }
  }
}
