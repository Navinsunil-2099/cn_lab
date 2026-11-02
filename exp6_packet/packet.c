//Exp 10: Develop a packet-capturing application using raw sockets.

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <unistd.h>
#include <stdlib.h>

unsigned char rdata[64*1024];

void print(const char* mesg, unsigned char* data, int begin, int end){
	printf("%s: ", mesg);
	for (int i = begin; i <= end; i++) {
		printf("%02x ", data[i]);
	}
	printf("\n");
}

void process(unsigned char* data, int len){
	printf("Len: %d\n", len);
        print("Eth to" , data, 0, 5);
        print("Eth from" , data, 6, 11);
        print("Eth Datatype" , data, 12, 13);
	if(data[12]==8 && data[13]==0){
		print("IP to" , data, 26, 29);
		print("IP from" , data, 30, 33);
		print("IP Datatype", data, 23, 23);
	}
	fflush(stdout);
}

int main() {
    int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // see man packet
    if (fd < 0) { perror("Error"); exit(1); }
    while (1) {
        int len = recv(fd, rdata, sizeof(rdata), 0);
        if (len < 0) { perror("Error"); exit(1); }
        process(rdata, len);
    }
    close(fd);
    return 0;
}

// To test: 
// bash$ unshare -r -n bash
// bash$ ip link set lo up
// bash$ gcc packet.c
// bash$ ./a.out | tee log &
// bash$ ping -c 3 127.0.0.1
// bash$ ./a.out | tee log &
// bash$ python3 -m http.server &
// bash$ wget http://localhost:8000/test.txt
// WAR:
//    /etc/sysctl.d/unshare.conf 
//        kernel.unprivileged_userns_clone=1
//        kernel.apparmor_restrict_unprivileged_userns=0
//

