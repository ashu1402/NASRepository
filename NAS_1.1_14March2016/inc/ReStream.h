#ifndef _ReStream_
#define _ReStream_


#include "CommonClass.h"
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <gst/rtsp/gstrtspdefs.h>
#include "MyThreadClass.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include "dataBaseAccess.h"

using namespace std;

#define FPS 10

struct MyContext
{
 // gboolean white;
  GstClockTime timestamp;
//  CvCapture* cap;
  GstBuffer *buffer;
  GstMapInfo map;
  IplImage* image;
  int width;
  int height;
  int fps;
  int ComboID;

  pthread_mutex_t imgLock;
} ;

class ReStream:public MyThreadClass 
{	
public:
	char * portNo;
	//DataBaseAccess *dBA;
	char *ipAddress;
	GstRTSPServer *server;
	GMainLoop *loop;
	GstRTSPMountPoints *mounts;
	bool isServerAttached;
	bool isLocalStream;
	CommonClass commclass;



public:
	ReStream(char * portNumber, const char * ipAddressInfo, bool globalLoop );
	~ReStream();
	char* getPortNo();
	char* getrtspUrl();
	void setPortNo(char* portNum);
	//void setrtspUrl(char* Url);
	void SetIpAddress(char * ipAddressInfo );
	static void media_constructed (GstRTSPMediaFactory * factory, GstRTSPMedia * media, gpointer user_data);
	static void prepared (GstRTSPMedia *gstrtspmedia, gpointer user_data);
	static void unprepared (GstRTSPMedia *gstrtspmedia, gpointer user_data);
	static void new_state (GstRTSPMedia *gstrtspmedia, gpointer user_data);
	static void new_stream (GstRTSPMedia *gstrtspmedia, gpointer user_data);
	static void removed_stream (GstRTSPMedia *gstrtspmedia, gpointer user_data);	
	static void media_configure (GstRTSPMediaFactory * factory, GstRTSPMedia * media, gpointer user_data);
	static void client_connected (GstRTSPServer *gstrtspserver, GstRTSPClient *arg1, gpointer user_data);
	static void media_configure_feature (GstRTSPMediaFactory * factory, GstRTSPMedia * media, gpointer user_data);
	static void unprepared_feature (GstRTSPMedia *gstrtspmedia, MyContext* ctx);
	static void need_data_feature (GstElement * appsrc, guint unused, gpointer user_data);
	void AddVODStream( char* fileName,char * mountUrl, char * outputRtspUrl );
	void GetCreatedRtsp ( char * rtsp );
	void RemoveVODStream ( char * mountUrl );
	void ThreadFunc();

	void AddChannelStream(char* rtspUrl,int CID, int DID, char* width,char* height, char* frameRate,char* bitrate, char * outputRtspUrl, bool isResize, int restreamID);
	void RemoveChannelStream(int CID, int DID,char* width,char* height, char* frameRate,char* bitrate, bool isResize , int restreamID);

	void AddFeatureStream(char* featureName,int COMBOID,char * outputRtspUrl,struct MyContext* ctx);
	void RemoveFeatureStream(char* featureName,int COMBOID);
	//void stopServer();
	//void RtstToRTspFeed(char* rtspUrl, char* portNo);
};

#endif

