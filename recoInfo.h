#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SCANINS "YZ-RECOSCAN\0"
#define SWITCHONINS "AT+YZSWITCH=1,on,"
#define SWITCHOFFINS "AT+YZSWITCH=1,off,"
#define GETINFOINS "AT+YZOUT\r\t"
#define RESULTOK "+OK"
#define RESULTERROR "+ERROR"

#define grantChar 5
#define grantLocalPort 49999
#define grantDstPort 49999
#define ScanDstPort 48899
#define dataSize 128

void Grant();
int UDPSend(int grantsocket,int len);
int RecoScan();
int RecoGrant();


struct UDPGrantInfo UGI;
int grantsocket,retimes=0;


struct UDPGrantInfo{
	int port;
	struct sockaddr_in addr;
	char data;
};

struct UDPScanInfo{
	int port;
	struct sockaddr_in addr;
	char data[dataSize];
};

struct TCPinfo{
	int num;
	int port;
	struct sockaddr_in addr;
	char data[dataSize];
};
