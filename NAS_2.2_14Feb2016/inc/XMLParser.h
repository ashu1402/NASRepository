#ifndef _XMLPARSER_
#define _XMLPARSER_

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "../rapidxml/rapidxml.hpp"
#include "Interaction.h"
#include "XMLCreator.h"
#include "DataBaseAccess.h"
#include "Retrieve.h"
//#include "CommonClass.h"
#include "VOD.h"
#include "Download.h"
#include "Notification.h"
#include "cv.h"
#include "opencv2/opencv.hpp"
#include <fstream>
#include "Failover.h"
#include "Interaction.h"


#define STORAGEPATH "/home/ubuntu/Frame"

using namespace cv;
using namespace std;
using namespace rapidxml;

class XMLParser
{
private:
	int sockfd;
	XMLCreator* XMLCreatorObj;
	DataBaseAccess* DataBaseAccessObj;
	Retrieve * RetrieveObj ;
	CommonClass* CommonClassObj;
	VOD* VODObj;
	Notification * notification;
	Failover *failover;
	Interaction * interaction;
	char *outputPath;
	int LoggedUID;
	int LoggedDID;
	char *clientIp;
	int imageRelatedCID, imageRelatedDID, imageRelatedCGID, imageRelatedUID;
	bool isAndroid;


public:
	XMLParser(int fd,Retrieve* Obj,VOD* vodObj, Notification *notifi, char*cIP, bool isAndroid, Failover * failoverObj,Interaction * interaction );
	~XMLParser();
	string getFileContents(char* filename);
	xml_node<>* getNodeByName(const string& nodeName, rapidxml::xml_node<>* parentNode);
	bool Action(xml_node<>* node,char * fileContains);
	bool Login(xml_node<>* node);
	bool XMLDakshaImaging(xml_node<>* node,char * fileContains, int &interactionValue);
	void parse();
	bool AddUser(xml_node<>* node);
	bool AddServer(xml_node<>* node);
	bool AddDevice(xml_node<>* node);
	bool AddChannel(xml_node<>* node,char * fileContains);
	bool AddUGroup(xml_node<>* node);
	bool AddCGroup(xml_node<>* node);
	bool AddUserUGroupRelation(xml_node<>* node);
	bool AddChannelCGroupRelation(xml_node<>* node);
	bool AddUGroupCGroupRelation(xml_node<>* node);
	bool FetchAllUser(xml_node<>* node);
	bool FetchAllUGroup(xml_node<>* node);
	bool FetchAllDevice(xml_node<>* node);
	bool FetchAllUserUGroupRelation(xml_node<>* node);
	bool FetchAllVideo(xml_node<>* node);
	bool FetchAllChannel(xml_node<>* node);
	bool FetchAllCGroup(xml_node<>* node);
	bool FetchAllChannelCGroupRelation(xml_node<>* node);
	bool FetchAllUserInteraction(xml_node<>* node);
	bool FetchAllUGroupInteraction(xml_node<>* node);
	bool FetchAllCGroupInteraction(xml_node<>* node);
	bool FetchAllChannelInteraction(xml_node<>* node);
	bool FetchAllUserUGroupRelationInteraction(xml_node<>* node);
	bool FetchAllChannelCGroupRelationInteraction(xml_node<>* node);
	bool FetchAllUGroupCGroupRelation(xml_node<>* node);
	bool recvData(void* data,int size, int sockFD);
	bool recvData(void* data,int size);
	bool sendData(int size,const void *data,int fd);
	bool StartRestream(xml_node<>* node);
	bool StartVOD(xml_node<>* node);
	bool StartDumping(xml_node<>* node);
	bool StopDumping(xml_node<>* node);
	bool StartDownload(xml_node<>* node);
	bool UpdateUser(xml_node<>* node);
	bool UpdateDevice(xml_node<>* node);
	bool UpdateChannel(xml_node<>* node,char * fileContains);
	void RestartChannel(int StartCID);
    bool EditUGroup(xml_node<>* node);
	bool EditCGroup(xml_node<>* node);
	bool InteractionXml(xml_node<>* node);
	bool Response(xml_node<>* node);
	bool ParseInteractionResponse(xml_node<>*node);
	bool LookingForServer(char * address , int port, int &sockfd );
	bool CheckForError(char *receivedXmlData, int &CID);
	bool ParseSuccessChannel(char * receivedXmlData,int &CID);
	bool GetDeviceInfo(int &enteredDeviceID,char *macValue, char * DeviceIP, int sockFD,char *deviceName,bool &backup);
	bool DeviceInfo(xml_node<>* node);
	bool ParseDeviceInfo(char *macValue, int &enteredDeviceID,char *xmlData, char * deviceName, bool &backUP);
	bool Interactions(xml_node<>* node);
	bool InteractionResponse(xml_node<>* node);
	void CreateNewCID(int DID, int &CID);
	void CreateUniqueUID(int DID, int &UID);
	void CreateUniqueCGID( int DID, int &CGID );
	void CreateUniqueUGID( int DID, int &CGID );
	void CreateUniqueUUGID(int DID,int &UUGID);
	void CreateUniqueCCGID(int DID,int &CCGID);
	void CreateUniqueUGCGID(int DID,int &UUGCCGID);
	bool GetAllUGroupRelationInfo(int &UUGID,xml_node<>* UGroupRelation,vector<int> &addCGID,vector<int> &removeCGID);
	bool GetAllCGroupRelationInfo(int &CCGID,xml_node<>* CGroupRelation,vector<int> &addUGID,vector<int> &removeUGID);
	bool FetchUGroupCGroupRelation(xml_node<>* node);
	bool UGroupCGroupRelation(xml_node<>* node);
	bool UpdateChannelGroupInfo(xml_node<>* node,char *CID, int DID,int LoggedUID);
	bool SendXmlToOtherNAS( int DID , char * CID, char * fileContents );
	bool SendXmlToNASUpdateChannel( int DID , char * CID, char * fileContents );
	bool UpdateChannelInSameMachine(xml_node<>* node, char *CID, int DID);
	bool StartProcessMonitoring(xml_node<>* node, char * fileContents);
	bool GetAllChannelDetailsForDID(xml_node<>* node, char * fileContents);
	bool DeviceInfoInteraction(xml_node<>* node);
	bool OfflinePerformance(xml_node<>* node, char * fileContents);
	bool ExtractUserInfo(xml_node<>*node);
	bool ExtractUserGroupInfo(xml_node<>*node);
	bool ExtractChannelInfo(xml_node<>*node);
	bool ExtractChannelGroupInfo(xml_node<>*node);
	bool ExtractChannelCGroupInfo(xml_node<>*node);
	bool ExtractUserUGroupInfo(xml_node<>*node);
	void CloseUserSession();
	bool ActiveUser(xml_node<>* node, char * fileContents);
	bool FetchStorageInfo(xml_node<>* node, char * fileContents);

	/*************************29 jan 2016****************************/
	bool FetchAllDeviceWithoutDetail(xml_node<>* node );
	bool FetchAllChannelWithoutDetail(xml_node<>* node );
	bool FetchAllChannelGroupWithoutDetail(xml_node<>* node );
	bool FetchAllUserWithoutDetail(xml_node<>* node );
	bool FetchAllUserGroupWithoutDetail(xml_node<>* node );
	bool GetGraphInfo(xml_node<>* node );
	void GetGraphDetail(xml_node<>* pNode , const char * tagName ,  vector<int> &value );
	/***************************************************************/
	bool LoginFromQT(xml_node<>* node );
	bool ContinuousMove(xml_node<>* node );
	bool RelativeMove(xml_node<>* node );
	bool VerificationCamera( xml_node<>* node );
	bool AddChannelSub(xml_node<>* node , int DID, int &CID );
	bool NotificationEvent(xml_node<>* node );
	bool SendInactiveChannelXml(int sockFD, char *CID, char *DID);
	bool GetChannelInfo(xml_node<>* node, char * fileContents );
	bool StoreImage(xml_node<>* node , char * fileDescriptor);
	bool SetImageInfo(xml_node<>* node );
	void ReadImageData(xml_node<>* node, char *outputPath, char * fileDescriptor, char * imageName);
	void ParserForAndroid();
	void ParserForDesktop();
	bool RtspInfo(xml_node<>* node);
	bool StartInteraction(xml_node<>* node);
};

#endif
