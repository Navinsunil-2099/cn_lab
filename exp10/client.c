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
  
  for(;;){
    char* p = fgets(wdata,sizeof(wdata),stdin);
    if(p == NULL){
      break;
    }
    xputs(wdata,cid);
  }
  // return 0;
}

void* mythread(void* arg) {
  int cid = (int)(intptr_t)arg;
  char rline[RMAX + 1];

  for(;;){
    int nread = xgets(rline,sizeof(rline) - 1, cid);
    if(nread <= 0){
      break;
    }

    rline[nread] = '\0';
    fputs(rline,stdout);
    fflush(stdout);

  }
  return 0;
}

int main(int argc,char* argv[]) {
  int cid = 0;
  conn[cid].fd = xsocket(SOCK_STREAM);
  conn[cid].addr = xcreateaddr("127.0.0.1",9999);
  xconnect(conn[cid].fd,&conn[cid].addr);
  while(1){
      pthread_create(&conn[cid].thread, NULL, mythread, (void*)(intptr_t)cid);

      processmain(cid);
      close(conn[cid].fd);
      conn[cid].fd = -1;
  }
  return 0;
}
