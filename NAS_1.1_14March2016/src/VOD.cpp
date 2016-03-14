/*
 * VOD.cpp
 *
 *  Created on: 18-Nov-2015
 *      Author: ubuntu
 */

#include "VOD.h"

	VOD::VOD(char * ipInfo , char * broadCastPortNumberInfo){
		//restreamVOD
		restreamVOD = new ReStream( broadCastPortNumberInfo, ipInfo, false  );
		restreamVOD->StartInternalThread();
		sleep(3);
	}

	void VOD::AddVOD( char * mountUrl , char * fileName ){
		char outputRTSP[10000];
		restreamVOD->AddVODStream(fileName,mountUrl,outputRTSP);
	}

	void VOD::RemoveVOD( char * mountUrl  ){
		//char outputRTSP[10000];
		restreamVOD->RemoveVODStream(mountUrl);
	}

	void VOD::AddVideoFiles(  char * mountUrl ,vector<std::string> fileName, vector<int> VID ){

		char tempMountUrl[SIZEOFCHARARRAYBIGGER];
		for ( int i = 0 ; i < (int)fileName.size() ; i ++ ){
			sprintf( tempMountUrl,"%s%d",mountUrl,VID[i]);
			AddVOD((char*)tempMountUrl,(char* )fileName[i].c_str());
		}

	}



