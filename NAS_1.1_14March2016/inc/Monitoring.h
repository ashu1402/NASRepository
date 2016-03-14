#ifndef _Monitoring_
#define _Monitoring_

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
#include "sys/vtimes.h"
#include "sys/types.h"
#include "sys/sysinfo.h"
#include <iostream>
#include "MyThreadClass.h"
#include "DataBaseAccess.h"

using namespace std;

class Monitoring:public MyThreadClass
{
private:
	clock_t lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;
	DataBaseAccess* dBA;
	bool isBreak;

public:
	Monitoring();
	void init();
	double getProcessCurrentCpuUtilisation();
	double getCurrentCpuUtilisation();

	void setMemoryInfo();
	int getProcessCurrentVirtualMemoryUtilisation(); //Note: this value is in KB!
	int getProcessCurrentPhysicalMemoryUtilisation(); //Note: this value is in KB!

	int  parse_netdev(unsigned long long int *receivedabs, unsigned long long int *sentabs);
	void calculate_speed(char *speedstr, unsigned long long int newval, unsigned long long int oldval);
	char* get_netusage(unsigned long long int *rec, unsigned long long int *sent);

	void ThreadFunc();

};

#endif
