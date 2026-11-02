#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<time.h>
#include<stdbool.h>
#include "nw.h"

bool isdelim(char c){

return (c==' ' || c==',' || c=='.' || c=='\n');

}

void getword(char input[],char word[]){

 word[0]='\0';
 for (int i =0; input[i] != '\0';i++){
  if (isdelim(input[i]))
     break;
    word[i]= input[i];
   word[i+1] = '\0';
}
}

int process(char input[], int len ,char output[]){
  input[len] = '\0';
  output[0] = '\0';
  char word[100] =" ";
  sscanf(input,"%s",word);
  printf("Debug %d: %s  %s\n", len,input, word);
  if(strcmp(word,"time")==0){
      time_t tm = time(0);
      char timestring[100];
      strftime(timestring, sizeof(timestring),"%H:%M:%S",localtime(&tm));
      sprintf(output + strlen(output),"Time is %s\n",timestring);
  }
  else{
      sprintf(output + strlen(output),"Invalid command: %s\n",word);
  }
}



char rdata[1000];
char wdata[100];
sockaddr_in raddr;

int main(){
 int fd=xsocket(SOCK_STREAM);
 xbind(fd,9999);
 xlisten(fd);

 while(1)
 {
    int cfd=xaccept(fd,&raddr);

    while(1)
    {
        int rdatalen=xrecv(cfd,rdata,sizeof(rdata));
        if(rdatalen<=0)
            break;

        process(rdata,rdatalen,wdata);
        xsend(cfd,wdata,strlen(wdata));
    }

    xclose(cfd);
 }

 xclose(fd);
 return 0;
}
