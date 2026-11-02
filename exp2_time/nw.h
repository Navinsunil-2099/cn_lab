#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int xsocket(int proto)
{
    int fd = socket(AF_INET, proto, 0);
    if(fd < 0)
    {
        perror("Error");
        exit(1);
    }
    return fd;
}

int xbind(int fd, int port)
{
    sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = 0
    };

    int status = bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    if(status < 0)
    {
        perror("Error");
        exit(1);
    }

    return status;
}

int xlisten(int fd)
{
    int status = listen(fd, 5);

    if(status < 0)
    {
        perror("Error");
        exit(1);
    }

    return status;
}

int xaccept(int fd, sockaddr_in *addr)
{
    socklen_t socklen = sizeof(sockaddr_in);

    int c = accept(fd, (sockaddr *)addr, &socklen);

    if(c < 0)
    {
        perror("Error");
        return -1;
    }

    return c;
}

int xconnect(int fd, sockaddr_in *addr)
{
    int status = connect(fd, (sockaddr *)addr, sizeof(sockaddr_in));

    if(status < 0)
    {
        perror("Error");
        exit(1);
    }

    return status;
}

int xrecv(int fd, char data[], int datamax)
{
    int len = recv(fd, data, datamax, 0);

    if(len < 0)
    {
        perror("Error");
        exit(1);
    }

    printf("recv:%d\n", len);

    return len;
}

int xsend(int fd, char data[], int datalen)
{
    int len = send(fd, data, datalen, 0);

    if(len < 0)
    {
        perror("Error");
        exit(1);
    }

    return len;
}

int xclose(int fd)
{
    return close(fd);
}
