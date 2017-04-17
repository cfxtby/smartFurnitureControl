#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define UDPGrantAddress "192.168.43.255"
#define UDPGrantCount 3
#define UDPGrantBeginNum 89
#define UDPGrantEndNum 86

int main(){
	int so_broadcast=1;
	int scanSocket= socket(AF_INET, SOCK_DGRAM, 0);//申请一个udp网络套接字
	setsockopt(scanSocket,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));
	if (scanSocket != -1) {
        printf("Create a socket with fd: %d\n", scanSocket);
    } else {
        fprintf(stderr, "Fail to create a socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in localAddr;//声明地址
    
    localAddr.sin_family = AF_INET;//ipv4
    localAddr.sin_port = htons(48899);//绑定本地端口
    localAddr.sin_addr.s_addr = inet_addr(INADDR_ANY);//本地地址绑定
    memset(localAddr.sin_zero, '\0', sizeof localAddr.sin_zero);
    if (bind(scanSocket, (const struct sockaddr *) &localAddr, sizeof localAddr) == 0) {
        printf("Bind socket to 48899   hello\n");
    } else {
        fprintf(stderr, "Fail to bind the socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
   
    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(48899);
    srv_addr.sin_addr.s_addr = inet_addr("192.168.93.255");//"192.168.43.255");
    memset(srv_addr.sin_zero, '\0', sizeof srv_addr.sin_zero);
	
    char buf1[]="YZ-RECOSCAN\0";
    printf("%s\n",buf1);
    //fgets(buf, 512, stdin);
    
    //buf[strlen(buf) - 1] = 0;
    int srv_len=sizeof(srv_addr);
    ssize_t send_size = sendto(scanSocket, buf1, strlen(buf1), 0,
                               (const struct sockaddr *) &srv_addr,
                               sizeof srv_addr);




    /*scan.port=ScanDstPort;//
    scan.addr.sin_family = AF_INET;
    scan.addr.sin_port = htons(48899);
    scan.addr.sin_addr.s_addr = htonl("192.168.43.255");//UDPGrantAddress);
    memset(scan.addr.sin_zero, '\0', sizeof scan.addr.sin_zero);
    char ins[]=SCANINS;
    printf("%s\n",ins);
	ssize_t send_size = sendto(scanSocket, ins,strlen(ins), 0,
                               (const struct sockaddr *) &(scan.addr),
                               sizeof scan.addr);*/
	if(send_size>0){
		printf("%s\n",buf1);
	}
	else{
		printf("%d  %s\n",send_size,buf1 );
	}
	
	char buf[512];
	struct sockaddr_in client_addr;
	int client_addrlen=sizeof(client_addr);
    bzero(&client_addr, sizeof(struct sockaddr_in));  
    client_addr.sin_family = AF_INET;  
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    client_addr.sin_port = htons(48899);  
	ssize_t recv_size =
	    recvfrom(scanSocket, buf, 512, 0, (struct sockaddr *) &client_addr,
	             &client_addr);
	printf("%s\n",buf);
	close(scanSocket);
}