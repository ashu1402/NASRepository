/*
 * capture.cpp
 *
 *  Created on: 06-Jul-2015
 *      Author: s2a
 */


#include "Channel.h"
#include "DataBaseAccess.h"
Channel::Channel ( ){
	//notification = new Notification ();
}

void Channel::StartReSending(DataBaseAccess * dBA){

	if(startResending && !sendingIsOn){

		restreamBroadCast->AddChannelStream( createdRtsp, CID, DID, NULL, NULL, NULL, NULL, resendRtsp, false,NULL );
		cout<<"\nInserting\n";
		dBA->InsertCreatedRtsp(CID,DID,resendRtsp);
		if ( ISDEBUG )
			cout<<"\nOutput---->"<<resendRtsp;
		sendingIsOn = true;
	}
}

void Channel::RemoveReSending(){
	if(!startResending && sendingIsOn){
		restreamBroadCast->RemoveChannelStream( CID, DID, NULL, NULL, NULL, NULL, false ,NULL);
		if ( ISDEBUG )
			cout<<"\nRemoved ReSending\n";
		sendingIsOn = false;
	}
}

void Channel::UpdateStartResending(bool status)
{
	startResending = status;
}

void Channel::RemoveReStreaming( int CID, int DID, char *width , char *height, char *frameRate, char *bitRate, int restreamID ){
	restreamBroadCast->RemoveChannelStream( CID, DID, width, height, frameRate, bitRate, true , restreamID);
	if ( ISDEBUG )
		cout<<"\nRemoved";
}

void Channel::AddReStreaming( int CID, int DID, char *width , char *height, char *frameRate, char *bitRate, int restreamID ){

	char output[2000];
	if ( ISDEBUG )
		cout<<"\nTrying to Add ReStreaming\n";

	restreamBroadCast->AddChannelStream( createdRtsp, CID, DID, width, height, frameRate, bitRate, output, true, restreamID );
	if ( ISDEBUG )
		cout<<"\n AddReStreaming -----> Output---->"<<output;
	ReStreamInfo tempData;
	sprintf ( tempData.bitRate,"%s" ,bitRate);
	sprintf ( tempData.frameRate,"%s" ,frameRate);
	sprintf ( tempData.height,"%s" ,height);
	sprintf ( tempData.width,"%s" ,width);
	tempData.restreamID = restreamID;
	restreamStoreData.push_back(tempData);

}


void Channel::CopyValues(const char *iP, const char * rtspInfo,  int CIDInfo, int DIDInfo, bool retrieverModeInfo, ReStream * restreamBroadCastInfo, ReStream * restreamLocalInfo, char * outputPathInfo, int restreamModeInfo ){

		sprintf ( iPAddress , "%s", iP );
		sprintf ( rtsp , "%s", rtspInfo );
		sprintf ( outputPath , "%s", outputPathInfo );
		if ( ISDEBUG )
			cout<<"\noutputPath------> "<<outputPath;
		retrieverMode = retrieverModeInfo;
		CID = CIDInfo;
		DID = DIDInfo;
		restreamBroadCast = restreamBroadCastInfo;
		restreamLocal = restreamLocalInfo;
		streamMode = restreamModeInfo;
}

void Channel::InitializeArrayOfChar(){
		rtsp = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIG);
		iPAddress = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIG);
		createdRtsp = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIGGER);
		outputPath = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIGGER);
		videoStoreDirectory = (char *)malloc(sizeof(char) * SIZEOFCHARARRAYBIGGER);
		filePreFix = (char *)malloc(sizeof(char) *SIZEOFCHARARRAYBIG);
		startDateTime = (char *)malloc(sizeof(char) *SIZEOFCHARARRAYBIG);
		endDateTime = (char *)malloc(sizeof(char) *SIZEOFCHARARRAYBIG);
		startTime = new struct tm();
	    endTime = new struct tm();
	    resendRtsp= (char *)malloc(sizeof(char) *SIZEOFCHARARRAYBIG);

}

void Channel::Initialize ( Channel * channel, struct channelInfo channelinfo, ReStream * restreamBroadCastInfo, ReStream * restreamLocalInfo, char * systemIP ){

	    InitializeArrayOfChar();
	    startResending = channelinfo.ReStream;

	    storageSpaceAllocated = atof( channelinfo.StorageSpace );
	    freeSpaceInDiskForChannel = ( storageSpaceAllocated *  (double)PERCENTAGEFREESPACE  )/( double )100;
	 //   storageSpaceAllocated = ( storageSpaceAllocated * ( 100 - (double)PERCENTAGEFREESPACE ) )/( double )100;
	    storageSpaceUsed = atof( channelinfo.StorageSpaceUsed );
	    storageStatus = channelinfo.StorageStatus;
	    CopyValues(channelinfo.cameraIp ,channelinfo.rtsp,channelinfo.CID,channelinfo.DID,channelinfo.retrieveMode,restreamBroadCastInfo,restreamLocalInfo,channelinfo.outputPath, channelinfo.ReStream);
		isBreak = false;
		dumpVideoFiles = new Dump( CID , DID );
		commClass = new CommonClass();
		commClass->CreateDirectory(outputPath);
		DataBaseAccess * dBA = new DataBaseAccess();
		commClass->CreateVideoStorageFolder(outputPath, CID,DID, videoStoreDirectory);
		if ( !streamMode ){
			retrieverMode = 1;

		}
		startResending = true;
		if ( !channelinfo.retrieveMode ){
			sprintf(createdRtsp,"%s", channelinfo.rtsp );
			sprintf(resendRtsp,"%s", channelinfo.rtsp );
			dBA->InsertCreatedRtsp(CID,DID,resendRtsp);
		}
		else{
			restreamLocal->AddChannelStream( rtsp, CID, DID, NULL, NULL, NULL, NULL, createdRtsp, false, NULL );
		}
		if ( ISDEBUG ){
			cout<<"\nChannel Initalizing......";
			cout<<"\tPlease wait For 2 seconds \n";
		}
		cout<<"\nIP"<<channelinfo.cameraIp;
		cout<<"\nUN"<<channelinfo.userName;
		cameraControl = new CameraControl(channelinfo.cameraIp , channelinfo.userName , channelinfo.password, systemIP , channelinfo.portNumber);
		sleep(2);

	if ( startResending )
		StartReSending(dBA);

	if ( pthread_create( &thread, NULL, StartThread, (void *)channel ) != 0)
		cout<<"\n Thread is not able to create"<<endl;
	else
		cout<<"\n Thread created"<<endl;
	dBA->Close();
	delete dBA;
}



void * Channel::StartThread ( void *para ){
	Channel * channel = (Channel *)para;
	channel->waitTime = 20;
	while(1){

		if ( channel->isBreak )
			break;
		sleep(channel->waitTime);
		if(channel->storageStatus && !channel->isDumpingOn ){
			channel->StartDumpingMainFunction(channel);
			//channel->SendMail("Camera Off");
		}
	}
}

bool Channel::RelativeMove ( int index ){
	if ( cameraControl != NULL )
		return cameraControl->StartRelativeMove( index );
	else
		return false;
}

bool Channel::ContinuousMove ( ){
	if ( cameraControl != NULL )
		return  cameraControl->StartContinuousMove();
	else
		return false;
}

void Channel::ReStartAllThread ( Channel * channel ){

	//StopAllThread();
	//Initialize ( channel );

}

void Channel::StopAllThread(){
	isBreak = true;
	StopDumping();
	pthread_join(thread, NULL);//(dumping);
	if ( startResending )
		RemoveReSending();

	RemoveAllReStreaming();
	if ( !retrieverMode ){
		restreamLocal->RemoveChannelStream(CID,DID,NULL,NULL,NULL,NULL,false,NULL);
	}

	free( rtsp );
	free(iPAddress);
	free(createdRtsp);
	free(outputPath);
	free(videoStoreDirectory);
}


void Channel::RemoveAllReStreaming(){

	int size = 0;
	ReStreamInfo tempData;
	size = restreamStoreData.size();
	for ( int i = 0 ; i < size ; i++ ){
		tempData = restreamStoreData[i];
		RemoveReStreaming(CID,DID,tempData.width, tempData.height,tempData.frameRate,tempData.bitRate,tempData.restreamID );
	}
	restreamStoreData.clear();
}

void Channel::StopDumping(){

	storageStatus = false;
	StopDumpingMainFunction();
}

void Channel::StopDumpingMainFunction(){
	if ( isDumpingOn ){
				if( dumpVideoFiles->loop!=NULL){
						if(g_main_loop_is_running(dumpVideoFiles->loop)){
							g_main_loop_quit(dumpVideoFiles->loop);
							if ( ISDEBUG ){
								cout<<"\nSuccessfully Quitted the main loop\t CID   "<<CID<<"\tDID  "<<DID;
								//SendMail("SuccessFully Stop Dumping");
							}
						}
						else{
							if ( ISDEBUG )
								cout<<"\nmain loop is not runnning\t CID   "<<CID<<"\tDID  "<<DID;
						}
					}
		dumpVideoFiles->WaitForInternalThreadToExit();
		isDumpingOn = false;
		}
}

void Channel::StartDumpingMainFunction(Channel *channel){
			int duration = 0;
			videoIndex = 0;
			char durationInNano[SIZEOFCHARARRAYBIG];

			SetNewFileCreateTime();
			commClass->GetStartAndEndTime( );
			commClass->ConvertDataTimeInPattern();
			commClass->GetTotalDurationInSeconds(duration);
			sprintf ( durationInNano,"%d000000000",duration);

			dumpVideoFiles->setRtspUrl(createdRtsp);
			dumpVideoFiles->setOutputFilePath(videoStoreDirectory);
			sprintf ( filePreFix,"%s_CID%dDID%d",commClass->startTimePattern,CID,DID );

			dumpVideoFiles->setFilePreFix(filePreFix);
			dumpVideoFiles->setDurationInNano(durationInNano);
			dumpVideoFiles->StartInternalThread();

			if ( ISDEBUG )
				cout<<"\nDumping Started" <<"\tCID"<<CID<<"\tDID"<<DID;
			isDumpingOn = true;
			dumpVideoFiles->WaitForInternalThreadToExit();
			isDumpingOn = false;
}


void Channel::StartDumping(  ){
	storageStatus = true;
}

void Channel::ReadFile ( std::string fileName , vector<std::string > & featureNames ){
	try{
	std::ifstream backstory ((char *)fileName.c_str());
			std::string line;
				if (backstory.is_open()){
					while (!backstory.eof()){
						getline(backstory,line);
						if ( strcmp((char *)line.c_str(),"") != 0)
							featureNames.push_back(line);
					}
					backstory.close();
				}
				else{
					std::cout << "Unable to open file" << std::endl << std::endl;
				}


}

	catch(cv::Exception e ){
		commClass->PrintException( "Channel", "ReadFile", e);
	}
	catch(std::exception e ){
		commClass->PrintException( "Channel", "ReadFile", e);
}
}




void Channel::CreateCurrentVideoFileEndTime(){

		struct tm temp;
		temp = *startTime;
		*endTime = temp;
		endTime->tm_hour +=  TOTALHOURS;

		endTime->tm_min +=  TOTALMIN;
		endTime->tm_sec +=  TOTALSEC;
		mktime(endTime);

}

void Channel::SetNewFileCreateTime(){
	 CreateCurrentVideoFileEndTime();
	 GetStartAndEndTimeInPattern();
		time_t t;
		time(&t);
		startTime = localtime(&t);
}


void Channel::GetStartAndEndTimeInPattern(){


	sprintf( startDateTime,"%04d-%02d-%02d %02d:%02d:%02d", (startTime->tm_year + 1900),(startTime->tm_mon + 1), startTime->tm_mday, startTime->tm_hour,
				startTime->tm_min, (startTime->tm_sec));
		sprintf(endDateTime,"%04d-%02d-%02d %02d:%02d:%02d", (endTime->tm_year + 1900),(endTime->tm_mon + 1), endTime->tm_mday, endTime->tm_hour,
				endTime->tm_min, (endTime->tm_sec));

}

bool Channel::GetVideoFileSize( char * videoName,double &fileSize ){
	try{
	struct stat file_status;
	if(stat(videoName, &file_status) != 0){
		return false;
	}
	else{
			fileSize = (double)file_status.st_size;
			return true;
	}
}
	catch(cv::Exception e ){
				 commClass->PrintException( "StoreDataAndDataBaseEntry", "GetVideoFileSize", e);
			}

			catch(std::exception e ){
				commClass->PrintException( "StoreDataAndDataBaseEntry", "GetVideoFileSize", e);
			}
			return false;
}

Channel::~Channel (){
	try{
		delete dumpVideoFiles;
}
	catch(cv::Exception e ){
		commClass->PrintException( "Channel", "~Channel", e);
	}
	catch(std::exception e ){
		commClass->PrintException( "Channel", "~Channel", e);
}
}




