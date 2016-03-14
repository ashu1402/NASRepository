/*
 * ClassDelete.cpp
 *
 *  Created on: 20-Jul-2015
 *      Author: ashu
 */


#include "DeleteClass.h"
#include "Channel.h"


ClassDelete::ClassDelete(){

}


void ClassDelete::DeleteFile(char* fileName){
	try{
	remove( fileName ) ;
}
		catch(cv::Exception e ){
			 commClass->PrintException( "ClassDelete", "DeleteFile", e);
		}

		catch(std::exception e ){
			commClass->PrintException( "ClassDelete", "DeleteFile", e);
		}
}

void ClassDelete::CheckChannelFolderSizeLimit( Channel *channel ){
	vector<std::string> videosName;
	vector<int> VIDs;
	double size = 0,storageSpaceUsed = 0;
	int index = 0;
	dBA = new DataBaseAccess();
	if ( channel->storageSpaceAllocated < channel->storageSpaceUsed ){
		dBA->GetVideoNameAndVID(videosName, VIDs , channel->CID, channel->DID);
		for (int i = 0 ; i < (int)VIDs.size() ; i++ ){
			if ( channel->storageSpaceAllocated - channel->freeSpaceInDiskForChannel > channel->storageSpaceUsed )
				break;
			dBA->DeleteVideoFromAllTable( VIDs[i], size );
			dBA->GetStorageSpaceUsed( channel->CID, channel->DID, storageSpaceUsed );
			channel->storageSpaceUsed = storageSpaceUsed;
			channel->storageSpaceUsed -= size;
			cout<<"\nchannel->storageSpaceUsed--->"<<channel->storageSpaceUsed<<channel->freeSpaceInDiskForChannel;
			DeleteFile((char *)videosName[i].c_str());
		}
	}
	dBA->Close();
	delete dBA;
}

