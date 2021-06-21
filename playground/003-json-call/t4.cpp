#include "test.h"
#include "t4.h"

namespace ts7 {
  namespace jsonrpc_playground {
    namespace json_call {
      /**
       * @brief example4
       *
       * This example is behaving like \ref example3, except that \p value2 is now optional. If it is missing a
       * zero will be provided by the json callback. Therefore the json callback is executed two times. One time
       * with both parameter provided. The second time with \p value2 missing to force the usage of the default
       * value.
       *
       * @note This test is still using \ref testFun to dump the provided values.
       *
       * @param test Json object that contains \p value1 and \p value2.
       * @param test2 Json object that only contains \p value2.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      void example4(const boost::json::object& test, const boost::json::object& test2) {
        using Tv4 = Test_v4<void, const std::string&, int>;
        Tv4 t4(testFunc, "value1", Tv4::OptionalParameter<int>("value2", 0));
        t4(test);
        t4(test2);
      }
    }
  }
}
