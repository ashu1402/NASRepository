/*
 * capture.h
 *
 *  Created on: 06-Jul-2015
 *      Author: s2a
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_


#include "CommonClass.h"
#include "Dump.h"
#include "ReStream.h"
//#include "SendAll.h"
#include "CameraControl.h"
#include "Notification.h"
#include "DataBaseAccess.h"

using namespace cv;
using namespace std;


struct RtspSender{
ReStream * gstreamerReader;
char rtspUrl [200];
};


class Channel{

public:
	//OLD
	bool isRTSPMode;
	cv::VideoCapture * videoCapture;
	char * rtspUrl ;
	int NoOfMethod;
	bool threadKilled;
	Dump * dumpVideoFiles;
	ReStream * restreamRtsp;
	ReStream * restreamRtspToUI;
	bool isDumpingOn;
	int videoIndex ;
	char *filePreFix;
	char * createdRtsp;
	char * NASIP, * databaseServerIp;
	char *resendRtsp;

//__________________________________________________________________________________________________________________________//
	//NEW
	char * rtsp , *iPAddress;
	int CID , DID;
	bool retrieverMode;
	Channel *channelObject;
	bool isBreak;
	pthread_t thread;
	ReStream *restreamBroadCast;
	ReStream *restreamLocal;
	char *outputPath, *videoStoreDirectory;
	CommonClass * commClass;
	bool startResending;
	vector < ReStreamInfo > restreamStoreData;
	char * startDateTime, *endDateTime;
	struct tm *startTime, *endTime;
	double storageSpaceAllocated;
	double freeSpaceInDiskForChannel;
	double storageSpaceUsed ;
	bool storageStatus;
	double waitTime;
	bool sendingIsOn;
	CameraControl * cameraControl;
	int streamMode;
	//Notification * notification;

public:

	//NEW
	Channel (  );
	void InitializeArrayOfChar();
	void CopyValues(const char *iP, const char * rtspInfo,  int CIDInfo, int DIDInfo, bool retrieverModeInfo, ReStream * restreamBroadCastInfo, ReStream * restreamLocalInfo, char * outputPathInfo, int streamMode);
	//void Initialize (Channel *channel,const char *iP,const char * rtspInfo ,  int CIDInfo , int DIDInfo , bool retrieverModeInfo, ReStream * restreamBroadCastInfo ,ReStream * restreamLocalInfo, char * outputPathInfo);
	void Initialize (Channel *channel,struct channelInfo channelinfo, ReStream * restreamBroadCastInfo, ReStream * restreamLocalInfo, char * systemIP );
	static void * StartThread ( void * );
	void StopAllThread();
	void ReStartAllThread ( Channel * channel );
	void StopDumping();
	void StartDumping();
	void StartReSending(DataBaseAccess * dBA);
	void RemoveReSending();
	void AddReStreaming(int CID, int DID, char *width , char *height, char *frameRate, char *bitRate, int restreamID);
	void RemoveReStreaming(int CID, int DID, char *width , char *height, char *frameRate, char *bitRate, int restreamID);
	void RemoveAllReStreaming();
	bool GetVideoFileSize(char * videoName,double &fileSize );
	void CreateCurrentVideoFileEndTime();
	void SetNewFileCreateTime();
	void GetStartAndEndTimeInPattern();
	void StopDumpingMainFunction();
	void StartDumpingMainFunction(Channel *channel);
	void UpdateStartResending(bool status);
	bool ContinuousMove();
	bool RelativeMove( int index );
	bool SendMail( char * msg );
	//____________________________________________________________________________________________________________________//
	//OLD
	~Channel ();
	void StartAllRtspSender( char *rtspUrlRestream );
	bool StartVideoCapture ();
	void StopVideoCapture ();
	void GetRtspFps( double &fps );
	void SetRtspFps( double fps );
	void GetRtspHeight( int &Height );
	void GetRtspWidth( int &Width  );
	bool SendImage();
	void ThreadStartFunction();
	static void* rtspSendFun(void* data);
	void StartFeature( char * className , int stringCID );
	void ReadFile ( std::string fileName , vector<std::string > & featureNames);
};


#endif /* CAPTURE_H_ */
