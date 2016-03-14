#include "BootLoader.h"

BootLoader::BootLoader(Retrieve* Obj)
{
	DataBaseAccessObj = new DataBaseAccess();
	RetrieveObj = Obj;
	CommonClassObj = new CommonClass();
	DataBaseAccessObj->UpdateActiveUserStatusFalse();
	DataBaseAccessObj->Close();
	delete DataBaseAccessObj;
}

BootLoader::~BootLoader()
{


}


bool BootLoader::CheckDirectoryExistance(char * directoryPath) {

	try{
		struct stat st;
		if (stat(directoryPath, &st) == 0)
			return true;
		else
			return false;

	}
	catch(cv::Exception e ){
		////cout<<"Exception caught in CheckDirectoryExistance Function"<<e.what()<<endl<<endl;
	}

	catch(std::exception e ){
		////cout<<"Exception caught in maiCheckDirectoryExistancen Function"<<e.what()<<endl<<endl;
	}
	return false;

}

void BootLoader::CreateDirectory(char *directoryPath) {
	try{
		if ( ! (mkdir(directoryPath, 0777) == 0) ) {
			printf("Unable to Create Directory");
		}

	}
	catch(cv::Exception e ){
		////cout<<"Exception caught in CreateDirectory Function"<<e.what()<<endl<<endl;
	}

	catch(std::exception e ){
		////cout<<"Exception caught in CreateDirectory Function"<<e.what()<<endl<<endl;
	}
}


void BootLoader::GetAllFileForDirectory(char * directoryPath, vector<string> &fileName){

DIR *dpdf;
struct dirent *epdf;

dpdf = opendir(directoryPath);
if (dpdf != NULL){
   while (epdf = readdir(dpdf)){
      fileName.push_back(epdf->d_name);
   }
  }
}

void BootLoader::CheckVideoExistance(char *directory, int tempCID,int sysDID){

	vector<string>  fileName;
	GetAllFileForDirectory(directory, fileName);
	char filePath[SIZEOFCHARARRAYBIGGER];
	for ( int i = 0 ; i < (int)fileName.size(); i++ ){
		sprintf( filePath,"%s/%s", directory, fileName[i].c_str());
		if ( !DataBaseAccessObj->FileExist(filePath)){
			if ( remove(filePath) )
				cout<<"\nRemoved"<<endl;
		}
	}
}

void BootLoader::checkConsistency(){

	vector<string> VName,outputPath;
	vector<int> VID,CID,DID, tempCID, tempDID;
	char directory[SIZEOFCHARARRAYBIGGER];
	DataBaseAccessObj->GetAllVideoInfo(&VID,&VName,&CID,&DID);
	int sysDID = DataBaseAccessObj->GetSystemDID();
	DataBaseAccessObj->GetAllCIDANDDID( tempCID, sysDID , outputPath);
	double channelUsedSize = 0;
	int rowCount = 0;
	for ( int i = 0 ; i < (int)tempCID.size() ; i++ ){
		 channelUsedSize = 0;
		 rowCount = DataBaseAccessObj->GetRowCountInVideoInfo(tempCID[i],sysDID);
		 if ( rowCount > 0 ){
			 sprintf( directory,"%s/CID%dDID%d",outputPath[i].c_str(),tempCID[i],sysDID);
		 	 CheckVideoExistance(directory, tempCID[i],sysDID);
		 	 DataBaseAccessObj->GetChannelUsedSpace(tempCID[i],sysDID,channelUsedSize);
		 }
		 DataBaseAccessObj->UpdateChannelStorageSpaceUsed(tempCID[i],sysDID,channelUsedSize);
	}
	double size = 0, StorageSpaceUsed = 0;
	for(int i=0;i< (int)VID.size();i++){
		if(!CheckDirectoryExistance((char*)VName[i].c_str())){
			DataBaseAccessObj->GetStorageSpaceUsed(CID[i],DID[i],StorageSpaceUsed);
			DataBaseAccessObj->GetVideoSize(VID[i],size);
			DataBaseAccessObj->DeleteVideoFromTable("videoinfo",VID[i]);
			StorageSpaceUsed -= size;
			DataBaseAccessObj->UpdateChannelStorageSpaceUsed(CID[i],DID[i],StorageSpaceUsed);
		}
	}


}

void BootLoader::StartBoot()
{
	DataBaseAccessObj = new DataBaseAccess();

	vector<int> ReStream, StorageStatus, CID,DID,Retrivermode,restreamID;
	vector<string> IpAddress, portNumber , userName, password,ChannelName, RTSP,StoragePath,StorageSpace,StorageSpaceUsed,StorageFileType,width,height,bitrate,framerate;
	int SysDID = DataBaseAccessObj->GetSystemDID();

	checkConsistency();

	DataBaseAccessObj->GetAllActiveChannelInfo(&CID,&ChannelName,&RTSP,&DID, &StoragePath,&Retrivermode,&StorageSpace,&StorageSpaceUsed,&StorageFileType,&StorageStatus,&ReStream,SysDID,&IpAddress, &portNumber , &userName,& password);

	for(int i = 0;i < (int)CID.size();i++){
		struct channelInfo channelObj;
		CommonClassObj->copyChannelInfo(&channelObj,CID[i],ChannelName[i],RTSP[i],DID[i], StoragePath[i],Retrivermode[i],StorageSpace[i],StorageSpaceUsed[i],StorageFileType[i],StorageStatus[i],ReStream[i], IpAddress[i], portNumber[i], userName[i], password[i]);
		RetrieveObj->CreateNewChannel(channelObj);
	}

	sleep(5);
		CID.clear();
		DID.clear();

		DataBaseAccessObj->GetAllFromChannelRTSP(&restreamID,&CID,&DID,&width,&height,&bitrate,&framerate);
		for( int i = 0; i < (int)restreamID.size(); i++){
			RetrieveObj->AddReStreaming(CID[i],DID[i],(char*)width[i].c_str(),(char*)height[i].c_str(),(char*)framerate[i].c_str(),(char*)bitrate[i].c_str(),restreamID[i]);
		}

		DataBaseAccessObj->Close();
		delete DataBaseAccessObj;
}
