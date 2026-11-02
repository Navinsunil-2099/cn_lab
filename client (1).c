#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

typedef struct sockaddr_in sockaddr_in;

int xsocket(int proto)
{
    int fd = socket(AF_INET, proto, 0);
    if(fd < 0) { perror("Error"); exit(1); }
    return fd;
}

sockaddr_in createaddr(char ip[], int port)
{
    sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };

    inet_pton(AF_INET, ip, &addr.sin_addr);

    return addr;
}

int xconnect(int fd, sockaddr_in addr)
{
    int status = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(status < 0) { perror("Error"); exit(1); }

    return status;
}

int xsend(int fd, char data[], int datalen)
{
    int len = send(fd, data, datalen, 0);
    if(len < 0) { perror("Error"); exit(1); }

    return len;
}

int xrecv(int fd, char data[], int datamax)
{
    int len = recv(fd, data, datamax, 0);
    if(len < 0) { perror("Error"); exit(1); }

    return len;
}

int xclose(int fd)
{
    return close(fd);
}

// int main()
// {
//     int fd = xsocket(SOCK_STREAM);
//
//     sockaddr_in saddr = createaddr("127.0.0.1", 9999);
//
//     xconnect(fd, saddr);
//
//     while(1)
//     {
//         char wdata[1000];
//         char rdata[1000];
//
//         printf("Enter command: ");
//
//         fgets(wdata, sizeof(wdata), stdin);
//
//         xsend(fd, wdata, strlen(wdata));
//
//         int rdatalen = xrecv(fd, rdata, sizeof(rdata));
//
//         if(rdatalen <= 0)
//             break;
//
//         rdata[rdatalen] = '\0';
//
//         printf("recvd: %s\n", rdata);
//     }
//
//     xclose(fd);
// } 

int main()
{
    int fd = xsocket(SOCK_STREAM);

    sockaddr_in saddr = createaddr("127.0.0.1", 9999);

    xconnect(fd, saddr);

    char wdata[1000];
    char rdata[1000];
    int n;

    /* Receive "Enter N:" */
    int rdatalen = xrecv(fd, rdata, sizeof(rdata) - 1);

    if (rdatalen <= 0)
        return 0;

    rdata[rdatalen] = '\0';
    printf("%s", rdata);

    /* Enter N */
    fgets(wdata, sizeof(wdata), stdin);
    xsend(fd, wdata, strlen(wdata));

    sscanf(wdata, "%d", &n);

    /* Receive "Enter matrix:" */
    rdatalen = xrecv(fd, rdata, sizeof(rdata) - 1);

    if (rdatalen <= 0)
        return 0;

    rdata[rdatalen] = '\0';
    printf("%s", rdata);

    /* Enter matrix rows */
    for (int i = 0; i < n; i++)
    {
        printf("Row %d: ", i + 1);

        fgets(wdata, sizeof(wdata), stdin);

        xsend(fd, wdata, strlen(wdata));
    }

    /* Receive result */
    rdatalen = xrecv(fd, rdata, sizeof(rdata) - 1);

    if (rdatalen <= 0)
        return 0;

    rdata[rdatalen] = '\0';

    printf("Result: %s\n", rdata);

    xclose(fd);

    return 0;
}
