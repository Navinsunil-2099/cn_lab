#include <pthread.h>
#include <stdint.h>
#include "nw.h"

#define RMAX 1000
#define WMAX 2000
#define CMAX 100

typedef struct Connection Connection;
struct Connection {
  int fd;
  sockaddr_in addr;
  pthread_t thread;
};
Connection conn[CMAX];
int nconn = 0;

int xgets(char* data, int size, int cid) {
  return recv(conn[cid].fd, data, size, 0);
}

int xputs(const char* data, int cid) {
  return send(conn[cid].fd, data, strlen(data), 0);
}

int processmain(int cid) {
  char rline[RMAX +1];
  char wdata[WMAX + 1];
  
  xputs("hello welcome to the chat\n",cid);
  
  for(;;){
    int nread = xgets(rline,RMAX,cid);
    if(nread <= 0){
      return -1;
    }
    rline[nread] = '\0';

    sprintf(wdata,"Client %d: %s",cid,rline);
    for(int i = 0;i < nconn; i++){
      if(i != cid && conn[i].fd >= 1){
        xputs(wdata,i);
      }
    }
  }

  return 0;
}

void* mythread(void* arg) {
  int cid = (int)(intptr_t)arg;

  processmain(cid);
  close(conn[cid].fd);
  conn[cid].fd = -1;

  printf("Client %d disconnected\n", cid);
  return NULL;
}

int main() {
  int fd = xsocket(SOCK_STREAM);
  xreuse(fd);
  xbind(fd, 9999);
  listen(fd, 10);

  for (;;) {
    sockaddr_in addr;
    int c = xaccept(fd, &addr);
    if (nconn == CMAX - 1) {
      return -1;
    }
    int cid = nconn++;
    conn[cid].fd = c;
    conn[cid].addr = addr;
    printf("Client %d connected", cid);

    pthread_create(&conn[cid].thread, NULL, mythread, (void*)(intptr_t)cid);
    printf("Client %d disconnected\n", cid); 
  }
  return 0;
}
