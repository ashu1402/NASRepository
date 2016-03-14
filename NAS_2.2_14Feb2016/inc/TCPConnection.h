#ifndef _TCPConnection_H_
#define _TCPConnection_H_

#include <ctime>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <vector>
#include <fstream>
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>
#include<algorithm>
#include <string.h>
#include <map>
#include "MyThreadClass.h"
#include "XMLParser.h"
#include "VOD.h"
#include "Interaction.h"
#include "Notification.h"
#include "Failover.h"

using namespace std;

struct threadData
{
	Retrieve* obj;
	int sockfd;
	VOD* vodObj;
	Notification * notification;
	char clientIp[40];
	bool isAndroid;
	Failover * failover;
	Interaction *interaction;
};

class TCPConnection:public MyThreadClass
{
private:
	vector<int> sockfd;
	int portNo;
	Retrieve* RetrieveObj;
	VOD* VODObj;
	Notification * notification;
	Failover * failover;
	Interaction * interaction;
public:
	bool isAndroid;
public:
	struct timeval timeout;

	public:
	TCPConnection(int value1,Retrieve* Obj,VOD* VodObj, Notification * notifi, bool isAndroidStatus, Failover * failoverObj,Interaction * interactionInfo);
	~TCPConnection();
	void error(const char *msg);
	void addSock(int fd);
	void deleteSock(int fd);
	void closeSock(int fd);
	void assignPort(int num);
	void ThreadFunc();
	bool sendData(int size, const void* data,int fd);
	static void* StartXMLParser(void * data);

};


#endif
