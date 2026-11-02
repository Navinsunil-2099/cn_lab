#include<stdio.h>
#include <ctype.h>

int main() {
    int N;
    char row[100];
    printf("ENTER THE ORDER \n");
    fgets(row, 100, stdin);
    sscanf(row, "%d", &N);

    int a [N][N];

    for (int i = 0; i < N; i++) {
        printf("ENTER THE ROW\n");
        fgets(row, sizeof(row), stdin);
        char *p=row;
        int count=0;
        while (count<N)
        {
          sscanf(p, "%d", &a[i][count]);
          ;
          while(isdigit(*p))
          {
              p++;
          }
          while(*p==' ')
          {
              p++;
          }
          count++;
        }


    }
    int u=1,l=1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          if(i<j && a[i][j]!=0)
            l=0;
          if(i>j && a[i][j]!=0)
            u=0;
        }
    }
     if (u && l)
        printf("DIAGONAL");
    else if (u)
        printf("UPPER TRIANGULAR");
    else if (l)
        printf("LOWER TRIANGULAR");
    else
        printf("NONE");
    return 0;

 }
