/*
 * Reteriver.h
 *
 *  Created on: 16-Nov-2015
 *      Author: ubuntu
 */

#ifndef RETRIEVE_H_
#define RETRIEVE_H_

#include "CommonClass.h"
#include "Channel.h"
#include "ReStream.h"
#include "DataBaseAccess.h"
#include "DeleteClass.h"
#include "CameraControl.h"
#include "Notification.h"

class Retrieve{

public:
	vector< Channel * > channelObjList;
	ReStream * restreamBroadCast;
	ReStream * restreamLocal;
	char * systemIP;
	char * broadCastPortNumber;
	char * localPortNumber;
	pthread_t startVideoEntry;
	CameraControl * cameraControl;


public:
	Retrieve( char * ipInfo , const char * broadCastPortNumber, const char * LocalPortNumber );
	void StartMultipleChannel( vector< channelInfo  > channelData );
	void CreateNewChannel(  channelInfo tempData );
	void StopChannel ( int CID, int DID );
	void RestartChannel ( channelInfo tempData );
	void StartDumping ( int CID, int DID );
	void StopDumping ( int CID, int DID );
	void StartReSending ( int CID, int DID );
	void RemoveReSending ( int CID, int DID );
	void AddReStreaming ( int CID, int DID, char *width , char *height, char *frameRate, char *bitRate , int restreamID);
	void RemoveReStreaming ( int CID, int DID, char *width , char *height, char *frameRate, char *bitRate, int restreamID );
	void PrintChannelList();
	void StartThreadForVideoEntry ( Retrieve * retrieveObject );
	static void *StartVideoEntry ( void *para );
	void GetDataBaseIp(char  *dataBaseIp);
	bool ContinuousMove ( int CID, int DID );
	bool RelativeMove ( int CID, int DID, int index );
	bool VerificationCamera( char * ipAddress , char * portNumber , char * userName , char * password ,char * rtsp  );
	bool GetChannelRtsp(int CID,int DID,char *rtsp);
};



#endif /* RETERIVER_H_ */
