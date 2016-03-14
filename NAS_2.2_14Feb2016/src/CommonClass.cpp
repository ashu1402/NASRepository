/*
 * CommonClass.cpp
 *
 *  Created on: 30-Jun-2015
 *      Author: s2a
 */


#include "CommonClass.h"

CommonClass::CommonClass(){
	startTimePattern = (char *) calloc(sizeof(char),  SIZEOFCHARARRAYMEDIUM);
	endTimePattern = (char *) calloc(sizeof(char),  SIZEOFCHARARRAYMEDIUM);
	startTime = new struct tm();
	endTime = new struct tm();

}

void CommonClass::CreateDirectory ( char * folderName ){

	if ( mkdir(folderName, 0777)  == 0 ){
		cout<<"\nCreated directory------> "<<folderName << endl;
	}
}

void CommonClass::GetStartTime() {
	try{
	time_t t;
	time(&t);
	startTime = localtime(&t);


}
	catch(cv::Exception e ){
				PrintException( "CommonClass", "GetStartTime", e);
			}

			catch(std::exception e ){
				PrintException( "CommonClass", "GetStartTime", e);
			}
}




void CommonClass::ConvertDataTimeInPattern( ) {

	try{

	sprintf(startTimePattern, "%04d%02d%02d%02d%02d%02d", (startTime->tm_year + 1900),(startTime->tm_mon + 1), startTime->tm_mday, startTime->tm_hour,
			startTime->tm_min, (startTime->tm_sec));
	struct tm temp;
	temp = *startTime;
	*endTime = temp;
	endTime->tm_hour +=  TOTALHOURS;
	endTime->tm_min +=  TOTALMIN;
	endTime->tm_sec +=  TOTALSEC;
	mktime(endTime);
	sprintf(endTimePattern, "%04d%02d%02d%02d%02d%02d", (endTime->tm_year + 1900),(endTime->tm_mon + 1), endTime->tm_mday, endTime->tm_hour,
			endTime->tm_min, (endTime->tm_sec));

	/*sprintf(startTimeInTimeStamp,"%04d-%02d-%02d %02d:%02d:%02d", (startTime->tm_year + 1900),(startTime->tm_mon + 1), startTime->tm_mday, startTime->tm_hour,
			startTime->tm_min, (startTime->tm_sec));
	sprintf(endTimeInTimeStamp,"%04d-%02d-%02d %02d:%02d:%02d", (endTime->tm_year + 1900),(endTime->tm_mon + 1), endTime->tm_mday, endTime->tm_hour,
			endTime->tm_min, (endTime->tm_sec));*/

}
	catch(cv::Exception e ){
				PrintException( "StoreDataAndDataBaseEntry", "ConvertDataTimeInPattern", e);
			}

			catch(std::exception e ){
				PrintException( "StoreDataAndDataBaseEntry", "ConvertDataTimeInPattern", e);
			}

}


void CommonClass::GetStartAndEndTime() {

	try{

	GetStartTime(  );
	struct tm temp;
	temp = *startTime;
	*endTime = temp;
	endTime->tm_hour +=  TOTALHOURS;

	endTime->tm_min +=  TOTALMIN;
	endTime->tm_sec +=  TOTALSEC;
	mktime(endTime);

}
	catch(cv::Exception e ){
				PrintException( "CommonClass", "GetStartAndEndTime", e);
			}

			catch(std::exception e ){
				PrintException( "CommonClass", "GetStartAndEndTime", e);
			}

}


void CommonClass::CreateVideoStorageFolder( char *outputPath, int CID, int DID, char *videoStoreDirectory ){
	sprintf ( videoStoreDirectory,"%s/CID%dDID%d",outputPath,CID,DID);
	cout<<"\nvideoStoreDirectory---->"<<videoStoreDirectory;
	CreateDirectory(videoStoreDirectory);
}


void CommonClass::ShowError(const char *msg) {
	try{
	perror(msg);
	exit(1);
	}

		catch(cv::Exception e ){
			PrintException( "CommonClass", "StartRTSPModeChannels", e);
		}
		catch(std::exception e ){
			PrintException( "CommonClass", "StartRTSPModeChannels", e);
		}
		exit(-1);
}





int CommonClass::GetDayOfYear( int year , int month , int day ){
	try{
	int totalDays = MonthDaysCount( month , year );
	totalDays = DayCount( totalDays, day );
	return totalDays;
}

		catch(cv::Exception e ){
			PrintException( "CommonClass", "StartRTSPModeChannels", e);
		}
		catch(std::exception e ){
			PrintException( "CommonClass", "StartRTSPModeChannels", e);
		}
		return 0;
}

void CommonClass::ConvertHourIntoSeconds (int &duration ){
	try{
	duration = duration + TOTALHOURS * 3600;
}
	catch(cv::Exception e ){
		PrintException( "CommonClass", "ConvertHourIntoSeconds", e);
	}
	catch(std::exception e ){
		PrintException( "CommonClass", "ConvertHourIntoSeconds", e);
	}

}


void CommonClass::ConvertMinIntoSeconds (int &duration ){
	try{
	duration = duration + TOTALMIN * 60;
}
	catch(cv::Exception e ){
		PrintException( "CommonClass", "ConvertMinIntoSeconds", e);
	}
	catch(std::exception e ){
		PrintException( "CommonClass", "ConvertMinIntoSeconds", e);
	}
}


void CommonClass::ConvertSecIntoSeconds (int &duration ){
	try{
	duration = duration + TOTALSEC;
	}
		catch(cv::Exception e ){
			PrintException( "CommonClass", "ConvertSecIntoSeconds", e);
		}
		catch(std::exception e ){
			PrintException( "CommonClass", "ConvertSecIntoSeconds", e);
		}
}

void CommonClass::GetTotalDurationInSeconds(int & duration ){
	try{
	duration = 0;
	if ( TOTALHOURS > 0 ){
		ConvertHourIntoSeconds( duration );
	}

	if ( TOTALMIN > 0 ){
		ConvertMinIntoSeconds( duration );
	}

	if ( TOTALSEC > 0 ){
		ConvertSecIntoSeconds( duration );
	}

}
		catch(cv::Exception e ){
			PrintException( "CommonClass", "GetTotalDurationInSeconds", e);
		}
		catch(std::exception e ){
			PrintException( "CommonClass", "GetTotalDurationInSeconds", e);
		}
}





bool CommonClass::IsleapYear (int year) {
try{
	if( (year % 100 == 0) )
		if ( (year % 400) == 0 )
			return true;
		else
			return false;
	else{
		if((year % 4) == 0)
			return true;
		else
			return false;
	}

}
		catch(cv::Exception e ){
			PrintException( "CommonClass", "IsleapYear", e);
		}
		catch(std::exception e ){
			PrintException( "CommonClass", "IsleapYear", e);
		}
		return false;

}

int CommonClass::MonthDaysCount (int month, int year) {
	int extraDay = 0, count = 0;
	try{

	if ( IsleapYear(year) ){
		extraDay = 1;
	}
	switch(month){
	case 1:
		count= 0;
		break;
	case 2:
		count = 31;
		break;
	case 3:
		count = 59 + extraDay;
		break;
	case 4:
		count = 90 + extraDay;
		break;
	case 5:
		count = 120 + extraDay;
		break;
	case 6:
		count = 151 + extraDay;
		break;
	case 7:
		count =181 + extraDay;
		break;
	case 8:
		count = 212 + extraDay;
		break;
	case 9:
		count = 243 + extraDay;
		break;
	case 10:
		count = 273 + extraDay;
		break;
	case 11:
		count = 304 + extraDay;
		break;
	case 12:
		count = 334 + extraDay;
		break;
	};
	return count;

}
		catch(cv::Exception e ){
			PrintException( "CommonClass", "MonthDaysCount", e);
		}
		catch(std::exception e ){
			PrintException( "CommonClass", "MonthDaysCount", e);
		}
		return count;

}

int CommonClass::DayCount ( int count, int day ) {

	int totalDays = 0;
	try{
	totalDays = count  + day;
	return  totalDays;

}
		catch(cv::Exception e ){
			PrintException( "CommonClass", "DayCount", e);
		}
		catch(std::exception e ){
			PrintException( "CommonClass", "DayCount", e);
		}
		return  totalDays;
}

void CommonClass::PrintException (string className,string functionName,Exception e)
{
	try{
	FILE* file = fopen("NAS.log","a");
	char output[SIZEOFCHARARRAYMEDIUM];
	time_t t;
	time(&t);
	strftime( output, 21,"%Y-%m-%d %H:%M:%S", localtime(&t));
	fprintf( file, "time---> %s \n", output);
	fprintf(file,"%s    %s    %s \n", className.c_str(), functionName.c_str(), e.msg.c_str());
	fclose(file);
	}
	catch(cv::Exception e ){
		PrintException( "CommonClass", "PrintException", e);
	}
	catch(std::exception e ){
		PrintException( "CommonClass", "PrintException", e);
		}

}

void CommonClass::PrintException (string className,string functionName,exception e)
{
	try{
		FILE* file = fopen("NAS.log","a");
		char output[SIZEOFCHARARRAYMEDIUM];

		time_t t;
		time(&t);
		strftime( output, 21,"%Y-%m-%d %H:%M:%S", localtime(&t));
		fprintf( file, "time---> %s \n", output);
		fprintf(file,"%s    %s    %s \n", className.c_str(), functionName.c_str(), e.what());
		fclose(file);

	}
	catch(cv::Exception e ){
		cout<<"\nError in Log Writer Function\n";
	}
	catch(std::exception e ){
		cout<<"\nError in Log Writer Function\n";
	}
}

void CommonClass::PrintLog( const char * logMessage ){
	try{
	FILE* file = fopen("NAS.log","a");
	time_t t;
	char output[SIZEOFCHARARRAYMEDIUM];

	time(&t);
	strftime( output, 21,"%Y-%m-%d %H:%M:%S", localtime(&t));
	//fprintf(file,"%s    %s    %s \n", className.c_str(), functionName.c_str(), e.what());
	fprintf(file,"time---> %s \n %s\n",output,logMessage);
	fclose(file);
}
	catch(cv::Exception e ){
		cout<<"\nError in Log Writer Function\n";
	}
	catch(std::exception e ){
		cout<<"\nError in Log Writer Function\n";
	}

}

void CommonClass::copyChannelInfos(struct channelInfo* tempData,int  CID, const char * ChannelName,const char * RTSP,int DID,const char * StoragePath,int Retrivermode,const char * StorageSpace,const char * StorageSpaceUsed,const char * StorageFileType,int StorageStatus,int ReStream, const char * ipAddress,const char * portNumber, const char * userName,const char * password)
{
			tempData->CID = CID;
			tempData->DID = DID;
			sprintf ( tempData->Name ,"%s",ChannelName);
			sprintf ( tempData->outputPath ,"%s",StoragePath);
			sprintf ( tempData->rtsp ,"%s",RTSP);
			tempData->retrieveMode = Retrivermode;
			sprintf (tempData->StorageSpace,"%s",StorageSpace);
			sprintf (tempData->StorageSpaceUsed,"%s",StorageSpaceUsed);
			sprintf (tempData->StorageFileType,"%s",StorageFileType);
			sprintf (tempData->cameraIp,"%s",ipAddress);
			sprintf (tempData->portNumber,"%s",portNumber);
			sprintf (tempData->userName,"%s",userName);
			sprintf (tempData->password,"%s",password);
			tempData->StorageStatus = StorageStatus;
			tempData->ReStream= ReStream;
}

void CommonClass::copyChannelInfo(struct channelInfo* tempData,int  CID, string ChannelName,string RTSP,int DID,string StoragePath,int Retrivermode,string StorageSpace,string StorageSpaceUsed,string StorageFileType,int StorageStatus,int ReStream, string ipAddress,string portNumber, string userName,string password)
{
 copyChannelInfos( tempData, CID, ChannelName.c_str(),RTSP.c_str(),DID,StoragePath.c_str(), Retrivermode,StorageSpace.c_str(), StorageSpaceUsed.c_str(), StorageFileType.c_str(), StorageStatus,ReStream, ipAddress.c_str(),portNumber.c_str(),userName.c_str(),password.c_str());
}
bool CommonClass::CloseSocketFD(int sockfd ){
	close(sockfd);
}

bool CommonClass::CreateConnectionClientReturnFD(char * address , int port,int &socketID ){


    struct sockaddr_in serv_addr;
    struct hostent *server;
    socketID = socket(AF_INET, SOCK_STREAM, 0);
        if (socketID < 0)
            cout<<"\nERROR opening socket\n";
        server = gethostbyname(address);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
          return false;
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        serv_addr.sin_port = htons(port);
        if (connect(socketID,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        	fprintf(stderr,"ERROR connecting");
        	return false;
        }
        else{

        	return true;
        }
}


bool CommonClass::CreateConnectionClient(char * address , int port,char * xmlData ){

    int sockfd = -1;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            cout<<"\nERROR opening socket\n";
        server = gethostbyname(address);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
          return false;
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        serv_addr.sin_port = htons(port);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        	fprintf(stderr,"ERROR connecting");
        	return false;
        }
        else{
        	char receiData[100000];
        	int size = 100000;
        	if ( sendData(sizeof(int),&size,sockfd) ){
        		if ( sendData(size,xmlData,sockfd) ){
        			cout<<"\nSend XML--->"<<endl;
        			if ( recvData(&size,sizeof(int), sockfd) ){
        				if ( recvData(&receiData,size, sockfd) ){
        					cout<<"\nReceived XML\n"<<endl;
        				}
        			}
        		}
        	}
        	close(sockfd);
        	return true;
        }
}

bool CommonClass::sendData(int size,const void *data,int fd){

	int  ret = 0;
	do{
		ret = send(fd,data,size,0);
		size = size - ret;
		data = data+ret;
	}while(size!=0);
	if(ret<=0){
		switch(errno){
		case EAGAIN: cout<<"\nSend Timeout\n";
		break;
		case ECONNRESET: cout<<"\nConnection forcibly closed by peer:";
		break;
		default:
			printf("\nFailed to send the file. Equal to zero. Closing the socket.\n");
			cout<<strerror(errno)<<endl;
			break;
		}
		cout<<"\nClosing the socket and deleting the entry:\n";
		close(fd);
		return false;
	}

	return true;

}

bool CommonClass::recvData(void* data,int size, int sockfd ){

	int temp = 0, ret = 0;
	ret = recv(sockfd, data, size, 0);
	if ( ret >= 0 ){
		while ( ret != size ){
			temp = recv(sockfd, ( data + ret ), size, 0);
			if ( temp < 0 )
				break;
			ret += temp;
			if ( ret == size )
				break;
		}
	}
	if(ret>0 && ret!=size){
		cout<<"Error: Incomplete data received\n";
		cout<<"Closing the socket.\n";

		return false;
	}
	if(ret<0){
		switch(errno){
		case EAGAIN: cout<<"Receive Timeout\n";
		break;
		case ECONNRESET: cout<<"Connection forcibly closed by peer\n";
		break;
		default:
			printf("Failed to receive the file.\n");
			cout<<strerror(errno)<<endl;
			break;
		}
		cout<<"Closing the socket.\n";

		return false;
	}
	return true;

}


