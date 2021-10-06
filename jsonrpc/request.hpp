#pragma once

#include "parameter.hpp"
#include "error/error.hpp"
#include "util/util.hpp"

namespace ts7 {
  namespace jsonrpc {
    template <typename TId>
    struct RequestID {
      using type = TId;

        static TId generate() {
          static TId id = TId();
          return ++id;
        }
    };

    template <typename TId, typename... TArgs>
    class Request {
      public:
        using parameter_tuple_t = std::tuple<Parameter<TArgs>...>;

        template <typename... UArgs>
        inline Request(const std::string& method, UArgs... args)
          : method(method),
            parameter(Parameter<TArgs>(args)...)
        {}

        const std::string& getMethod() const {
          return method;
        }

        void setMethod(const std::string& name) {
          method = name;
        }

        inline boost::json::object operator()(TArgs... args) const {
          boost::json::object notification;
          notification["jsonrpc"] = "2.0";
          notification["id"] = util::AsJson<typename TId::type>(TId::generate());
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
        inline void apply_args(boost::json::object& o, const TTuple& tuple, std::index_sequence<Is...>) const {
           ((std::get<Is>(parameter).store(o, std::get<Is>(tuple))), ... );
        }
    };
  }
}
