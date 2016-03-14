/*
 * Delete.h
 *
 *  Created on: 20-Jul-2015
 *      Author: ashu
 */

#ifndef DELETE_H_
#define DELETE_H_

#include "CommonClass.h"
#include "DataBaseAccess.h"

//class StoreDataAndDataBaseEntry;

class Channel;

class ClassDelete {

public:
	DataBaseAccess * dBA;
	char *directoryPath;
	int COMBOID;
	double folderSizeLimit;
	double freeSpaceLimit;
	CommonClass* commClass;

public:
	ClassDelete();

	/*~ClassDelete();
	void SetFolderSizeLimit( double size );
	void DeleteFile(char* fileName);
	void GetFiles( vector<std::string> &videosName, vector<int> &VIDs );
	//double GetFolderSize();
	bool IsChannelFolderLimitExceed( double folderSize, double limit, double requiredFolderSpace);
	bool IsAvailableFreeSpace( double folderSize, double folderLimitSpace, double requiredFolderSpqace);
	void StartDelete( double & );
	void SetRequiredSpaceLimit( );*/
	void CheckChannelFolderSizeLimit( Channel *channel );
	void DeleteFile(char* fileName);
};
#endif
