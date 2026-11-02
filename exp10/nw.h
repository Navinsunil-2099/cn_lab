#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
int xsocket(int type) {
  int fd = socket(AF_INET, type, 0);
  if (fd < 0) { perror("Error"); exit(1); }
  return fd;
}
void xbind(int fd, int port) {
  sockaddr_in addr = {
      .sin_family = AF_INET, .sin_port = htons(port), .sin_addr.s_addr = 0};
  int status = bind(fd, (const sockaddr*)&addr, sizeof(addr));
  if (status < 0) { perror("Error"); exit(1); }
}

int xrecvfrom(int fd, char data[], int size, sockaddr_in* addr) {
  socklen_t socklen = sizeof(sockaddr_in);
  int len = recvfrom(fd, data, size, 0, (sockaddr*)addr, &socklen);
  if (len < 0) { perror("Error"); exit(1); }

  return len;
}
int xsendto(int fd, char data[], int size, sockaddr_in* addr) {
  int len = sendto(fd, data, size, 0, (const sockaddr*)addr, sizeof(sockaddr_in));
  if (len < 0) { perror("Error"); exit(1); }
  return len;
}

sockaddr_in xcreateaddr(const char* ip, int port) {
  sockaddr_in ret = {.sin_family = AF_INET,
                     .sin_port = htons(port),
                     .sin_addr.s_addr = inet_addr(ip)};
  return ret;
}

int xaccept(int fd, sockaddr_in* addr) {
  socklen_t socklen = sizeof(sockaddr_in);
  int c = accept(fd, (sockaddr*)addr, &socklen);
  return c;
}

void xnonblock(int fd) {
  int fl = fcntl(fd, F_GETFL, 0);
  if (fl < 0) { perror("Error"); exit(1); }
  int status = fcntl(fd, F_SETFL, fl | O_NONBLOCK);
  if (status < 0) { perror("Error"); exit(1); }
}

void xreuse(int fd) {
  int tmp = 1;
  int status = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp));
  if (status < 0) { perror("Error"); exit(1); }
}

void xconnect(int fd, sockaddr_in* addr) {
  int status = connect(fd, (const sockaddr*)addr, sizeof(sockaddr_in));
  if (status < 0) { perror("Error"); exit(1); }
}

