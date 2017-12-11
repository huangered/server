//
//  server.cpp
//  test
//
//  Created by huang on 2017/12/9.
//  Copyright © 2017年 huang. All rights reserved.
//

#include "server.hpp"
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>

void Server::start() {

  pthread_t tid;

  int sockfd, new_fd;

  sockaddr_in my_addr;
  sockaddr_in their_addr;

  unsigned int sin_size, listnum;

  listnum = 10;
  // create socket
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket in error\n;");
    return;
  }
  my_addr.sin_family = PF_INET;
  my_addr.sin_port = htons(_port);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(my_addr.sin_zero), 0);

  if (bind(sockfd, (sockaddr *)&my_addr, sizeof(sockaddr)) == -1) {
    perror("bind is error\n");
    return;
  }
  if (listen(sockfd, listnum) == -1) {
    perror("listen is error\n");
    return;
  }

  while (true) {
    sin_size = sizeof(sockaddr_in);
    if ((new_fd = accept(sockfd, (sockaddr *)&their_addr, &sin_size)) == -1) {
      perror("accept is error\n");
    } else {
      printf("Server: got conn from %s\n", inet_ntoa(their_addr.sin_addr));
    }
    ThreadArg *t = new ThreadArg;
    t->_fd = new_fd;
    t->_h = _h;
    t->_msgLen = _msgLen;
    pthread_create(&tid, NULL, &Server::run, t);
    pthread_detach(tid);
  }
}
void *Server::run(void *arg) {
  std::cout << "create new thread" << std::endl;
  ThreadArg *t = static_cast<ThreadArg *>(arg);
  handler h = t->_h;

  int client_fd = t->_fd;
  int msg_len = t->_msgLen;
  delete t;

  char *sock_buf = new char[msg_len];

  bzero(sock_buf, msg_len);
  int rval = 0;
  while (true) {
    rval = recv(client_fd, sock_buf, msg_len, 0);
    if (rval > 0) {
      h(sock_buf);
    } else if (rval == 0) {
      close(client_fd);
      printf("close socket\n");
      break;
    } else {
      perror("recv error\n");
      close(client_fd);
      break;
    }
    bzero(sock_buf, msg_len);
  }
  delete[] sock_buf;
  printf("exist thread\n");
  return 0;
}
