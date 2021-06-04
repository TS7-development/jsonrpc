#include "test.h"

void simple() {
  std::cout << "Simple print" << std::endl;
  print<A>();
  print<B>();
  print<C<int>>();
  print<C<double*>>();
  print<C<A>>();
  std::cout << std::endl;
}
