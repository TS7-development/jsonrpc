#pragma once

#include <cxxabi.h>
#include <iostream>

struct A {
    void print() const {
      std::cout << "A" << std::endl;
    }
};

struct B {
    void print() const {
      std::cout << "B" << std::endl;
    }
};

template <typename T>
char* type_name() {
  return abi::__cxa_demangle(typeid (T).name(), 0, 0, nullptr);
}

template <typename T>
struct C {
    void print() const {
      std::cout << "C<" << type_name<T>() << ">" << std::endl;
    }
};

template <typename T>
void print() {
  T t;
  t.print();
}

void simple();
void any();
void storage();
void variant();
void variant_storage();
