/*
 * Failover.cpp
 *
 *  Created on: 24-Feb-2016
 *      Author: ubuntu
 */


#include "Failover.h"

Failover::Failover(BootLoader *bootLoaderObj){
	;//dataBaseObject = new DataBaseAccess();
	isBreak = false;
	bootLoader = bootLoaderObj;
	xml = new CMarkup();
}

Failover::~Failover(){

}


void Failover::StartThread(Failover *failoverObj){

	struct failOverThreadData * tempData = (struct failOverThreadData *)malloc(sizeof(struct failOverThreadData)*1);
	tempData->failover = failoverObj;

	if ( pthread_create( &failoverThread, NULL, StartThreadMain, (void *)(tempData) ) != 0 )
			cout<<"\n Thread is not able to create"<<endl;
		else
			cout<<"\n Thread created"<<endl;
	//delete tempData;
}

bool Failover::CreateConnectionFailOver ( const char * address  ){

	bool isRestart = false;
	int count = -1;
    int sockfd = -1;
    int port = tcpSocketPortNumber;
    Restart:
    count ++;
    if ( count > FAILURERecursion )
    	return false;
   if ( isRestart ){
	  cout<<"\n-----> Sleep Time --->"<< count * FAILURETIMEINTERVAL;
	   sleep( count * FAILURETIMEINTERVAL);
   }
    struct sockaddr_in serv_addr;
    struct hostent *server;
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0){
            cout<<"\nERROR opening socket\n";
            isRestart = true;
            goto Restart;
        }
        server = gethostbyname(address);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            isRestart = true;
            goto Restart;
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        serv_addr.sin_port = htons(port);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        	fprintf(stderr,"ERROR connecting");
        	isRestart = true;
        	goto Restart;
        }
        else{
        	close(sockfd);
        	return true;
        }
}

bool Failover::FindElement( string mainString, string subString ){

	int pos = -1;
	pos = mainString.find(subString);
	if ( pos != -1 )
		return true;
	return false;
}

void Failover::CreateFailOverXml( char * xmlData){
	try{
	xml = new CMarkup();
	xml->OutOfElem();
	if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
		xml->AddElem( "XMLDakshaImaging" );
	xml->IntoElem();
	xml->AddElem( "Interaction" );
	xml->AddAttrib("ActionName", "StartInteraction");
	xml->OutOfElem();
	xml->OutOfElem();

	sprintf ( xmlData,"%s",xml->GetDoc().c_str());
	//cout<<"\nxmlData---->"<< xmlData<<endl;
	//delete xml;
}
	catch(std::exception e){
		cout<<"Exception caught in Failover:CreateFailOverXml Function:"<<e.what()<<endl;
	}
}

void Failover::GetInformationAboutFromAllOtherServer( vector<string> DeviceIP, const char * DID ){

	CommonClass * commFunc = new CommonClass();
	char xmlData[SIZEOFCHARARRAYBIGGER];
	CreateFailOverXml(xmlData);
	//cout<<"\nStart Interaction---->"<<xmlData<<endl;
	for ( int i = 0 ; i < (int)DeviceIP.size() ; i++ ){
		commFunc->CreateConnectionClient((char*)DeviceIP[i].c_str(),tcpSocketPortNumber,xmlData);
	}
}

void Failover::CheckForXMLHeader( const char * actionName ){

		xml->OutOfElem();
			if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
				xml->AddElem( "XMLDakshaImaging" );
			xml->IntoElem();
			xml->AddElem( "Action" );
			xml->AddAttrib("Name", actionName);
			xml->IntoElem();

}

void Failover::CreateXmlForChannel(DataBaseAccess *dBA,const char *DIDInfo, char * xmlData , int sysDID ){
	vector<int> CID,DID,status,storagestatus;
	vector<string> deviceIP,ChannelName,RTSP,RetrieverMode,ChannelMode,StoragePath,StorageSpace,StorageSpaceUsed,StorageFileType,ReStream;
	vector<string>ptzControl,motion,description,ipAddress,portNumber,userName,password, make , model;
	dBA->GetAllChannelForDID(DIDInfo,&CID,&ChannelName,&RTSP,&DID,&status,&storagestatus,&RetrieverMode,&ChannelMode,&StoragePath,&StorageSpace,&StorageSpaceUsed,&StorageFileType,&ReStream,&deviceIP , &ipAddress, &userName ,  &portNumber ,  &password,  &ptzControl,  &motion, &description, &make ,&model );
	xml = new CMarkup();
		xml->OutOfElem();
					if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
						xml->AddElem( "XMLDakshaImaging" );
	for ( int i = 0 ; i < (int)ChannelName.size() ; i++ ){
		CheckForXMLHeader("AddChannel");
		xml->AddElem("ChannelName",ChannelName[i].c_str());
		xml->AddElem("IPAddress",ipAddress[i].c_str());
		xml->AddElem("UserName",userName[i].c_str());
		xml->AddElem("Password",password[i].c_str());
		xml->AddElem("PortNo",portNumber[i].c_str());
		xml->AddElem("RTSP",RTSP[i].c_str());
		xml->AddElem("StorageStatus",storagestatus[i]);
		xml->AddElem("StorageFileType",StorageFileType[i].c_str());
		xml->AddElem("StorageSpace",StorageSpace[i].c_str());
		xml->AddElem("RetrieveMode",RetrieverMode[i].c_str());
		xml->AddElem("Status",status[i]);
		xml->AddElem("PTZ",ptzControl[i].c_str());
		xml->AddElem("ReplicaMode",ChannelMode[i].c_str());
		xml->AddElem("Make",make[i].c_str());
		xml->AddElem("Model",model[i].c_str());
		xml->AddElem("Desc",description[i].c_str());
		xml->AddElem("StreamMode",ReStream[i].c_str());
		xml->AddElem("Motion",motion[i].c_str());
		xml->AddElem("StoragePath",ipAddress[i].c_str());
		xml->AddElem("DID",sysDID);
		xml->OutOfElem();

	}

	for ( int i = 0 ; i < CID.size() ; i++ ){
		dBA->DeleteTableEntry("channelcgrouprelation","CID",CID[i]);
		dBA->DeleteTableEntry("rtspinfo","CID",CID[i]);
	}

	dBA->DeleteTableEntryForDID("channelinfo", DIDInfo);
	string xml_Data = xml->GetDoc();
	sprintf ( xmlData,"%s",xml_Data.c_str());
}

void Failover::CreateXMLForUser( DataBaseAccess *dBA,const char *DIDInfo ,char * xmlData){
	vector<string> DID,UID,UName,UserName, Password,UserRole,Email, Mobile,Status;
	dBA->GetAllUserForDID(DIDInfo,&UID,&UName,&UserName,&Password,&UserRole,&Email,&Mobile,&Status);
	xml = new CMarkup();
		xml->OutOfElem();
					if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
						xml->AddElem( "XMLDakshaImaging" );
	for ( int i = 0 ; i < (int)UID.size() ; i++ ){
		CheckForXMLHeader("AddUser");
		xml->AddElem("UName",UName[i].c_str());
		xml->AddElem("UserName",UserName[i].c_str());
		xml->AddElem("Password",Password[i].c_str());
		xml->AddElem("UserRole",UserRole[i].c_str());
		xml->AddElem("Email",Email[i].c_str());
		xml->AddElem("ContactNo",Mobile[i].c_str());
		xml->AddElem("Status",Status[i].c_str());
		xml->OutOfElem();
	}

	for ( int i = 0 ; i < UID.size() ; i++ ){
			dBA->DeleteTableEntry("userugrouprelation","UID",atoi(UID[i].c_str()));
		}

	dBA->DeleteTableEntryForDID("userinfo", DIDInfo);
	string xml_Data = xml->GetDoc();
		sprintf ( xmlData,"%s",xml_Data.c_str());
}

void Failover::CreateXMLForCgroup(DataBaseAccess *dBA,const char *DIDInfo,char * xmlData){
	vector<string> CGID,CGName,DID;
	dBA->GetAllCGroupForDID(DIDInfo,&CGID,&CGName);
	xml = new CMarkup();
		xml->OutOfElem();
					if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
						xml->AddElem( "XMLDakshaImaging" );
	for ( int i = 0 ; i < (int)CGID.size() ; i++ ){
		CheckForXMLHeader("AddCGroup");
		xml->AddElem("CGroupInfo");
		xml->IntoElem();
		xml->AddElem("CGNAME",CGName[i].c_str());
		xml->OutOfElem();
	}
	dBA->DeleteTableEntryForDID("cgroupinfo", DIDInfo);
	string xml_Data = xml->GetDoc();
		sprintf ( xmlData,"%s",xml_Data.c_str());
}

void Failover::CreateXMLForUGroup(DataBaseAccess *dBA,const char *DID,char * xmlData){
	vector<string> UGID,UGName;
	dBA->GetAllUGroupForDID(DID,&UGID,&UGName);
	xml = new CMarkup();
		xml->OutOfElem();
					if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
						xml->AddElem( "XMLDakshaImaging" );
	for ( int i = 0 ; i < (int)UGID.size() ; i++ ){
			CheckForXMLHeader("AddUGroup");
			xml->AddElem("UGroupInfo");
			xml->IntoElem();
			xml->AddElem("UGNAME",UGName[i].c_str());
			xml->OutOfElem();
		}

	for ( int i = 0 ; i < UGID.size() ; i++ ){
			dBA->DeleteTableEntry("userugrouprelation","UGID",atoi(UGID[i].c_str()));
		}
	dBA->DeleteTableEntryForDID("ugroupinfo", DID);
	string xml_Data = xml->GetDoc();
		sprintf ( xmlData,"%s",xml_Data.c_str());
}

void Failover::CreateXMLForChannelCgroup(DataBaseAccess *dBA,const char *DID,char * xmlData){

	vector<string> CID,CGID,CGName,UID;
	dBA->GetAllChannelCGroupRelationForDID(DID,&CID,&CGID,&CGName,&UID);
	xml = new CMarkup();
		xml->OutOfElem();
					if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
						xml->AddElem( "XMLDakshaImaging" );
	for ( int i  = 0 ; i < (int)UID.size() ; i++ ){
		CheckForXMLHeader("AddchannelCGroupRelation");
		xml->IntoElem();
		xml->AddElem("CID",CID[i].c_str());
		xml->AddElem("CGID",CGID[i].c_str());
		xml->AddElem("UID",UID[i].c_str());
		xml->OutOfElem();
	}
	dBA->DeleteTableEntryForDID("channelcgrouprelation", DID);
	string xml_Data = xml->GetDoc();
		sprintf ( xmlData,"%s",xml_Data.c_str());
}

void Failover::CreateXMLForUserUGroup(DataBaseAccess *dBA,const char *DID,char * xmlData){
	vector<string> UID,UGID,UGName,UUGID;
	dBA->GetAllUserUGroupRelationForDID(DID,&UID,&UGID,&UGName);
	xml = new CMarkup();
		xml->OutOfElem();
					if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
						xml->AddElem( "XMLDakshaImaging" );
	for ( int i = 0 ; i < (int)UID.size() ; i++ ){
		CheckForXMLHeader("AddUserUGroupRelation");
		xml->IntoElem();
		xml->AddElem("UID",UID[i].c_str());
		xml->AddElem("UGID",UGID[i].c_str());
		xml->OutOfElem();
	}
	dBA->DeleteTableEntryForDID("userugrouprelation", DID);
	cout<<"\nCompleted\n"<<endl;
	string xml_Data = xml->GetDoc();
    sprintf ( xmlData,"%s",xml_Data.c_str());
	cout<<"\nCompleted\n"<<endl;
}

void Failover::CreateXMLForUGroupCGroup(DataBaseAccess *dBA,const char *DID,char * xmlData){
	vector<string> UGCGID,UGID,CGID,CGNAME,UGNAME,UID,DIDInfo;
	dBA->GetUGroupCGroupRelationForDID(DID,UGID,CGID,CGNAME,UGNAME);
	cout<<"\nInSIDE--->"<<endl;
	int sysDID = dBA->GetSystemDID();
	dBA->GetActiveUID(UID,sysDID);

	xml = new CMarkup();
	xml->OutOfElem();
				if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
					xml->AddElem( "XMLDakshaImaging" );
	if ( UID.size() > 0 ){
		cout<<"\nUGID Size--->"<<UGID.size()<<endl;
	for ( int i = 0 ; i < (int)UGID.size() ; i++ ){
		CheckForXMLHeader("UGroupCGroupRelation");
		xml->IntoElem();
		xml->AddElem("UID",UID[0].c_str());
		xml->AddElem("UGroupRelation");
		xml->IntoElem();
		xml->AddElem("UGroupInfo");
		xml->IntoElem();
		xml->AddElem("UGID",UGID[i].c_str());
		xml->AddElem("AddCGroup");
		xml->IntoElem();
		xml->AddElem("CGID",CGID[i].c_str());
		xml->OutOfElem();
		xml->OutOfElem();
		xml->OutOfElem();
		xml->OutOfElem();

	}
		dBA->DeleteTableEntryForDID("ugroupcgrouprelation", DID);
		string xml_Data = xml->GetDoc();
		sprintf ( xmlData,"%s",xml_Data.c_str());
	}
}


void Failover::SendReceiveXML(char * xmlData, int sockfd ){

		char receiData[100000];
	        	int size = 200000;
	        	if ( CommonClassObj->sendData(sizeof(int),&size,sockfd) ){
	        		if ( CommonClassObj->sendData(size,xmlData,sockfd) ){
	        			cout<<"\nSend XML--->"<<endl;
	        			if ( CommonClassObj->recvData(&size,sizeof(int), sockfd) ){
	        				if ( CommonClassObj->recvData(&receiData,size, sockfd) ){
	        					cout<<"\nReceived XML\n"<<endl;
	        				}
	        			}
	        		}
	        	}
}

void Failover::CreateXML(DataBaseAccess *dBA,const char *DID , int sysDID){

	if ( xml != NULL )
		delete xml;

	//xml = new CMarkup();
	char channelData[200000], userData[200000], cgroupData[200000],ugroupData[200000], channelCgroupData[200000], userUgroupData[200000], ugroupcgroupData[200000];
	int socketID = -1;
	char deviceIP[100];
	dBA->GetDeviceIP(deviceIP,sysDID);
	cout<<"\ndeviceIP---->"<<deviceIP<<endl;
	CommonClassObj->CreateConnectionClientReturnFD(deviceIP, tcpSocketPortNumber, socketID);
	cout<<"\nSocketID ----->"<<socketID<<endl;
	if ( socketID != - 1){

		CreateXMLForUGroupCGroup(dBA,DID,ugroupcgroupData);
	    CreateXMLForUserUGroup(dBA,DID,userUgroupData);
	    CreateXMLForChannelCgroup(dBA,DID,channelCgroupData);
		CreateXMLForUser(dBA,DID,userData);
		CreateXMLForCgroup(dBA,DID,cgroupData);
		CreateXMLForUGroup(dBA,DID,ugroupData);
		CreateXmlForChannel(dBA,DID,channelData, sysDID);

		cout<<"\nchannelData--->"<<channelData<<endl;
	    SendReceiveXML(channelData, socketID);
	   SendReceiveXML(userData, socketID);
	   SendReceiveXML(cgroupData, socketID);
	   SendReceiveXML(ugroupData, socketID);
	   SendReceiveXML(channelCgroupData, socketID);
	   SendReceiveXML(userUgroupData, socketID);
	  SendReceiveXML(ugroupcgroupData, socketID);


	}
	else{
		cout<<"\nNot able to create connection\n"<<endl;
	}
}

void Failover::ReplaceDownServerDIDWithBackUPServerDID(const char *DID, int sysDID,DataBaseAccess * dBA, vector<string> DeviceIP ){

	GetInformationAboutFromAllOtherServer(DeviceIP, DID);
	CreateXML(dBA,DID, sysDID );
	bootLoader->StartBoot();
}

void Failover::MakeReplica( const char *DeviceIP , const char * DID,DataBaseAccess * dBA, vector<string> DevicesIP ){

	cout<<"\nMake Replica\n";
	int sysDID = dBA->GetSystemDID();
	ReplaceDownServerDIDWithBackUPServerDID(DID, sysDID,dBA, DevicesIP);
}


void *Failover::StartThreadMain ( void *para ){
	struct failOverThreadData * tempData = (struct failOverThreadData*)para;
	vector < string > DID,DName,DeviceIP,Status,Owner,MacAddress,backup;
	Failover * failover = tempData->failover;
	CommonClass * commFunc = new CommonClass();

	while(1){

		tempData->failover->dataBaseObject = new DataBaseAccess();

		if ( tempData->failover->isBreak ){
			cout<<"\nBreaked"<<endl;
			int sysDID = tempData->failover->dataBaseObject->GetSystemDID();
			tempData->failover->dataBaseObject->UpdateDeviceBackupStatus(sysDID,0);
			tempData->failover->dataBaseObject->Close();
			delete tempData->failover->dataBaseObject;
			break;
		}
		if ( DID.size() > 0 ){
			DID.clear();
			DName.clear();
			DeviceIP.clear();
			Status.clear();
			Owner.clear();
			MacAddress.clear();
		}
		tempData->failover->dataBaseObject->GetAllDeviceInfo(&DID,&DName,&DeviceIP,&Status,&Owner,&MacAddress,&backup, false);
		cout<<"\nFound Server count---->"<<DID.size()<<"\n"<<endl;
		if ( DID.size() > 0 ){
		for ( int i = 0 ; i < (int)DID.size() ; i++ ){
				if ( tempData->failover->CreateConnectionFailOver( DeviceIP[i].c_str()) )
					cout<<"\nDeviceip--->"<<DeviceIP[i].c_str()<<"----"<<"exists"<<endl;
				else
				{
					cout<<"\nDeviceip--->"<<DeviceIP[i].c_str()<<"----"<<"Does not exists"<<endl;
					tempData->failover->MakeReplica( DeviceIP[i].c_str() , DID[i].c_str(), tempData->failover->dataBaseObject,DeviceIP );
					tempData->failover->isBreak = true;
					break;
				}
		}
		}
		tempData->failover->dataBaseObject->Close();
		delete tempData->failover->dataBaseObject;
		sleep(FAILURECheckTimeInterval);
	}
	delete commFunc;
}

bool Failover::WaitForThreadCompletion(){
	pthread_join(failoverThread,NULL);
	return true;
}

bool Failover::CheckServerStatus(){

}



