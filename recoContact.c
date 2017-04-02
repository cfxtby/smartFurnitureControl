#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "recoInfo.h"

#define UDPGrantAddress "192.168.1.255"
#define UDPGrantCount 3
#define UDPGrantBeginNum 89
#define UDPGrantEndNum 86
struct UDPGrantInfo UGI;
int grantsocket,retimes=0;
int RecoGrant(int[] args){

	grantsocket= socket(AF_INET, SOCK_DGRAM, 0);//申请一个udp网络套接字
	if (grantsocket != -1) {
        printf("Create a socket with fd: %d\n", grantsocket);
    } else {
        fprintf(stderr, "Fail to create a socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in localAddr;//声明地址
    localAddr.sin_family = AF_INET;//ipv4
    localAddr.sin_port = htons(grantLocalPort);//绑定本地端口
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);//本地地址绑定
    memset(localAddr.sin_zero, '\0', sizeof localAddr.sin_zero);
     if (bind(grantsocket, (const struct sockaddr *) &localAddr, sizeof localAddr) == 0) {
        printf("Bind socket to 48899\n");
    } else {
        fprintf(stderr, "Fail to bind the socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
	SIGNAL(ALARM,Grant);	//闹钟警报
	

	UGI.port=grantLocalPort;//
    UGI.addr.sin_family = AF_INET;
    UGI.addr.sin_port = htons(grantDstPort);
    UGI.addr.sin_addr.s_addr = htonl(UDPGrantAddress);
    memset(UGI.addr.sin_zero, '\0', sizeof UGI.addr.sin_zero);
    //此处需要计算得到密码，与密码之后的结束值，即之前的340
	Grant();
    char buf[512];
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
	memset(buf, 0, sizeof buf);
	while(1){
		int time=0;
	ssize_t recv_size =
	    recvfrom(grantsocket, buf, 512, 0, (struct sockaddr *) &client_addr,
	             &client_addrlen);
	    if(recv_size==25){
	    	time++;
	    }
	    if(time>=3){
	    	ALARM(0);
			close(grantsocket);
	    	return 0;
	    }
	    if(retimes>=10){ 
	    	ALARM(0);
			close(grantsocket);
	    	return -1;
		}	
	}
}

void Grant(){

	int[] secret={4,0,8,2,4,0,8,2};
    int end=340;
    int i,j=0;
    int num76=204;
	while(j<5){
	    for(i=0;i<num76;i++){
	    	UDPSend(grantsocket,UGI,76);
	    }
	    for(i=0;i<UDPGrantCount;i++){
	    	UDPSend(grantsocket,UGI,UDPGrantBeginNum);
	    }

	    int serectLen=sizeof(secret)/sizeof(int);
	    for(i=0;i<serectLen;i++){
	    	UDPSend(grantsocket,UGI,secret[i]);
	    }

		for(i=0;i<UDPGrantCount;i++){
	    	UDPSend(grantsocket,UGI,UDPGrantEndNum);
	    }
		for(i=0;i<UDPGrantCount;i++){
	    	UDPSend(grantsocket,UGI,end);
	    }
	    j++;
	}
	retimes++;
	ALARM(1);
}
//建立函数进行发送UDP报文，参数分别为socket，udp传输信息，长度len，即传输信息
int UDPSend(int grantsocket,struct UDPGrantInfo UGI,int len){
	char* data;
	data=(char*)malloc(len);
	memset(data, UGI.data, len);
	ssize_t send_size = sendto(grantsocket, data, len, 0,
                               (const struct sockaddr *) &(UGI.addr),
                               sizeof UGI.addr);
	if(send_size>0)return 0;
	else return -1;
}

