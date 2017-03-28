#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SCANINS "RZ-RECOSCAN"
#define grantChar 5
#define grantLocalPort 49999
#define grantDstPort 49999
#define ScanDstPort 48899

struct UDPGrantInfo{
	int port;
	struct sockaddr_in addr;
	char data=grantChar;
}

struct UDPScanInfo{
	int num;
	int port;
	struct sockaddr_in addr;
	char data[]=SCANINS;
}

struct TCPinfo{
	int num;
	int port;
	struct sockaddr_in addr;
	char* data;
}