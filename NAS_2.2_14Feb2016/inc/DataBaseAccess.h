#ifndef _DATABASEACCESS_
#define _DATABASEACCESS_

#include "mysql.h"
#include <pthread.h>
#include <netdb.h>
#include <errno.h>
#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "ctime"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "arpa/inet.h"
#include <dirent.h>
#include <sys/statvfs.h>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <vector>
#include "CommonClass.h"


#define QUERY_RECURSION_COUNT 10

#define MYSQL_ERROR_SERVER_GONE	2006
#define MYSQL_ERROR_CANT_CONNECT 2003
#define MYSQL_ERROR_UNKNOWN_DB	1049
#define MYSQL_ERROR_ACCESS_DENIED 1045
#define MYSQL_ERROR_CONNECTION_LOST 2013

using namespace std;

class DataBaseAccess {

private:

	string serverName;
	string hostName;
	string password;
	int portNumber;
	string schemaName;
	MYSQL *mySqlConnection;
	char * serverIp;
	pthread_mutex_t database_mutex;

public:
	DataBaseAccess();
	~DataBaseAccess();

	void PrintException (string className,string functionName,exception e);

	bool Open();
	bool OpenRecursion ();
	bool ExecuteQueryWithConnectionOpenRecursion (char *query);
	bool ExecuteQueryWithOutConnectionOpenRecursion (char *query);
	bool StartQueryRecursion (char *query);
	void Close();
	bool AuthenticateUser(char* username,char* password,int* UID);
	bool GetUGroupInfoFromUID(int UID,vector<int> *UGID,vector<string> *UGName);
	bool GetCGroupInfoFromUID(int UID,vector<int> *CGID, vector<string> *CGName);
	bool GetAllChannel(vector<int> *CID,vector<string> *ChannelName, vector<string> *RTSP, vector<int> *DID,vector<int> *status, vector<int> *storagestatus,vector<string> *RetrieverMode, vector<string> *ChannelMode,vector<string> *StoragePath, vector<string> *StorageSpace,vector<string> *StorageSpaceUsed, vector<string> *StorageFileType,vector<string> *ReStream, vector<string>*deviceIP , vector<string>*cameraIP , vector<string>*userName , vector<string>*portNumber , vector<string>*password, vector<string>*ptzControl, vector<string>*motion, vector<string>*description, vector<string>*make, vector<string>*model);
	bool GetAllChannelUsingDID(vector<int> *CID, vector<string> *ChannelName,vector<string> *RTSP,vector<int> *DID,vector<int> *status,vector<int> *storagestatus,vector<string> *RetrieverMode,vector<string> *ChannelMode,vector<string> *StoragePath,vector<string> *StorageSpace,vector<string> *StorageSpaceUsed,vector<string>	*StorageFileType,vector<string> *ReStream, vector<string>*deviceIP, int DIDInfo, vector<string>*cameraIP , vector<string>*userName , vector<string>*portNumber , vector<string>*password, vector<string>*ptzControl, vector<string>*motion, vector<string>*description , vector<string>*make, vector<string>*model);
	bool GetChannelInfoFromCGID(int CGID, vector<string> *CID,vector<string> *ChannelName, vector<string> *RTSP, vector<string> *DID,vector<string> *status, vector<string> *storagestatus,vector<string> *RetrieverMode, vector<string> *ChannelMode,vector<string> *StoragePath, vector<string> *StorageSpace,vector<string> *StorageSpaceUsed, vector<string> *StorageFileType,vector<string> *ReStream,vector<string>*cameraIP , vector<string>*userName , vector<string>*portNumber , vector<string>*password, vector<string>*ptzControl, vector<string>*motion, vector<string>*description, vector<string>*make, vector<string>*model );
	bool GetUserInfoFromUID(int UID,vector<string> *UName,vector<string> *UserName, vector<string> *Password,vector<string> *UserRole,vector<string> *Email,vector<string> *Mobile,vector<string> * Status);
	int GetSystemDID();
	string GetSystemIPFromDID(int DID);
	bool GetVIDForChannelAccordingTime( vector< string > &videoName, vector<int> &VID,vector<string> &VideoStartTime, vector<string> &VideoEndTime ,int CID, int DID,char* StartTime, char* EndTime);
	void GetVIDForSelectChannelAccordingTime( vector< string > &videoName, vector<int> &VID,vector<int> CID, vector<int> DID,char* StartTime, char* EndTime);
	int InsertUserInfo( int UID,char* name, char *userName, char *password,int userRole,char* email,char* mobile,int DID,bool status);
	int InsertDeviceInfo(int DID, char* DeviceName, char *DeviceIP,int Status,int Owner,char* MacId, bool isBackUp );
	int InsertUGroupInfo( char* UGName,int DID, int UGID );
	int InsertCGroupInfo(int CGID, char* CGName,int DID);
	int InsertUserUGroupRelation(int UUGID,int UID,int UGID,int DID);
	int InsertchannelCGroupRelation(int CCGID, int CID,int CGID,int DID,int UID);
//	int InsertchannelCGroupRelation(int CID,int CGID,int DID,int UID);
	int InsertUGroupCGroupRelation(int UGCGID, int UGID,int CGID,int DID,int UID);
	int InsertChannelInfo(int CID ,char* ChannelName, char *RTSP,int Status,int DID,int RetrieverModeValue,int ChannelModeValue,char* StoragePath, char* StorageSpaceValue,char* StorageSpaceUsedValue,char* StorageFileTypeValue,int StorageStatusValue,int ReStreamValue);
	bool GetAllActiveChannelInfo(vector<int> *CID, vector<string> *ChannelName,vector<string> *RTSP,vector<int> *DID, vector<string> *StoragePath,vector<int> *Retrivermode ,  vector<string> *StorageSpace,vector<string> *StorageSpaceUsed,vector<string> *StorageFileType,vector<int> *StorageStatus,vector<int> *ReStream,int SysDID,vector<string> *IpAddress,vector<string> *portNumber , vector<string> *userName,vector<string> * password);
	bool GetActiveChannelInfoFromCID(int StartCID,vector<int> *CID, vector<string> *ChannelName,vector<string> *RTSP,vector<int> *DID, vector<string> *StoragePath,vector<int> *Retrivermode, vector<string> *StorageSpace,vector<string> *StorageSpaceUsed,vector<string> *StorageFileType,vector<int> *StorageStatus,vector<int> *ReStream,int SysDID,vector<string> *IpAddress,vector<string> *portNumber , vector<string> *userName,vector<string> * password);
	bool DeleteVideoFromTable(const char * tableName, int VID );
	void InsertVideoInfo(char *videoName,int CID, int DID,char *startDateTime, char *endDateTime, double videoSize);
	void GetVideoSize( int VID , double & size );
	bool DeleteVideoFromAllTable( int VID , double & size);
	void UpdateChannelStorageSpaceUsed( int CID,int DID, double storageSpaceUsed);
	void GetVideoNameAndVID( vector< string > &videoName, vector<int> &vidInfo ,int CID, int DID);
	void GetAllUser(vector<string> *UID,vector<string> *Name,vector<string> *UName, vector<string> *Password,vector<string> *UserRole,vector<string> *Email,vector<string> *Mobile,vector<string>* Status);
	void GetAllUGroup(vector<string> *UGID,vector<string> *UGName);
	void GetAllCGroup(vector<string> *CGID,vector<string> *CGName);
	void GetAllVideoInfo( vector<int> *VID, vector<string> *VName, vector<int> *CID, vector<int> *DID);
	void GetAllDeviceInfo( vector<string> *DID,vector<string> *DName,vector<string> *DeviceIP,vector<string> *Status,vector<string> *Owner,vector<string> *MacAddress,vector<string> *backup,  bool withOweres);
	int UpdateUserInfo(char* UID,char* name, char *userName, char *password,char* userRole,char* email,char* mobile,char* status);
	void GetAllUserUGroupRelation( vector<string> *UID,vector<string> *UGID,vector<string> *UGName);
	int DeleteUserUGroupRelation(int UID,int UGID);
	int UpdateDumpingStatus(char* CID,char* DID,int status);
	int UpdateDeviceInfo(char* DID,char* DeviceName, char *DeviceIP,char* Status, char * backUP);
	void GetAllChannelCGroupRelation( vector<string> *CID,vector<string> *CGID,vector<string> *CGName);
	int DeleteChannelCGroupRelation(int CID,int CGID);
	int UpdateChannelInfo(int DID,char * CID, char * CName, char * MAKE,char * MODEL,char * USERNAME,char * PASSWORD,char *IPADDRESS,char *PORTNUMBER,char *RTSP,char *STORAGESPACE,char *STORAGESPACEUSED,char *STORAGEFILETYPE,char *STORAGESTATUS ,char *STREAMMODE ,char *PTZCONTROL ,char *RETRIVERMODE,char *REPLICAMODE ,char *STATUS,char *MOTION, char *DESCRIPTION);	//Ashu
	bool GetDeviceIP(char *deviceIp, int DID);
	void UpdateChannelInfoStatus(int sysDID);
	bool IsServerExist(char * deviceIP,char * deviceName, int &DID, char *getdeviceIP );
	bool isMacAddressExist(char * macAddress, char * deviceIP );
	bool InsertOwnerDeviceIp(char * macAddress, int DID,char * ip );
	int GetRowCount(const char * tableName);
    bool GetDeviceInfo(char * deviceName,char * deviceip, int &DID, char *macAddress, bool & backup);
	bool GetDeviceDetails( vector<int> &DID,vector<string> &macAddress,vector<string> &DeviceIP, vector<int> &status,vector<string> &deviceName,vector<int> &backup);
	bool GetDeviceID( int &DID, char *DeviceIP);
	bool CheckAlreadyExist(char * value ,char * tableName, char * attributeName );
	int UpdateUGroupInfo(int UGID, char* UGName,int DID);
	int UpdateCGroupInfo(int CGID, char* CGName,int DID);
	bool CheckChannelCGroupInfoExist(char * CGID ,char * CID, char * UID,int DID );
	bool RemoveUserFromUGID( int UGID, vector<int> removeUID);
	bool AddUserToUGID(int UGID, vector<int> addUID, int DID);
	bool GetAllAvailableNASDetail( vector<string> & nasIP );
	bool AddChannelToCGID(int CGID, vector<int> addCID, int DID, int UID);
	bool RemoveChannelFromCGID( int CGID, vector<int> removeCID, int UID);
	int UpdateChannelCGroupRelation(int CCGID, char* CID,char* CGID,int DID,char* UID);
	int UpdateUserUGroupRelation(int UUGID, char* UID,char* UGID,int DID);
	bool CheckUserUGroupInfoExist(char * UGID ,char * UID,int DID );
	bool GetChannelInfoFromCGIDInteraction(bool isAllGroup,int CGID,vector<string> *CID );

	//Relation UGroup with Cgroup
	bool InsertCGroupRelationWithUGroup(int CGID,vector<int> addUGID,vector<int>removeUGID, int UID, int DID);
	bool InsertUGroupRelationWithCGroup(int UGID,vector<int> addCGID,vector<int>removeCGID, int UID, int DID);
	bool InsertCGroupUGroupRelation(int CGID, vector<int> addUGID,int DID, int UID );
	bool InsertUGroupCGroupRelation(int UGID, vector<int> addCGID,int DID, int UID);
	bool RemoveCGroupUGroupRelation(int CGID, vector<int> removeUGID,int DID, int UID);
	bool RemoveUGroupCGroupRelation(int UGID, vector<int> addCGID,int DID, int UID);
	bool GetUGroupCGroupRelation(vector<string> &UGID,vector<string> &CGID,vector<string> &CGNAME,vector<string> &UGNAME);
	void GetAllFromChannelRTSP( vector<int> *RESTREAMID,vector<int> *CID,vector<int> *DID,vector<string> *width,vector<string> *height,vector<string> *bitrate,vector<string> *framerate);
	int InsertReStream( char *CID, char *DID, char *width,char * height, char * frameRate,char * bitRate );
	bool InactiveChannel(char * CID );
	void InsertIntoProcessMonitor(double ProcessCpuUtilisation,double OverallCpuUtilisation,int ProcessVirtualMemoryUtilisation,int ProcessPhysicalMemoryUtilisation, unsigned long long int DataSent,unsigned long long int  DataReceived);
	bool GetMinProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive, const char * startDate , const char * startTime , const char * endDate , const char * endTime);
	bool GetMaxProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive, const char * startDate , const char * startTime , const char * endDate , const char * endTime);
	bool GetAvgProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive, const char * startDate , const char * startTime , const char * endDate , const char * endTime);
	bool GetProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive,vector<string> &entryDate,vector<string> &entryTime, const char * fillterValue , const char * startDate , const char * startTime , const char * endDate , const char * endTime);
	int CheckUGroupCGroupInfoExist( char *UGID ,char * CGID, int DID, int LoggedUID );
	bool UpdateUGroupCGroupRelation(int UGCGID, char* UGID, char* CGID,int DID, char* UID);
	bool GetOffLinePerformanceMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive, const char * startDate , const char * startTime , const char * endDate , const char * endTime, int DID, int count );

	bool GetUGroupCGroupRelationOnlyOwners(vector<string> &UGID,vector<string> &CGID,vector<string> &UGCGID,vector<string> &UID, vector<string> & DID);
	void GetAllCGroupOnlyOwner(vector<string> &DID, vector<string> *CGID,vector<string> *CGName);
	void GetAllUGroupOnlyUser(vector<string> &DID, vector<string> *UGID,vector<string> *UGName);
	bool GetAllChannelOnlyOwner(int sysDID, vector<int> *CID, vector<string> *ChannelName,vector<string> *RTSP,vector<int> *DID,vector<int> *status,vector<int> *storagestatus,vector<string> *RetrieverMode,vector<string> *ChannelMode,vector<string> *StoragePath,vector<string> *StorageSpace,vector<string> *StorageSpaceUsed,vector<string>	*StorageFileType,vector<string> *ReStream, vector<string>*deviceIP,vector<string>*ptzControl,vector<string>*motion,vector<string>*description,vector<string>*ipAddress,vector<string>*PortNumber,vector<string>*userName,vector<string>*Password,vector<string>*make,vector<string>*model );
	void GetAllUserUGroupRelationOnlyOwners( vector<string> & DID,  vector<string> *UUGID,vector<string> *UID,vector<string> *UGID,vector<string> *UGName);
	void GetAllUserOnlyOwners(vector<string> &DID,vector<string> *UID,vector<string> *Name,vector<string> *UName, vector<string> *Password,vector<string> *UserRole,vector<string> *Email,vector<string> *Mobile,vector<string> *Status);
	void GetAllChannelCGroupRelationOnlyOwners( vector<string> &DID, vector<string> *CCGID,vector<string> *CID,vector<string> *CGID,vector<string> *CGName,vector<int> *UID);
	bool GetLiveProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive );
	bool GetAllChannelDetailsForDID( int DID , vector<std::string > & cName, vector<bool> & dumpingStatus ,vector<bool> & activeStatus ,vector<bool> & resendStatus ,vector<bool> & retrievelStatus,vector<bool> & channelModeStatus  );
	bool GetBooleanValue( char * value );


	/*********   Active User   ***************/
	int CheckActiveUser(int UID,int DID);
	int InsertActiveUserInfo(int UID, int DID,const char *clientIP,int status);
	bool UpdateActiveUserInfo(int AUID,const char *clientIP,int status);
	bool GetActiveUserInfo( int DID, vector<string> &Uname, vector<int> &Urole, vector<string> &lastLoginTime,vector<string> &IP  );
	bool UpdateActiveUserStatus(int AUID,int status);
	bool CheckUserLoginStatus( int AUID);
	bool UpdateActiveUserStatusFalse();
	/*****************************************/
	void UpdateDeviceBackupStatus(int sysDID , int status );
	int CheckUserInfo( char *UserName,char *UID );
	bool CheckIDExistInTable( const char * tableName , const char * columnName, int value );
	/**************************29 jan 2016***************************/
	void GetAllChannelWithoutDetails(  vector < std::string >&cID , vector < std::string >&cName, const char * searchText);
	void GetAllChannelGroupWithoutDetails(  vector < std::string >&CGID , vector < std::string >&UGName, const char * searchText);
	void GetAllUserWithoutDetails(  vector < std::string >&UID , vector < std::string >&UName, const char * searchText);
	void GetAllUserGroupWithoutDetails(  vector < std::string >&UGID , vector < std::string >&UGName, const char * searchText);
	void GetAllDeviceWithoutDetails(  vector < std::string >&DID , vector < std::string >&DName, const char * searchText);
	void GetUGIDRelatedToUID( vector< int > &UGID , vector< int > UID );
	void GetUIDRelatedToUGID( vector< int > &UID , vector< int > UGID );
	void GetCGIDRelatedToUGID( vector< int > &CGID , vector< int > UGID );
	void GetUGIDRelatedToCGID( vector< int > &UGID , vector< int >CGID );
	void GetCIDRelatedToCGID( vector< int > &CID , vector< int > CGID );
	void GetCGIDRelatedToCID( vector< int > &CGID , vector< int > CID );
	void GetDIDRelatedToCID( vector< int > &DID , vector< int > CID );
	void GetCIDRelatedToDID( vector< int > &CID , vector< int > DID );
	void GetNameFromTable(vector< int > &UID,vector< string > &UName ,const char * tableName, const char * getColumnName, const char * conditionColumnName );
	void GetName(vector< int >&UID,vector< int >&UGID,vector< int >&CGID,vector< int >&CID,vector< int >&DID,vector< string >& UName,vector< string >&UGName,vector< string >&CGName,vector< string >&CName,vector< string >&DName);
	void GetRelation( vector < int > relationTo, vector < int > relationFrom, vector < int > &IDLinkFrom, vector < int > &IDLinkTo, const char *tagLinkFrom, const char *tagLinkTo, const char *tableName );
	void GetChannelInfo(char * channelName , int &CID , int &DID);
	/****************************************************************/
	void GetStorageSpaceUsed(int CID, int DID,double &storageSpaceUsed);
	int NewInsertChannelInfo(int CID, char * CName, int DID, char * MAKE,char * MODEL,char * USERNAME,char * PASSWORD,char *IPADDRESS,char *PORTNUMBER,char *RTSP,char *STORAGEPATH,char *STORAGESPACE,char *STORAGESPACEUSED,char *STORAGEFILETYPE,char *STORAGESTATUS ,char *STREAMMODE ,char *PTZCONTROL ,char *RETRIVERMODE,char *REPLICAMODE ,char *STATUS,char *MOTION, char *DESCRIPTION);
	void GetAllCIDANDDID( vector<int> &tempCID,int tempDID , vector<string> &outputPath);
	void GetChannelUsedSpace(int tempCID,int sysDID,double &channelUsedSize);
	int GetRowCountInVideoInfo(int tempCID,int sysDID);
	bool CheckChannelExists(const char * CID, int sysDID);
	void GetDIDFromCID( int &mainDID,char * CID);
	void GetOutputFolder(char* outputPath,int CID, int sysDID );
	void GetAllVideosForParticularCID( int tempCID, vector<string> &vName );
	bool FileExist(const char *fileName );
	bool GetAllChannelGroupForUID( int UID, vector <std::string> &CGID,vector <std::string> & CGNAME);
	bool GetChannelForCGID( std::string CGID,vector<std::string> &CNAME);
	bool GetChannelID( char *CGName,char * CName, int &CID,int &DID );
	void GetUserID( const char * UserName, int &UID);
	bool InsertImageInfo(char *imageName,char * imagePath,int imageRelatedCID,int imageRelatedDID,int imageRelatedUID);
	void GetCreatedRtsp( const char *CID,   char *createdRTSP);
	bool InsertCreatedRtsp(int CID, int DID ,   char * createdRtsp);
	bool ClearRtspInfo( int CID );
	bool UpdateRtspInfo(char * CID, char * DID , char * createdRtsp);
	void ClearRtspTable();
	void GetAllRtspInfo( vector<string> &CID,vector<string> &DID,vector<string> & rtsp );
	int IsChannelsExist( int DID );
	bool ReplaceDID(const char * tableName, const char * oldDID, int newDID);

	//****************************************************/
	void GetAllUGroupForDID(const char * DIDInfo,vector<string> *UGID,vector<string> *UGName);
	void GetAllCGroupForDID(const char * DIDInfo,vector<string> *CGID,vector<string> *CGName);
	void GetAllUserForDID(const char *DIDInfo,vector<string> *UID, vector<string> *Name,vector<string> *UName, vector<string> *Password,vector<string> *UserRole, vector<string> *Email, vector<string> *Mobile,vector<string> *Status) ;
	bool GetAllChannelForDID(const char *DIDInfo ,vector<int> *CID,vector<string> *ChannelName, vector<string> *RTSP, vector<int> *DID,vector<int> *status, vector<int> *storagestatus,vector<string> *RetrieverMode, vector<string> *ChannelMode,vector<string> *StoragePath, vector<string> *StorageSpace,vector<string> *StorageSpaceUsed, vector<string> *StorageFileType,vector<string> *ReStream, vector<string>*deviceIP , vector<string>*cameraIP , vector<string>*userName , vector<string>*portNumber , vector<string>*password, vector<string>*ptzControl, vector<string>*motion, vector<string>*description, vector<string>*make, vector<string>*model);
	void GetAllChannelCGroupRelationForDID(const char * DIDInfo, vector<string> *CID,vector<string> *CGID, vector<string> *CGName, vector<string> *UID);
	void GetAllUserUGroupRelationForDID( const char * DID,vector<string> *UID,vector<string> *UGID, vector<string> *UGName );
	bool GetUGroupCGroupRelationForDID(const char * DID,vector<string> &UGID,vector<string> &CGID, vector<string> &CGNAME, vector<string> &UGNAME);
	void GetActiveUID(vector<string> &UID, int DID);
	void DeleteTableEntryForDID( const char * tableName ,const char *DIDInfo );
	void DeleteTableEntry( const char * tableName ,const char * columnName, int columnValue );
	/****************************************************/
};


#endif
