#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "nw.h"

void process(int matrix[][100], int n, char result[])
{
    int upper = 1;
    int lower = 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            
            if (i > j && matrix[i][j] != 0)
                upper = 0;

            
            if (i < j && matrix[i][j] != 0)
                lower = 0;
        }
    }

    if (upper && lower)
        strcpy(result, "Diagonal Matrix");
    else if (upper)
        strcpy(result, "Upper Triangular Matrix");
    else if (lower)
        strcpy(result, "Lower Triangular Matrix");
    else
        strcpy(result, "Neither Upper nor Lower Triangular Matrix");
}


typedef struct Connection Connection;

struct Connection{
	int fd;
	sockaddr_in addr;
	int matrixn;
	int matrix[100][100];
};

Connection conn[10];

int xgets(char* data, int size, int cid){
	return recv(conn[cid].fd, data, size,0);
}

int xputs(const char* data, int cid){
	return send(conn[cid].fd, data, strlen(data), 0);
}

int processmain(int cid){
	int n;
    int matrix[100][100];
    char line[256];
    char result[100];

   
    xputs("Enter N: ",cid);
    xgets(line, sizeof(line), cid);
    sscanf(line, "%d", &n);

   
    xputs("Enter matrix:\n",cid);

    for (int i = 0; i < n; i++)
    {
        xgets(line, sizeof(line), cid);

        char *ptr = line;

        for (int j = 0; j < n; j++)
        {
            sscanf(ptr, "%d", &matrix[i][j]);

            while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n')
                ptr++;

            while (*ptr == ' ')
                ptr++;
        }
    }

    process(matrix, n, result);

    xputs(result,cid);

    return 0;
}

// int main(){
// 	int fd = xsocket(SOCK_STREAM);
// 	xbind(fd, 9999);
// 	listen(fd, 10);
// 	for(;;){
// 		int cid = 0;
// 		conn[cid].fd = xaccept(fd, &conn[cid].addr);
// 		printf("connected\n");
//
// 		processmain(cid);
//
// 		close(conn[cid].fd);
// 		printf("disconnected\n");
// 	}
// }

int main()
{
    int fd = xsocket(SOCK_STREAM);

    xbind(fd, 9999);
    xlisten(fd);

    printf("Server listening on port 9999...\n");

    while (1)
    {
        int cid = 0;

        conn[cid].fd = xaccept(fd, &conn[cid].addr);

        if (conn[cid].fd < 0)
            continue;

        printf("Client connected\n");

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            close(conn[cid].fd);
            continue;
        }

        if (pid == 0)
        {
            close(fd);

            processmain(cid);

            close(conn[cid].fd);

            printf("Client disconnected\n");

            exit(0);
        }
        else
        {
            close(conn[cid].fd);
        }
    }

    close(fd);

    return 0;
}

// int main()
// {
//     int fd = xsocket(SOCK_STREAM);
//
//     xbind(fd, 9999);
//
//     xlisten(fd);
//
//     while(1)
//     {
//         int cfd = xaccept(fd);
//
//         while(1)
//         {
//             char rdata[1000];
//             char wdata[1000];
//
//             int rdatalen = xrecv(cfd, rdata, sizeof(rdata));
//
//             if(rdatalen <= 0)
//                 break;
//
//             process(rdata, rdatalen, wdata);
//
//             printf("Output:%s\n", wdata);
//
//             xsend(cfd, wdata, strlen(wdata));
//         }
//
//         xclose(cfd);
//     }
//
//     xclose(fd);
// }
