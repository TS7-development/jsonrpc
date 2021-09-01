#pragma once

#include "parameter.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename... TArgs>
    class Notification {
      public:
        using parameter_tuple_t = std::tuple<Parameter<TArgs>...>;

        template <typename... UArgs>
        Notification(const std::string& method, UArgs... args)
          : method(method),
            parameter(Parameter<TArgs>(args)...)
        {}

        const std::string& getMethod() const {
          return method;

        }

        boost::json::object operator()(TArgs... args) const {
          boost::json::object notification;
          notification["jsonrpc"] = "2.0";
          notification["method"] = method;

          boost::json::object params;
          std::tuple<TArgs...> argTuple{args...};
          apply_args(params, argTuple, std::make_index_sequence<std::tuple_size<parameter_tuple_t>::value>{});
          notification["params"] = params;

          return notification;
        }

      protected:
        std::string method;
        parameter_tuple_t parameter;

        template <typename TTuple, std::size_t... Is>
        void apply_args(boost::json::object& o, const TTuple& tuple, std::index_sequence<Is...>) const {
           ((std::get<Is>(parameter).store(o, std::get<Is>(tuple))), ... );
        }
    };
  }
}
