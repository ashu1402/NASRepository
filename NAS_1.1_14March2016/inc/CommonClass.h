/*
 * CommonClass.h
 *
 *  Created on: 30-Jun-2015
 *      Author: ashu
 */

#ifndef COMMONCLASS_H_
#define COMMONCLASS_H_

#include <netdb.h>
#include <errno.h>
#include "iostream"
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "ctime"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "arpa/inet.h"
#include <dirent.h>
#include <sys/statvfs.h>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string>
#include "opencv2/objdetect/objdetect.hpp"
#include "algorithm"
#include <curl/curl.h>


using namespace cv;
using namespace std;


#define PERCENTAGEFREESPACE 10
#define TOTALHOURS 1
#define TOTALMIN 0
#define TOTALSEC 0
#define SIZEOFCHARARRAYSMALL 100
#define SIZEOFCHARARRAYMEDIUM 500
#define SIZEOFCHARARRAYBIG 1000
#define SIZEOFCHARARRAYBIGGER 500000
#define VIDEOFILEEXTENSION "avi"
#define SIZEOFCHARARRAY 1000
#define SIZEOFCHARARRAYBIGGER 10000
#define THREADENTRYINTERVAL 120
static bool ISDEBUG = false;
#define FAILURECheckTimeInterval 20
#define FAILURERecursion 1
#define FAILURETIMEINTERVAL 1
#define tcpSocketPortNumber 55555
#define AndroidTcpSocketPortNumber 55590
static char *BroadCastStreamPortNumber = "55560";
static char * LocalStreamPortNumber = "55570";
static char * VODPortNumber = "55580";

struct ReStreamInfo {
	char width[20];
	char height[20];
	char frameRate[20];
	char bitRate[20];
	int restreamID;
};

struct DownloadInfo{
	int status;//-1 for new entry  /0 for processing /1 for completed
	char fileName[10000];
};


struct channelInfo {
	char Name[100];
	char rtsp[1000];
	char outputPath[1000];
	bool retrieveMode;
	int CID;
	int DID;
	char StorageSpace[1000];
	char StorageSpaceUsed[1000];
	char StorageFileType[1000];
	int StorageStatus;
	int ReStream;
	char cameraIp[100];
	char portNumber[100];
	char userName[1000];
	char password[1000];
};




//_________________________________________________________________________________________________________________________//
//OLD
struct storeDataClass{
		cv::Mat *image;
		cv::VideoCapture * videoCapture;
		cv::VideoWriter * videoWriter;
		char *rtspPath, *storeVideoPath, *storeVideoPathDirectory,*newVideoName, *startTimePattern,*endTimePattern;
		struct tm *startTime, *endTime;
		char *outputPath, *iP, *directoryPath;
		char *channelName;
		int portNumber;
		int videoWriterFPS;
		double spaceLimit;
		char startTimeInTimeStamp[30];
		char endTimeInTimeStamp[30];

};


typedef struct _imageData {
	int width;
	int height;
} S_image;

struct rectInfo {
	int x;
	int y;
	int width;
	int height;
};


struct Buffer{
	Mat * image;
	//cv::Rect * rect;
};

class CommonClass{

public:
	struct tm *startTime;
	struct tm *endTime;
	char * endTimePattern, *startTimePattern;

public:
	CommonClass();
	void CreateDirectory ( char * folderName );
	void CreateVideoStorageFolder( char *outputPath, int CID, int DID, char *videoStoreDirectory);
	void ShowError(const char *msg);
	bool IsleapYear (int year);
	int MonthDaysCount (int month, int count);
	int DayCount ( int count, int day );
	int GetDayOfYear( int year , int month , int day );
	void GetTotalDurationInSeconds( int & duration );
	void ConvertHourIntoSeconds ( int &duration );
	void ConvertMinIntoSeconds ( int &duration );
	void ConvertSecIntoSeconds ( int &duration );
	void GetIpAddress(char * ipAddress );
	void GetStartAndEndTime();
	void GetStartTime();
	void ConvertDataTimeInPattern();
	void PrintException (string className,string functionName,Exception e) ;
	void PrintException (string className,string functionName,exception e) ;
	void PrintLog( const char * logMessage );
	bool CreateConnectionClientReturnFD(char * address , int port,int &socketID );
	bool CloseSocketFD(int sockfd );
	//void copyChannelInfo(struct channelInfo* tempData);
	void copyChannelInfo(struct channelInfo* tempData,int  CID, string ChannelName,string RTSP,int DID,string StoragePath,int Retrivermode,string StorageSpace,string StorageSpaceUsed,string StorageFileType,int StorageStatus,int ReStream, string ipAddress,string portNumber, string userName,string password);
	void copyChannelInfos(struct channelInfo* tempData,int  CID, const char * ChannelName,const char * RTSP,int DID,const char * StoragePath,int Retrivermode,const char * StorageSpace,const char * StorageSpaceUsed,const char * StorageFileType,int StorageStatus,int ReStream, const char * ipAddress,const char * portNumber, const char * userName,const char * password);
	bool CreateConnectionClient(char * address , int port,char * xmlData );
	bool sendData(int size,const void *data,int fd);
	bool recvData(void* data,int size, int sockfd );
};



#endif /* COMMONCLASS_H_ */
