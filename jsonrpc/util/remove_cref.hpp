#pragma once

#include <type_traits>

namespace ts7 {
  namespace jsonrpc {
    namespace util {
      /// Const reference remover
      template <typename U>
      using remove_cref = typename std::remove_const<typename std::remove_reference<U>::type>::type;
    }
  }
}
