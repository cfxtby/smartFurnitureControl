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

int contral(char ins[],struct smartSwitch *ss){
	int s = socket(AF_INET, SOCK_STREAM, 0);
	char buf[100];
    if (s != -1) {
        printf("Create a socket with fd: %d\n", s);
    } else {
        fprintf(stderr, "Fail to create a socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
	memset(ss->addr.sin_zero, '\0', sizeof ss->addr.sin_zero);
	if (connect(s, (const struct sockaddr *) &(ss->addr), sizeof ss->addr) ==0) {
        printf("Connect to the server\n");
    } else {
        fprintf(stderr, "Fail to connect to the server: %s\n",
                strerror(errno));
        exit(EXIT_FAILURE);
    }
	ssize_t send_size = send( s,ins, strlen(ins), 0);
	ssize_t recv_size = recv(s,buf, 100, 0);
	buf[recv_size]="\0";
	if(recv_size>10){
		if(recordReco(buf,ss)==0)
			return 0;
		else
			return -1;
	}
	else{
		if(ins[3]=="N"||ins[3]=="n"){
			if(strcmp(buf,"+ok")==0){
				ss->state=1;
				return 0;
			}
			else{
				return -1;
			}
		}
		else if(ins[3]=="F"||ins[3]=="f"){
			if(strcmp(buf,"+ok")==0){
				ss->state=0;
				return 0;
			}
			else{
				return -1;
			}
		}
		
	}
	
}


//1118-1896-8545-7524-8021-7867

int recordReco(char buf[],struct smartSwitch *ss){

    char* delim=",";//分隔符字符串
    char* I=strtok(buf,delim);//第一次调用strtok
    char *U,*P,*E;
	int tmp;
	tmp=atoi(I);
    if(tmp==0){
    	return -1;
    }
	ss->I=tmp;
    U=strtok(NULL,delim);
	tmp=atoi(U);
    if(tmp==0){
    	return -1;
    }
	ss->U=tmp;
	
    P=strtok(NULL,delim);
	tmp=atoi(P);
    if(tmp==0){
    	return -1;
    }
    P=strtok(NULL,delim);
	tmp=atoi(P);
    if(tmp==0){
    	return -1;
    }
	ss->P=tmp;
	
	P=strtok(NULL,delim);
	tmp=atoi(P);
    if(tmp==0){
    	return -1;
    }
	P=strtok(NULL,delim);
	tmp=atoi(P);
    if(tmp==0){
    	return -1;
    }
	ss->E=tmp;

    return 0;
}



/*
11:52:24 发送数据：AT+YZSWITCH=1,ON,201704031153[1次]
11:52:24 收到数据：+ok


11:52:43 发送数据：AT+YZSWITCH=1,OFF,201704031153[1次]
11:52:43 收到数据：+ok
                   +error


11:53:18 发送数据：AT+YZOUT
[1次]
11:53:18 收到数据：+ok=3,22694,5007,0,0,3548,0

*/

