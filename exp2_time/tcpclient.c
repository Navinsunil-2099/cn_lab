#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include "nw.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

char rdata[1000];
char wdata[100];
sockaddr_in raddr;

int main()
{
    raddr = (sockaddr_in){
        .sin_family = AF_INET,
        .sin_port = htons(9999),
        .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    int fd = xsocket(SOCK_STREAM);

    xconnect(fd, &raddr);

    while(1)
    {
        fgets(wdata, sizeof(wdata), stdin);

        xsend(fd, wdata, strlen(wdata));

        int rdatalen = xrecv(fd, rdata, sizeof(rdata));

        if(rdatalen <= 0)
            break;

        rdata[rdatalen] = '\0';

        printf("Recvd: %s\n", rdata);
    }

    xclose(fd);
    return 0;
}
