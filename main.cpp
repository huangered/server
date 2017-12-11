//
//  main.cpp
//  test
//
//  Created by huang on 2017/11/26.
//  Copyright © 2017年 huang. All rights reserved.
//
#include "server.hpp"
#include <iostream>

void test(void *p) {
  char *p1 = static_cast<char *>(p);
  std::cout << p << std::endl;
  printf("data: %s\n", p1);
}

int main(int argc, const char *argv[]) {
  // insert code here...
  Server *server = new Server(12345, 1024, test);
  server->start();

  while (true) {
  }
  return 0;
}
