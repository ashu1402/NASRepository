#ifndef _XMLCREATOR_
#define _XMLCREATOR_

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "DataBaseAccess.h"
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"
#include <cstdlib>
#include "Markup.h"

using namespace std;
using namespace rapidxml;


class XMLCreator
{
public:
	CMarkup *xml;
	xml_document<> doc;
	DataBaseAccess* DataBaseAccessObj;
	xml_node<>* rootNode;
	int AUID;
public:
	XMLCreator();
	~XMLCreator();

	string convertToString();
	bool FindElement( string mainString, string subString );
	void CreateGetDeviceInfo(char *DeviceIP);
	void NewSuccessDeviceInfoAction( int DID,char *macAddress, char * deviceName, bool &backup);
	void NewFetchDeviceInfo();
	void CreateInteractionXml();
	void CreateInteractionResponse();
	void NewSuccessInteraction();
	void InteractionFailureAction(char* ErrorMsg, char* ErrorId,char* ActionName);
	void InteractionSuccessAction(char* ActionName, char* msg );
	void NewFetchUGroupCGroupRelation(bool isAllChannel);
	void NewUGroupCGroupInfo(const char *DID,const char *UGID,const char *CGID,const char *CGNAME , const char * UGNAME,const char *UGCGID, bool isAllChannel,const char * UID );
	void SuccessRestream(int restreamID,char * actionName );
	void CreateProcessMonitoringXML(int LoggedDID,  char * fillterValue ,  char * startDate ,  char * startTime ,  char * endDate ,  char * endTime);
	void NewProcessMontitoringInfo(const char *processCPU,const char *totalCPU,const char *virtualMemory,const char *physicalMemory,const char *dataSend, const char *dataReceive );
	void NewUserUGroupInfo(const char* DID,const char* UUGID,const char* UID,const char*  UGID,const char* UGName,bool isAllInfo );
	void CreateAllChannelDetails( int DID );
	void CreateOfflineAnalysisDetails( int DID, char * StartDate, char *StartTime,char *EndDate,char *EndTime );
	bool KeepActionUserInfo(int UID,int DID,const char *clientIP,int status );
	void CloseUserSession();
	void CreateActiveUserXML( char *DID );
	void NewActiveUserXML(const char *userName,int userRole, const char * lastLoginTime, const char *ip);
	void CreateStorageInfoXML(char *DID);
	string CheckBool ( bool value );
	void CreateAllChannelWithoutDetail( const char * searchText );
	void CreateAllChannelGroupWithoutDetail(  const char * searchText);
	void CreateAllUserGroupWithoutDetail(  const char * searchText);
	void CreateAllUserWithoutDetail(  const char * searchText);
	void CreateAllDeviceWithoutDetail(  const char * searchText);
	void CreateGraphInfo(vector<int>UID,vector<int>UGID,vector<int>CGID,vector<int>CID,vector<int>DID);
	void CreateRelationXML(vector<int>UID,vector<int>UGID,vector<int>CGID,vector<int>CID,vector<int>DID);
	void CreateNode(vector<int>UID,vector<string> UName,const char *IDTag, const char *nameTag, const char *parentTagName );
	void CreateRelationTableIntoXml( vector<int> relationTo,vector<int>relationFrom,vector<int>UID,vector<int>UGID ,const char * parentTag, const char *tagLink,const char *tagLinkTo, const char *tableName);
	void CreateRelationsXML(vector<int> IDLinkFrom,vector<int> IDLinkTo,const char *parentTag,const char *tagLinkFrom,const char *tagLinkTo);
	void NewSuccessActionWithID( char* ActionName, char* Msg, char* tagName , int tagValue );
	void NewSuccessActionWithString( char* ActionName, char* Msg, char* tagName , char * tagValue );
	void CreateInactiveChannelXml(char *CID,char *DID, char * xmlData);
	void NewRtspInfo();
	void ReInitializeXML();
	bool CreateXMLCreator(char *);
	string NewconvertToString();
	void NewChannelInfo( int CID, const char* CName,const char* RTSP,int  DID,int Status,int StorageStatus, const char* RetrieverMode,const char* ReplicaMode,const char* StoragePath,const char* StorageSpace,const char* StorageSpaceUsed, const char* StorageFileType,const char* streamMode, const char * deviceIP ,const char * cameraIP , const char * userName , const char * portNumber , const char * password, const char * ptzControl, const char * motion, const char * description, const char * make ,const char * model, bool isAndroidStatus);
	void NewFetchAllChannelCGroupRelation(bool isAllUser);
	void NewFetchAllUGroup(bool isAllUser);
	void NewFetchAllChannel(bool isAllUser);
	void NewUGroupInfo(const char* DID,const char* UGID, const char* UGName,bool isAllGroup);
	void NewVideoInfo( const char* VName,int VID, const char* VideoStartTime,const char* VideoEndTime);
	void NewChannelCGroupInfo(const char * DID,const char * CCGID,const char* CID,const char*  CGID,const char* CGName, int UID, bool isAllInfo);
	void NewDeviceInfo(	const char* DID,const char* DName,const char* DeviceIP,const char* Status,const char* Owner,const char*  MacAddress, const char * backup);
	void NewUserInfo(const char *DID,int UID,const char*  UName,const char* UserName,const char* Password,const char* UserRole,const char* Email,const char* Mobile,const char* Status, bool isAllUser );
	void NewFetchAllUser(bool isAllUser);
	void NewFetchAllUserUGroupRelation(bool isAllUser);
	void NewFetchAllCGroup(bool isAllUser);
	void NewCGroupInfo(const char * DID, int CGID, const char* CGName,bool isAllGroup, bool isAndroidStatus);
	void NewLoginSuccess(int UID, const char * clientIp, bool isAndroidStatus);
	void NewFetchAllDevice();
	void NewSuccessAction( char* ActionName, char* Msg);
	void NewSuccessAddUserAction(char* UID);
	void NewSuccessAddChannelAction(char* CID, char * deviceIp);
	void NewSuccessAddDeviceAction( char* DID, char * deviceName );
	void NewSuccessVODAction(vector<string> VName,vector<int> VID,vector<string> VideoStartTime,vector<string> VideoEndTime);
	void NewSuccessFetchAllVideoAction(vector<string> VName,vector<int> VID,vector<string> VideoStartTime,vector<string> VideoEndTime);
	void NewSuccessStartDownloadAction(const char* PortNumber);
	void NewErrorMsgXML(const char* ErrorMsg, const char* ErrorId );
	void NewFailureAction(const char* ErrorMsg, const char* ErrorId,const char* ActionName);
	void NewCreateLoginAction(char* UserName, char* Password);
	void CreateLoginSuccessXml( int UID);
	void CreateChannelInfo( char * rtsp);
	bool GetChannelRtsp(const char * CName,const char * CGName, char*createRtsp);
};



#endif
