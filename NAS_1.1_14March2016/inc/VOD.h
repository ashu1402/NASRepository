/*
 * VOD.h
 *
 *  Created on: 18-Nov-2015
 *      Author: ubuntu
 */

#ifndef VOD_H_
#define VOD_H_

#include "ReStream.h"
#include "CommonClass.h"


class VOD {

ReStream * restreamVOD;

public:
	VOD(char * ipInfo , char * broadCastPortNumberInfo);
	void AddVOD( char * mountUrl , char * fileName);
	void RemoveVOD( char * mountUrl );
	//void AddFiles();
	//void AddVideoFiles( vector<std::string> fileName );
	void AddVideoFiles(  char * mountUrl ,vector<std::string> fileName, vector<int> VID );

};



#endif /* VOD_H_ */
