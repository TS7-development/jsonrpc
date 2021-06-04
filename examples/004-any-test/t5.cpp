#include "test.h"

#include <map>
#include <string>

#include <boost/variant.hpp>

using variant_type = boost::variant<A, B, C<int>, C<double>, C<A>>;

struct variant_visitor : public boost::static_visitor<> {
    template<typename  T>
    void operator()(const T& t) const {
      t.print();
    }
};

struct VariantStorage {
    template <typename T>
    void add(const std::string& name, T&& t) {
      callbacks[name] = std::forward<T>(t);
    }

    void print(const std::string& name) {
      boost::apply_visitor(variant_visitor(), callbacks[name]);
    }

    std::map<std::string, variant_type> callbacks;
};

void variant_storage() {
  std::cout << "VariantStorage print" << std::endl;

  VariantStorage vs;
  vs.add("ConfigurationSend", A());
  vs.add("ConfigurationRead", B());
  vs.add("ConfigurationChecksum", C<int>());

  vs.print("ConfigurationSend");
  vs.print("ConfigurationRead");
  vs.print("ConfigurationChecksum");

  std::cout << std::endl;
}
