#include <cxxabi.h>
#include <iostream>

#include "test.h"

int main()
{
  using namespace ts7::jsonrpc_playground::any_collection;
  simple();
  any();
  storage();
  variant();
  variant_storage();

  return 0;
}
