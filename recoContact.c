#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "recoInfo.h"
#include "equipment.h"

#define UDPGrantAddress "192.168.253.255"
#define UDPGrantCount 3
#define UDPGrantBeginNum 89
#define UDPGrantEndNum 86
int recordReco(char buf[],struct smartSwitch *ss,struct smartSwitch *head);

void Grant(){

	int secret[]={4,0,8,2,4,0,8,2};
    int end=340;
    int i,j=0;
    int num76=204;
	while(j<20){
	    for(i=0;i<num76;i++){
	    	usleep(10000);
	    	UDPSend(grantsocket,76);
	    }
	    
	    printf("76\n");
	    for(i=0;i<UDPGrantCount;i++){
	    	usleep(10000);
	    	UDPSend(grantsocket,UDPGrantBeginNum);
	    }
	    printf("89\n");
        
	    int serectLen=sizeof(secret)/sizeof(int);
	    for(i=0;i<serectLen;i++){
	    	usleep(10000);
	    	UDPSend(grantsocket,secret[i]+124);
	    }
	    printf("mima\n");
       
		for(i=0;i<UDPGrantCount;i++){
			usleep(10000);
	    	UDPSend(grantsocket,UDPGrantEndNum);
	    }
	    printf("86\n");
		for(i=0;i<UDPGrantCount;i++){
			usleep(10000);
	    	UDPSend(grantsocket,end);
	    }
	    printf("340\n");
	    j++;
	}
	retimes++;
	alarm(1);
}

int RecoScan1(struct smartSwitch *head,char addr[]){
    struct UDPScanInfo scan;
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
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);//本地地址绑定
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
    srv_addr.sin_addr.s_addr = inet_addr("192.168.253.255");//"192.168.43.255");
    memset(srv_addr.sin_zero, '\0', sizeof srv_addr.sin_zero);
    
    char buf1[]=SCANINS;
    printf("%s\n",buf1);
    //fgets(buf, 512, stdin);
    
    //buf[strlen(buf) - 1] = 0;
    //int add=sizeof(scan.addr);
    ssize_t send_size = sendto(scanSocket, buf1, strlen(buf1), 0,
                               (const struct sockaddr *) &(srv_addr),
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
    recv_size =
        recvfrom(scanSocket, buf, 512, 0, (struct sockaddr *) &client_addr,
                 &client_addr);
    printf("%s\n",buf);
    close(scanSocket);
}


int RecoScan(struct smartSwitch *head,char addr[]){
	struct UDPScanInfo scan;
	int so_broadcast=1;
	int scanSocket= socket(AF_INET, SOCK_DGRAM, 0);//申请一个udp网络套接字
	setsockopt(scanSocket,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));//设置允许广播
	if (scanSocket != -1) {
        printf("Create a socket with fd: %d\n", scanSocket);
    } else {
        fprintf(stderr, "Fail to create a socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in localAddr;//声明地址
    scan.port=ScanDstPort;
    localAddr.sin_family = AF_INET;//ipv4
    localAddr.sin_port = htons(48899);//绑定本地端口
    localAddr.sin_addr.s_addr =  htonl(INADDR_ANY);//本地地址绑定
    memset(localAddr.sin_zero, '\0', sizeof localAddr.sin_zero);
    if (bind(scanSocket, (const struct sockaddr *) &localAddr, sizeof localAddr) == 0) {
        printf("Bind socket to 48899   hello\n");
    } else {
        fprintf(stderr, "Fail to bind the socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("%s\n",addr );
    scan.addr.sin_family = AF_INET;
    scan.addr.sin_port = htons(48899);
    scan.addr.sin_addr.s_addr =  inet_addr(addr);//UDPGrantAddress);
    memset(scan.addr.sin_zero, '\0', sizeof scan.addr.sin_zero);
    char ins[]=SCANINS;
    printf("sending:%s\n",ins);
    ssize_t send_size;
    for (int i=0;i<3;i++){
	send_size = sendto(scanSocket, ins,strlen(ins), 0,
                               (const struct sockaddr *) &(scan.addr),
                               sizeof scan.addr);
	}
	if(send_size>0){
		printf("send:ins%s\n",ins);
	}
	else{
		printf("%d  %s\n",send_size,ins );
	}
	
	char buf[512];
	struct sockaddr_in client_addr;
	int client_addrlen=sizeof(client_addr);
    bzero(&client_addr, sizeof(struct sockaddr_in));  
    client_addr.sin_family = AF_INET;  
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    client_addr.sin_port = htons(48899);
    int time=0;
    struct smartSwitch *p=head,*q;
    struct timeval tv_out;
    tv_out.tv_sec = 3;//等待10秒
    tv_out.tv_usec = 0;
    setsockopt(scanSocket,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));
    while(1){
		ssize_t recv_size =
			recvfrom(scanSocket, buf, 512, 0, (struct sockaddr *) &client_addr,
					 &client_addr);
		if(recv_size==-1&&errno==EAGAIN)
			break;
		q=(struct smartSwitch *)malloc(sizeof(struct smartSwitch));
		printf("%s\n",buf);
		if(time==0){
			if(recordReco(buf,p,head)==0){
				time=1;
			}
		}
		else{
			if(recordReco(buf,q,head)==0){
				p->next=q;
				p=q;
			}
		}
	//memset(buf,"\0",512);
	}
	close(scanSocket);
    return 1;
}



int RecoGrant(){
    int so_broadcast=1;
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
    setsockopt(grantsocket,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));//设置允许广播
    if (bind(grantsocket, (const struct sockaddr *) &localAddr, sizeof localAddr) == 0) {
        printf("Bind socket to 49999\n");
    } else {
        fprintf(stderr, "Fail to bind the socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
	//signal(SIGALRM,Grant);	//闹钟警报
	pthread_t id;
　　int i,ret;
　　ret=pthread_create(&id,NULL,(void *) Grant,NULL); // 成功返回0，错误返回错误编号
　　if(ret!=0) {
　　　　printf ("Create pthread error!\n");
　　　　exit (1);
　　}
	
	UGI.port=grantLocalPort;//
    UGI.addr.sin_family = AF_INET;
    UGI.addr.sin_port = htons(grantDstPort);
    UGI.addr.sin_addr.s_addr = inet_addr(UDPGrantAddress);
    memset(UGI.addr.sin_zero, '\0', sizeof UGI.addr.sin_zero);
    //此处需要计算得到密码，与密码之后的结束值，即之前的340
	Grant();
    char buf[512];
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
	memset(buf, 0, sizeof buf);
	int time=0;
	struct timeval tv_out;
	tv_out.tv_sec = 10;//等待10秒
    tv_out.tv_usec = 0;
	setsockopt(grantsocket,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));
	while(1){
	    ssize_t recv_size =
	    recvfrom(grantsocket, buf, 512, 0, (struct sockaddr *) &client_addr,
	             &client_addrlen);
		if(recv_size==-1&&errno==EAGAIN){
			pthread_exit(ret);
			close(grantsocket);
			return -1;
		}
	    if(recv_size==25){
	    	time++;
	    	printf("shoudao 25\n");
	    }
	    if(time>=5){
			pthread_exit(ret);
			close(grantsocket);
	    	return 0;
	    }
	}
}


//建立函数进行发送UDP报文，参数分别为socket，udp传输信息，长度len，即传输信息
int UDPSend(int grantsocket,int len){
	char* data;
	data=(char*)malloc(len);
	memset(data, 5, len);
	ssize_t send_size = sendto(grantsocket, data, len, 0,
                               (const struct sockaddr *) &(UGI.addr),
                               sizeof UGI.addr);
	if(send_size>0)return 0;
	else return -1;
}


int recordReco(char buf[],struct smartSwitch *ss,struct smartSwitch *head){

    char* delim=",";//分隔符字符串
    char* ip=strtok(buf,delim);//第一次调用strtok
    char *mac,*sn,*flag;
	struct in_addr addr; 
	if(ip==NULL)
		return -1;
    int isIp = inet_pton(AF_INET, ip, &addr);  
    if(isIp<=0){
    	return -1;
    }
    mac=strtok(NULL,delim);
    if(mac==NULL||strlen(mac)!=12){
    	return -1;
    }
    sn=strtok(NULL,delim);
    if(sn==NULL||strlen(sn)>12){
    	return -1;
    }
    flag=strtok(NULL,delim);
    if(flag==NULL||strlen(flag)>1){
    	return -1;
    }

    flag=strtok(NULL,delim);
    if(flag==NULL||strlen(flag)>1||(flag[0]!='0'&&flag[0]!='1')){
    	return -1;
    }
    struct smartSwitch *p=head;
    while(p!=NULL){
    	if(strcmp(p->sn,sn)==0){
    		return -1;
    	}
    	p=p->next;
    }

    ss->addr.sin_family = AF_INET;//ipv4
    ss->addr.sin_port = htons(8899);//绑定本地端口
    printf("%s\n",ip);
    ss->addr.sin_addr.s_addr = inet_addr(ip);//本地地址绑定
    memset(ss->addr.sin_zero, '\0', sizeof ss->addr.sin_zero);

    strcpy(ss->mac,mac);
    strcpy(ss->sn,sn);
    if(flag[0]=='1')ss->state=1;
    else ss->state=0;
    ss->next=NULL;
    return 0;
}



