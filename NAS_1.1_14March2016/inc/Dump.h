#ifndef _Dump_
#define _Dump_

#include <gst/gst.h>

#include <iostream>
#include <stdio.h>
#include "CommonClass.h"
#include "MyThreadClass.h"

using namespace std;

struct DumpStruct {
	GMainLoop * loop;
	//DataBaseAccess * dBAOBj;
	int COMBOID;
};


class Dump:public MyThreadClass
{
private:
	char* rtspUrl;
	char* outputFilePath;
	char* fileName;
	char* duration;
	GstElement *pipeline;
public:
	GMainLoop* loop;
	 int CID,DID;
	//DataBaseAccess* dBA;
	CommonClass commclass;
	
public:
	//Dump(int COMBOIDInfo , DataBaseAccess * dBAObj);
	Dump(int CID, int DID);
	void setRtspUrl(char* Url);
	void setOutputFilePath(char* fileurl);
	void setFilePreFix(char* filePreFix);
	void setDurationInNano(char* durationInNano);
	static gboolean TimeoutFunc(gpointer data);

	static gboolean bus_call (GstBus  *bus, GstMessage *msg, gpointer    data);
	void ThreadFunc();
	//void pause();
	//void restart();
	//void stop();
};

#endif
