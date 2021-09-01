#include <functional>
#include <iostream>
#include <tuple>
#include <utility>

#include <boost/json.hpp>

namespace ts7 {
  namespace jsonrpc_playground {
    namespace request {
      /**
       * @brief Vector2d
       *
       * Dummy structure just to check, if structs are worrking as well.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      struct Vector2d {
          /// x and y axis values
          int x,y;
      };

      /**
       * @brief To JSON converter
       *
       * Converts any value to a boost value. If the required type cannot be converted
       * directly a template specialization needs to be created.
       *
       * @tparam T Data type that shall be converted.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename T>
      struct AsJson{
          /**
           * @brief constructor
           *
           * Stores a const reference to the value that shall be converted.
           *
           * @param t The value reference that shall be converted.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline AsJson(const T& t)
            : t(t)
          {}

          /**
           * @brief operator boost::json::value conversion
           *
           * Converts the stored value reference to the boost json value.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          operator boost::json::value() const {
            return boost::json::value(t);
          }

          /// Read-only value reference
          const T& t;
      };

      /**
       * @brief Vector2d to JSON converter
       *
       * Converts a Vector2d to a JSON object.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <>
      struct AsJson<Vector2d> {
          /**
           * @brief constructor
           *
           * Stores the value reference of a Vector2d for the JSON conversion.
           *
           * @param t The read-only Vector2d reference that shall be converted.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          inline AsJson(const Vector2d& t)
            : t(t)
          {}

          /**
           * @brief JSON conversion operator
           *
           * Converts the stored Vector2d reference to a JSON object.
           *
           * @since 1.0
           *
           * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
           */
          operator boost::json::value() const {
            boost::json::object vec;
            vec["x"] = AsJson<int>(t.x);
            vec["y"] = AsJson<int>(t.y);

            return vec;
          }

          /// The read-only Vector2d reference
          const Vector2d& t;
      };

      /**
       * @brief Request ID Generator
       *
       * Generates the next available request ID
       *
       * @tparam T The data type of the request ID.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename T>
      struct request_id {
        /// Data type of the request ID
        using id_t = T;

        /**
         * @brief Generate next ID
         *
         * Generates the next request ID available.
         *
         * @return Returns the generated request ID.
         *
         * @note The data type needs to suppor the pre-increment operator.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        static T make() {
          static T t = T();
          return ++t;
        }
      };

      /**
       * @brief Request
       *
       * Stores the request information to generate a proper JSON-RPC request.
       *
       * @tparam TId Structure to generate request ID's.
       * @tparam TArgs List of arguments that needs to be stored in the JSON-RPC request.
       *
       * @since 1.0
       *
       * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
       */
      template <typename TId, typename... TArgs>
      struct Request {
        /**
         * @brief Request parameter
         *
         * Stores the name and type information of a parameter.
         *
         * @tparam U Type of the parameter that shall be added to the request.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename U>
        struct Parameter {
            /**
             * @brief constructor
             *
             * Creates a parameter by storing its name.
             *
             * @param name The name of the parameter.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            inline Parameter(const std::string& name)
              : name(name)
            {}

            /**
             * @brief Store value
             *
             * Stores the value in a JSON object.
             *
             * @param o Reference to the boost JSON object to store in.
             * @param u The value that shall be stored.
             *
             * @note This requires a proper AsJson converter to store the value.
             *
             * @since 1.0
             *
             * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
             */
            void store(boost::json::object& o, const U& u) const {
              o[name] = AsJson<U>(u);
            }

            /// The parameter name
            std::string name;
        };

        /// Type definition of the parameter tuple
        using parameter_tuple_t = std::tuple<Parameter<TArgs>...>;

        /**
         * @brief constructor
         *
         * Constructs the request by its method name and the required parameter of the
         * requests.
         *
         * @tparam UArgs Parameter list of the request.
         *
         * @param method Method name of the request.
         * @param args List of parameter that is required for the request.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename... UArgs>
        Request(
            const std::string& method, UArgs... args)
          : method(method),
            parameter(Parameter<TArgs>(args)...)
        {}

        /**
         * @brief Store arguments
         *
         * Stores the arguments in the request object.
         *
         * @tparam TTuple Tuple with the provided request parameter values.
         * @tparam Is Index sequence to iterate the tuple.
         *
         * @param o Object that shall be used to store the values.
         * @param tuple The tuple that contains the values.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        template <typename TTuple, std::size_t... Is>
        void apply_args(boost::json::object& o, const TTuple& tuple, std::index_sequence<Is...>) {
           ((std::get<Is>(parameter).store(o, std::get<Is>(tuple))), ... );
        }

        /**
         * @brief call operator
         *
         * Provides a function call like behavior to provide the values that
         * shall be stored in a request. This generates the full JSON-RPC request.
         *
         * @param args List of arguments that shall be stored in the request.
         *
         * @return Returns the fully generated JSON-RPC request.
         *
         * @since 1.0
         *
         * @author Tarek Schwarzinger <tarek.schwarzinger@googlemail.com>
         */
        boost::json::object operator()(TArgs... args) {
          boost::json::object request;
          request["jsonrpc"] = "2.0";
          request["id"] = AsJson<typename TId::id_t>(TId::make());
          request["method"] = method;

          std::tuple<TArgs...> argTuple{args...};
          boost::json::object params;
          apply_args(params, argTuple, std::make_index_sequence<std::tuple_size<parameter_tuple_t>::value>{});

          request["params"] = params;
          return request;
        }

        /// Request method name
        std::string method;

        /// List of provided parameter information
        parameter_tuple_t parameter;
      };
    }
  }
}

int main()
{
  // Just have easier access to the required structures
  using namespace ts7::jsonrpc_playground::request;

  // Create the request for later usage
  Request<request_id<int>, int, int, std::string, Vector2d> request = Request<request_id<int>, int, int, std::string, Vector2d>("dummy", "x", "y", "name", "offset");

  // Call the request generation and store it
  boost::json::object reqObj = request(3, 7, "Hallo Welt", Vector2d{1,2});

  // Dump the generated object
  std::cout << reqObj << std::endl;

  return 0;
}
