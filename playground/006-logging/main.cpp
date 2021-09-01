#include <iostream>

#include "logging.h"

int main()
{
  using namespace ts7::jsonrpc_playground::logging;

  trivial();
  trivialFilter();
  sink();
  scopes();

  return 0;
}
