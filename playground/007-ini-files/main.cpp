#include <iostream>

#include "ini-files.h"

int main()
{
  using namespace ts7::jsonrpc_playground::ini;

  v1::simple();
  v2::structured();
  v3::structuredVariant();

  return 0;
}
