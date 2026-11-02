#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "nw.h"

#define RMAX 1000
#define WMAX 2000

int main()
{
    int fd;
    sockaddr_in server_addr;
    char filename[RMAX];
    char buffer[WMAX + 1];
    int n;

    fd = xsocket(SOCK_STREAM);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(fd, (sockaddr *)&server_addr,
               sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        xclose(fd);
        exit(1);
    }

    printf("Connected to server\n");

    printf("Enter filename: ");
    scanf("%999s", filename);

    xsend(fd, filename, strlen(filename));

    while((n = xrecv(fd, buffer, WMAX)) > 0)
    {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    xclose(fd);

    return 0;
}
