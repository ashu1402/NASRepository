/*
 * Failover.h
 *
 *  Created on: 24-Feb-2016
 *      Author: ubuntu
 */

#ifndef FAILOVER_H_
#define FAILOVER_H_

#include "DataBaseAccess.h"
#include "CommonClass.h"
#include <dirent.h>
#include "Markup.h"
#include "iostream"
#include "BootLoader.h"

using namespace std;


class Failover
{
public:
	CMarkup *xml;
	DataBaseAccess* dataBaseObject;
	CommonClass* CommonClassObj;
	pthread_t failoverThread;
	bool isBreak;
	BootLoader* bootLoader;
public:
	Failover(BootLoader* );
	~Failover();
	void StartThread(Failover *failoverObj);
	bool CheckServerStatus();
	static void *StartThreadMain (void *para);
	bool CreateConnectionFailOver ( const char * address );
	void MakeReplica( const char *DeviceIP , const char * DID ,DataBaseAccess * dBA, vector<string> DevicesIP);
	void ReplaceDownServerDIDWithBackUPServerDID(const char *DID, int sysDID,DataBaseAccess * dBA, vector<string> deviceIP );
	bool WaitForThreadCompletion();
	void GetInformationAboutFromAllOtherServer( vector<string> DeviceIP, const char * DID );
	void CreateFailOverXml(char * xmlData);
	bool FindElement( string mainString, string subString );
	void CreateXML(DataBaseAccess *dBA,const char *DID, int sysDID );
	void SendReceiveXML(char * xmlData , int socketID);
	void CreateXmlForChannel(DataBaseAccess *dBA,const char *DID,char * xmlData, int sysDID);
	void CreateXMLForUser(DataBaseAccess *dBA,const char *DID,char * xmlData);
	void CreateXMLForCgroup(DataBaseAccess *dBA,const char *DID,char * xmlData);
	void CreateXMLForUGroup(DataBaseAccess *dBA,const char *DID,char * xmlData);
	void CreateXMLForChannelCgroup(DataBaseAccess *dBA,const char *DID,char * xmlData);
	void CreateXMLForUserUGroup(DataBaseAccess *dBA,const char *DID,char * xmlData);
	void CreateXMLForUGroupCGroup(DataBaseAccess *dBA,const char *DID,char * xmlData);
	void CheckForXMLHeader( const char * actionName );
};


struct failOverThreadData {
	//int id;
	Failover * failover;
};


#endif /* FAILOVER_H_ */
