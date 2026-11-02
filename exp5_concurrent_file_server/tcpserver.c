#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "nw.h"

#define RMAX 1000
#define WMAX 2000
#define CMAX 100

typedef struct Connection Connection;
struct Connection {
  int fd;
  sockaddr_in addr;
  pthread_t thread_id;
};
Connection conn[CMAX];

int xgets(char* data, int size, int cid) {
  return recv(conn[cid].fd, data, size, 0);
}

int xputs(const char* data, int cid) {
  return send(conn[cid].fd, data, strlen(data), 0);
}

// Input:
//   Filename
// Output:
//   PID SUCCESS/ERROR Filename
//   Contents of size bytes
int processmain(int cid) {
  char rline[RMAX + 1];
  char wdata[WMAX + 1];
  char filename[100];
  int pid = getpid();

  // read filename
  int nread = xgets(rline, RMAX, cid);
  if (nread <= 0)
    return -1;
  rline[nread] = '\0';
  if (sscanf(rline, "%99s", filename) < 1) {
    return -1;
  }

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    sprintf(wdata, "%d ERROR %s\n", pid, filename);
    xputs(wdata, cid);
    return -1;
  }

  sprintf(wdata, "%d SUCCESS %s\n", pid, filename);
  xputs(wdata, cid);

  while (fgets(rline, sizeof(rline), file) != NULL) {
    xputs(rline, cid);
  }
  fclose(file);

  return 0;
}

void* mythread(void* arg) {
  int cid = (int)(intptr_t)arg;

  processmain(cid);
  close(conn[cid].fd);
  conn[cid].fd = -1;

  printf("Client %d disconnected\n", cid);
  pthread_exit(NULL);
}

int findvalid() {
  for (int i = 0; i < CMAX; i++) {
    if (conn[i].fd <= 0) {
      return i;
    }
  }
  return -1;
}

int main() {
  int fd = xsocket(SOCK_STREAM);
  xbind(fd, 9999);
  listen(fd, 10);

  for (;;) {
    int cid = -1;
    while ((cid = findvalid()) < 0) {
      usleep(10000);
    }

    conn[cid].fd = xaccept(fd, &conn[cid].addr);
    printf("Client %d connected\n", cid);

    pthread_create(&conn[cid].thread_id, NULL, mythread, (void*)(intptr_t)cid);
    pthread_detach(conn[cid].thread_id);
  }
  return 0;
}
