/*
 * Download.h
 *
 *  Created on: 20-Nov-2015
 *      Author: ubuntu
 */

#ifndef DOWNLOAD_H_
#define DOWNLOAD_H_

#include <iostream>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <vector>
#include <fstream>
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>
#include<algorithm>
#include <string.h>
#include <map>
#include <vector>
#include "Retrieve.h"
#include "CommonClass.h"
#include <gst/gst.h>

class Download{


public:
	char * fileName;
	vector < DownloadInfo > downloadList;
	pthread_t downloadThread;
	int portNumber;

public:
	Download(  );
	static void *StartThread( void * para );
	void CreateDownloadServer( char ** videoName , int size );
	void StartDownloadThread(Download * downloadObj, vector<string> videoName );
	void AddVideoFileToDownloadQueue ( char * fileName );
	void RemoveVideoFileFromDownloadQueue ( char * fileName );
	void* StartDownloadThread( void *para );
	int getPort();
};

struct DownloadPara {
	Download * downloadObj;
	char ** videoNames;
	int size ;
};

#endif /* DOWNLOAD_H_ */
