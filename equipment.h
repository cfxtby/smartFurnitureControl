#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct smartSwitch
{
	struct sockaddr_in addr;//存储当前的reco设备地址
	char mac[7];//存储当前reco设备的Mac地址
	int state;//存储当前设备的开关状态
	int I;//存储当前设备的电流值，单位为0.01A
	int U;//存储当前设备的电压值，单位为0.01V
	int P;//存储当前设备的功率值，单位为0.01W
	int E;
	//char name[20];//存储当前设备的别名
	struct smartSwitch *next;
	char sn[10];
};
