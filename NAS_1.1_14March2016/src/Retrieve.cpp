/*
 * Retrieve.cpp
 *
 *  Created on: 16-Nov-2015
 *      Author: ubuntu
 */

#include "Retrieve.h"
#include "DataBaseAccess.h"

Retrieve::Retrieve( char * ipInfo , const char * broadCastPortNumberInfo,const char * LocalPortNumberInfo  ){

	systemIP = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIG);
	broadCastPortNumber = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIG);
	localPortNumber = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIG);
	sprintf( systemIP,"%s", ipInfo);
	sprintf(broadCastPortNumber,"%s", broadCastPortNumberInfo);
	sprintf(localPortNumber,"%s", LocalPortNumberInfo );
	restreamBroadCast = new ReStream( broadCastPortNumber, systemIP, false  );
	restreamLocal = new ReStream ( localPortNumber, "127.0.0.1", true );
	restreamBroadCast->StartInternalThread();
	restreamLocal->StartInternalThread ();
	cameraControl = new CameraControl ();
	//notification = new Notification ();
	sleep(3);
}

void *Retrieve::StartVideoEntry ( void *para ){

	Retrieve * retrieveObject = ( Retrieve *)para;
	int i = 0;
	Channel * channel;
	ClassDelete *deleteClass = new ClassDelete();
	DataBaseAccess * dBA;//= new DataBaseAccess(  );
	char nextFileName[SIZEOFCHARARRAYBIG],fileName[SIZEOFCHARARRAYBIGGER];
	double videoSize = 0;
	while(1){

		sleep(THREADENTRYINTERVAL);
		dBA = new DataBaseAccess(  );
		for ( i = 0 ; i < (int)retrieveObject->channelObjList.size(); i++ ){
			channel = retrieveObject->channelObjList[i];
			if ( channel->isDumpingOn ){
				sprintf( fileName,"%s/%s%08d.avi",channel->videoStoreDirectory,channel->filePreFix,channel->videoIndex);
				sprintf( nextFileName,"%s/%s%08d.avi",channel->videoStoreDirectory,channel->filePreFix,channel->videoIndex + 1);
				if ( channel->GetVideoFileSize(nextFileName,	videoSize) ){

					channel->SetNewFileCreateTime();
					channel->GetVideoFileSize(fileName,	videoSize);
					dBA->InsertVideoInfo(fileName,channel->CID,channel->DID,channel->startDateTime,channel->endDateTime, videoSize);
					channel->storageSpaceUsed += videoSize;
					channel->videoIndex++;
					//deleteClass->CheckChannelFolderSizeLimit( channel );
					dBA->UpdateChannelStorageSpaceUsed(channel->CID,channel->DID,channel->storageSpaceUsed);
				}
			}
		}
		dBA->Close();
		delete dBA;
	}
}

void Retrieve::StartThreadForVideoEntry ( Retrieve * retrieveObject ) {
	if ( pthread_create( &startVideoEntry, NULL, StartVideoEntry, (void *)retrieveObject ) != 0){
		if ( ISDEBUG )
			cout<<"\n Thread is not able to create"<<endl;
	}
	else{
		if ( ISDEBUG )
			cout<<"\n video entry Thread created"<<endl;
	}
}

void Retrieve::StartMultipleChannel( vector< channelInfo > channelData ){

	int size = (int)channelData.size();
	channelInfo tempData;

	for ( int i = 0 ; i < size ; i ++ ){
		tempData = channelData[i];
		CreateNewChannel(tempData);
	}
}

void Retrieve::CreateNewChannel(  channelInfo tempData ){
	Channel * channel;
	channel = new Channel(  );
	channel->Initialize(channel,tempData, restreamBroadCast, restreamLocal,systemIP);
	channelObjList.push_back(channel);
}

void Retrieve::PrintChannelList(){
	Channel * channel;
	int size = (int)channelObjList.size();
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( ISDEBUG )
			cout<<"\n Exits ---->CID "<<channel->CID<<"\tDID "<<channel->DID<<endl;
	}
}

bool Retrieve::RelativeMove ( int CID, int DID, int index ){
	Channel * channel;
	int size = (int)channelObjList.size();
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( channel->CID == CID &&  channel->DID == DID ){
			return channel->RelativeMove( index );
		}
	}
}

bool Retrieve::ContinuousMove ( int CID, int DID ){
	Channel * channel;
	int size = (int)channelObjList.size();
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( channel->CID == CID &&  channel->DID == DID ){
			return channel->ContinuousMove();
		}
	}
}


bool Retrieve::GetChannelRtsp(int CID,int DID,char *rtsp){
	Channel * channel;
		int size = (int)channelObjList.size();
		for ( int i = 0 ; i < size ; i ++ ){
			channel = channelObjList[i];
			if ( channel->CID == CID &&  channel->DID == DID ){
				if ( channel->startResending )
					sprintf( rtsp , "%s", channel->resendRtsp);
				else
					return false;
				cout<<"\nRTSP--->"<<rtsp;
				cout<<"\n44444"<<endl;
				return true;
			}
		}
		return false;
}


void Retrieve::StopChannel ( int CID, int DID ){
	Channel * channel;
	int size = (int)channelObjList.size();
	if ( ISDEBUG ){
		cout<<"\nSize before Stop ----"<<size;
		PrintChannelList();
	}
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( channel->CID == CID &&  channel->DID == DID ){
			channel->StopAllThread();
			channelObjList.erase( channelObjList.begin() + i );

			delete channel;
			break;
		}
	}
	if ( ISDEBUG ){
		cout<<"\nSize After Stop ----"<<channelObjList.size()<<endl;
		PrintChannelList();
	}
}

void Retrieve::RestartChannel ( channelInfo tempData ){
	//Channel * channel;
	StopChannel(tempData.CID,tempData.DID);
	CreateNewChannel ( tempData );
}

void Retrieve::StopDumping ( int CID, int DID ){
	Channel * channel;
	int size = (int)channelObjList.size();
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( channel->CID == CID &&  channel->DID == DID ){
			channel->StopDumping();
			break;
		}
	}
}


void Retrieve::StartDumping ( int CID, int DID ){
	Channel * channel;
	int size = (int)channelObjList.size();
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( channel->CID == CID &&  channel->DID == DID ){
			channel->StartDumping( );
			break;
		}
	}
}

void Retrieve::RemoveReStreaming ( int CID, int DID, char *width , char *height, char *frameRate, char *bitRate, int restreamID ){
	Channel * channel;
	int size = (int)channelObjList.size();
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( channel->CID == CID &&  channel->DID == DID ){
			channel->RemoveReStreaming ( CID, DID, width , height,frameRate, bitRate, restreamID );
			ReStreamInfo tempData, storeData;
			sprintf ( tempData.bitRate,"%s" ,bitRate);
			sprintf ( tempData.frameRate,"%s" ,frameRate);
			sprintf ( tempData.height,"%s" ,height);
			sprintf ( tempData.width,"%s" ,width);
			for ( int j = 0 ; j < (int)channel->restreamStoreData.size(); j++ ){
				storeData = channel->restreamStoreData[j];
				if ( strcmp(storeData.bitRate,tempData.bitRate) == 0 && strcmp(storeData.frameRate,tempData.frameRate ) == 0 &&strcmp(storeData.height,tempData.height) == 0 && strcmp( storeData.width,tempData.width) == 0){
					channel->restreamStoreData.erase(channel->restreamStoreData.begin() + j );
					break;
				}
			}
			break;
		}
	}
}

void Retrieve::AddReStreaming ( int CID, int DID, char *width , char *height, char *frameRate, char *bitRate ,int restreamID){
	Channel * channel;
	int size = (int)channelObjList.size();
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( channel->CID == CID &&  channel->DID == DID ){
			channel->AddReStreaming( CID, DID, width , height,frameRate, bitRate , restreamID);
			break;
		}
	}
}

void Retrieve::RemoveReSending ( int CID, int DID ){
	Channel * channel;
	int size = (int)channelObjList.size();
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( channel->CID == CID &&  channel->DID == DID ){
			channel->UpdateStartResending(false);
			channel->RemoveReSending ( );
			break;
		}
	}
}

void Retrieve::StartReSending ( int CID, int DID ){
	Channel * channel;
	int size = (int)channelObjList.size();
	DataBaseAccess * dBA = new DataBaseAccess();
	cout<<"\nCreate StartResending DBA\n";
	for ( int i = 0 ; i < size ; i ++ ){
		channel = channelObjList[i];
		if ( channel->CID == CID &&  channel->DID == DID ){
			channel->UpdateStartResending(true);
			channel->StartReSending( dBA );
			dBA->Close();
			cout<<"\nClose StartResending DBA\n";
			delete dBA;
			break;
		}
	}
	cout<<"\nClose StartResending DBA\n";
	dBA->Close();
	delete dBA;
}

bool Retrieve::VerificationCamera( char * ipAddress , char * portNumber , char * userName , char * password ,char * rtsp){
	char tempSystemIp[1000];
	sprintf( tempSystemIp,"%s", systemIP);
	return cameraControl->VerificationCamera(ipAddress, portNumber , userName, password, rtsp, tempSystemIp );
}

/*bool Retrieve::SendMail(){
	cout<<"\nRetrieve SendMail\n"<<endl;
	notification->SendMail("ashu@dakshaiamging.com", "ashu@dakshaimaging.com","nnehasharma1706@gmail.com","Daksha VMS","Dumping Off","Thank you for using daksha imaging VMS");
}*/

