#include <iostream>

#include <callfromjson.hpp>
#include <procedure.hpp>
#include <request.hpp>

int testFunc(int a, int b) {
  std::cout << a << " + " << b << " = " << (a+b) << std::endl;
  return a + b;
}

int main()
{ 

  ts7::jsonrpc::Request<int> request = ts7::jsonrpc::Request<int>::Create(
    "testFunc",
    1,
    "a", 3,
    "b", 7
  );

  auto p = ts7::jsonrpc::make_request_handler<int>(ts7::jsonrpc::CallFromJson<int, int, int>(testFunc, "a", "b"));
//  ts7::jsonrpc::Procedure<int, ts7::jsonrpc::CallFromJson<int, int, int>> p(
//    ts7::jsonrpc::CallFromJson<int, int, int>(testFunc, "a", "b")
//  );

  std::cout << "<- " << request << std::endl;
  auto c = p.onRequest(request);
  std::cout << "-> " << c << std::endl;

  return 0;
}
