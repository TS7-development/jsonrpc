#include "test.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

#include <boost/any.hpp>

struct Storage {
    using print_t = std::function<void(const boost::any&)>;

    struct node_t {
        std::size_t type_hash;
        boost::any value;
    };

    template <typename U>
    void add(const std::string& name, U&& u) {
      elements[name] = node_t{typeid(U).hash_code(), std::forward<U>(u)};
      types[typeid(U).hash_code()] = [](const boost::any& a) {
        boost::any_cast<U>(a).print();
      };
    }

    void print(const std::string& name) {
      const node_t& node = elements[name];
      print_t p = types[node.type_hash];
      p(node.value);
    }

    std::map<std::string, node_t> elements;
    std::map<std::size_t, print_t> types;
};

void storage() {
  std::cout << "Storage print" << std::endl;

  Storage s;
  s.add("ConfigurationSend", A());
  s.add("ConfigurationRead", B());
  s.add("ConfigurationChecksum", C<int>());
  s.add("ConfigurationTime", C<double>());
  s.add("ConfigurationInvalidate", A());

  s.print("ConfigurationSend");
  s.print("ConfigurationChecksum");

  std::cout << std::endl;
}
