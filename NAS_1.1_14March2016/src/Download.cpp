/*
 * Download.cpp
 *
 *  Created on: 20-Nov-2015
 *      Author: ubuntu
 */

#include "Download.h"

Download::Download(){

	portNumber=0;
}

int Download::getPort()
{ try{
	while(1){
		if(portNumber==0)
			usleep(0.5);
		else
			return portNumber;

	}
	cout<<"Error in fetching portNo\n";
}catch (exception &e){
	cout<<"Exception caught in getPort Function"<<e.what()<<endl;
}
return 0;
}

void *Download::StartThread( void * para ){
	try{
		cout<<"Start Therea\n\n";
		struct DownloadPara * downloadPara = (struct DownloadPara * )para;
		downloadPara->downloadObj->CreateDownloadServer(downloadPara->videoNames, downloadPara->size );
		cout<<"Ending of Start Theras\n\n\n;";
	}catch (exception &e){
		cout<<"Exception caught in StartThread Function"<<e.what()<<endl;
	}

	return 0;
}

void Download::StartDownloadThread(Download * downloadObj, vector<string> videoName ){
	try{
		struct DownloadPara * downlaodPara = ( DownloadPara *)malloc(sizeof(DownloadPara) * 1);
		downlaodPara->downloadObj = downloadObj;
		cout<<"Entered Start Download thread\n";
		cout<<"Size of Vnmae="<<videoName.size()<<endl;
		cout<<"\n1!"<<endl;
		downlaodPara->videoNames = (char**)malloc(sizeof(char*)*videoName.size());
		downlaodPara->size = videoName.size();
		for ( int i = 0 ; i < (int)videoName.size() ; i++ ){
			downlaodPara->videoNames[i] = (char*)malloc(sizeof(char)*10000);
			sprintf(downlaodPara->videoNames[i],"%s",videoName[i].c_str());
		}
		cout<<"\n21!"<<endl;
		if ( pthread_create( &downloadThread, NULL, StartThread, (void *)downlaodPara ) != 0){
			if ( ISDEBUG )
				cout<<"\n video entry Thread is not able to create"<<endl;
		}
		else{
			if ( ISDEBUG )
				cout<<"\n video entry Thread created"<<endl;
		}
		cout<<"\n21!"<<endl;
	}catch (exception &e){
		cout<<"Exception caught in StartDownloadThread Function"<<e.what()<<endl;
	}
}


void Download::CreateDownloadServer( char ** videoName, int size ){
	try{
		cout<<"inside Download Server \n\n";
		int listenfd = 0;
		int connfd = 0;
		struct sockaddr_in serv_addr;
		char sendBuff[1025];

		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		printf("Socket retrieve success\n");
		memset(&serv_addr, '0', sizeof(serv_addr));
		memset(sendBuff, '0', sizeof(sendBuff));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(portNumber);
		//portNumber
		bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

		socklen_t len = sizeof(serv_addr);
		if (getsockname(listenfd, (struct sockaddr *)&serv_addr,&len) == -1)
			perror("getsockname");
		else{
			portNumber =ntohs(serv_addr.sin_port);
		}

		if(listen(listenfd, 10) == -1){
			printf("Failed to listen\n");
		}


		cout<<"Before While Loop.portNUmber="<<portNumber<<endl;
		while(1){
			cout<<"Waiting for Connection to be made \n;";
			connfd = accept( listenfd, (struct sockaddr*)NULL ,NULL);
			cout<<"Connection made\n\nwith size of downlaod list="<<size<<endl;
			FILE * fp;
			for ( int i = 0 ;  i < size; i++ ){
				////	if ( downloadList[i].status == -1 ){
				//		downloadList[i].status = 0;
				cout<<"opening the file for iteration ===="<<i<<endl<<endl;
				fp = fopen(videoName[i],"rb");
				if(fp == NULL){
					printf("File opern error");
				}

				fseek(fp, 0L, SEEK_END);
				int size = ftell(fp);
				cout<<"size of File ="<<size<<endl;
				write(connfd,&size,sizeof(int));

				fseek(fp, 0L, SEEK_SET);


				while(1){
					unsigned char buff[1024]={0};
					int nread = fread(buff, 1, 1024,fp);
					if( nread > 0 )
					{
						send(connfd,&buff,nread,0);
						size = size - nread;
					}
					else{
						fclose(fp);
						cout<<"\nBreak";
						break;
					}
				}

				cout<<"End of while lop[p\n";
			}
			close(connfd);
		}
	}catch (exception &e){
		cout<<"Exception caught in CreateDownloadServer Function"<<e.what()<<endl;
	}
}

void Download::AddVideoFileToDownloadQueue ( char * fileName ){
	DownloadInfo tempData;
	sprintf ( tempData.fileName,"%s", fileName);
	tempData.status = -1;
	downloadList.push_back(tempData);
}

void Download::RemoveVideoFileFromDownloadQueue ( char * fileName ){
	DownloadInfo tempData;
	sprintf ( tempData.fileName,"%s", fileName);
	tempData.status = 1;
}


