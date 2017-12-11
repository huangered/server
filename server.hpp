//
//  server.hpp
//  test
//
//  Created by huang on 2017/12/9.
//  Copyright © 2017年 huang. All rights reserved.
//

#ifndef server_hpp
#define server_hpp

typedef void (*handler)(void *);

struct ThreadArg {
  int _fd;
    int _msgLen;
  handler _h;
};

class Server {
private:
  int _port;
  int _msgLen;
  handler _h;
  handler _error_handler;

  static void *run(void *arg);

public:
  Server(int p, int msgLen, handler h) : _port(p), _msgLen(msgLen), _h(h) {}

  void start();
};

#endif /* server_hpp */
