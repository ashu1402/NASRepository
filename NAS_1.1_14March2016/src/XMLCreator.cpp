#include "XMLCreator.h"

XMLCreator::XMLCreator()
{
	try{
		DataBaseAccessObj = new DataBaseAccess();
		xml_node<>* decl = doc.allocate_node(node_declaration);
		decl->append_attribute(doc.allocate_attribute("version", "1.0"));
		decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
				//decl->append_attribute(doc.allocate_attribute("encoding", "iso-8859-1"));

		doc.append_node(decl);
		xml = new CMarkup();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator Function:"<<e.what()<<endl;
	}
}


void XMLCreator::ReInitializeXML(){
	if ( xml != NULL ){
		delete xml;
		string xml_as_string;
		rapidxml::print(std::back_inserter(xml_as_string), doc);
		xml = new CMarkup(xml_as_string.c_str());
	}
}

XMLCreator::~XMLCreator()
{
	if ( DataBaseAccessObj != NULL ){
		DataBaseAccessObj->Close();
		delete DataBaseAccessObj;
	}
}

bool XMLCreator::CreateXMLCreator(char *data){
	//cout<<"\nData--->"<<data<<endl;
	if ( xml != NULL )
		delete xml;
	xml = new CMarkup(data);

}

string XMLCreator::NewconvertToString()
{
	return xml->GetDoc();
}



void XMLCreator::NewChannelInfo( int CID, const char* CName,const char* RTSP,int  DID,int Status,int StorageStatus, const char* RetrieverMode,const char* ReplicaMode,const char* StoragePath,const char* StorageSpace,const char* StorageSpaceUsed, const char* StorageFileType,const char* streamMode, const char * deviceIP , const char * cameraIp , const char * userName , const char * portNumber , const char * password, const char * ptzControl, const char * motion, const char * description, const char * make, const char * model, bool isAndroidStatus){

	try{
		xml->AddElem( "CID", CID );
		xml->AddElem( "ChannelName", CName );
		xml->AddElem( "RTSP", RTSP );
		xml->AddElem( "Status", Status );

		if (! isAndroidStatus ){

					xml->AddElem( "DID", DID );
					xml->AddElem( "IPAddress", cameraIp );
					xml->AddElem( "PortNo", portNumber );
					xml->AddElem( "UserName", userName );
					xml->AddElem( "Password", password );
					xml->AddElem( "StorageStatus", StorageStatus );
					xml->AddElem( "RetrieverMode", RetrieverMode );
					xml->AddElem( "ReplicaMode", ReplicaMode );
					xml->AddElem( "StoragePath", StoragePath );
					xml->AddElem( "StorageSpace", StorageSpace );
					xml->AddElem( "StorageSpaceUsed", StorageSpaceUsed );
					xml->AddElem( "StorageFileType", StorageFileType );
					xml->AddElem( "StreamMode", streamMode );
					xml->AddElem( "DeviceIP", deviceIP );
					xml->AddElem( "ResendStatus", streamMode );
					xml->AddElem( "RestreamStatus",streamMode );
					xml->AddElem( "RetrieveStatus", RetrieverMode );
					xml->AddElem( "PTZ", ptzControl );
					xml->AddElem( "Motion", motion );
					xml->AddElem( "Decs", description );
					xml->AddElem( "Make", make );
					xml->AddElem( "Model", model );
		}

	}

	catch (exception &e){
		cout<<"Exception caught in XMLCreator:ChannelINfo Function:"<<e.what()<<endl;
	}

}


void XMLCreator::NewVideoInfo( const char* VName,int VID, const char* VideoStartTime,const char* VideoEndTime)
{
	try{

		xml->AddElem( "VN",VName );
		xml->AddElem( "VID",VID );
		xml->AddElem(  "ST",VideoStartTime );
		xml->AddElem( "ET",VideoEndTime );
	}
	catch (exception &e){
		cout<<"Exception caught in XMLCreator:ChannelINfo Function:"<<e.what()<<endl;
	}
}

void  XMLCreator::NewUGroupInfo( const char* DID,const char* UGID, const char* UGName,bool isAllGroup)
{
	try{
		xml->AddElem( "UGID", UGID );
		xml->AddElem( "UGName", UGName );
		if ( !isAllGroup )
			xml->AddElem( "DID", DID );

	}
	catch (exception &e){
	}

}



void XMLCreator::NewUserInfo(const char *DID,int UID,const char*  UName,const char* UserName,const char* Password,const char* UserRole,const char* Email,const char* Mobile,const char* Status, bool isAllUser )
{
	try{

		xml->AddElem("UID",UID);
		xml->AddElem( "UName",UName);
		xml->AddElem( "UserName",UserName);
		xml->AddElem( "Password",Password);
		xml->AddElem( "UserRole",UserRole);
		xml->AddElem( "Email",Email);
		xml->AddElem( "ContactNo",Mobile);
		xml->AddElem( "Status",Status);
		if ( !isAllUser )
			xml->AddElem( "DID",DID);

	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:UGroupINfo Function:"<<e.what()<<endl;
	}
}



void XMLCreator::NewUserUGroupInfo(const char* DID,const char* UUGID,const char* UID,const char*  UGID,const char* UGName,bool isAllInfo )
{
	try{
		if ( !isAllInfo ){
			xml->AddElem("UUGID",UUGID);
			xml->AddElem( "UGName",UGName);
			xml->AddElem( "DID",DID);

		}
		xml->AddElem("UID",UID);
		xml->AddElem( "UGID",UGID);
	}
	catch (exception &e){
		cout<<"Exception caught in XMLCreator:UGroupINfo Function:"<<e.what()<<endl;
	}
}


void XMLCreator::NewChannelCGroupInfo(const char * DID, const char * CCGID, const char* CID,const char*  CGID,const char* CGName,int UID,bool isAllInfo)
{
	try{

		xml->AddElem("CID",CID);
		xml->AddElem( "CGID",CGID);
		xml->AddElem( "CGName",CGName);
		if ( !isAllInfo ){
			xml->AddElem("CCGID",CCGID);
			xml->AddElem( "UID",UID);
			xml->AddElem( "DID",DID);
		}

	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:UGroupINfo Function:"<<e.what()<<endl;
	}
}


void XMLCreator::NewDeviceInfo(	const char* DID,const char* DName,const char* DeviceIP,const char* Status,const char* Owner,const char*  MacAddress)
{
	try{
		xml->AddElem("DID",DID);
		xml->AddElem( "DName",DName);
		xml->AddElem( "DeviceIP",DeviceIP);
		xml->AddElem("Status",Status);
		xml->AddElem("Owner",Owner);
		xml->AddElem("MacAddress",MacAddress);
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:UGroupINfo Function:"<<e.what()<<endl;
	}
}

void XMLCreator::NewRtspInfo(){
	try
		{
			vector<string> CID,DID,rtsp;
			DataBaseAccessObj->GetAllRtspInfo(CID,DID,rtsp);
			xml->OutOfElem();
			if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
				xml->AddElem( "XMLDakshaImaging" );
			xml->IntoElem();

			xml->AddElem( "Interaction" );
			xml->AddAttrib("ActionName", "RtspInfo");
			xml->IntoElem();
			for ( int i = 0 ; i < (int)CID.size() ; i++ ){
				xml->AddElem( "RtspInfo" );
				xml->IntoElem();
				xml->AddElem( "DID",DID[i].c_str() );
				xml->AddElem( "CID",CID[i].c_str() );
				xml->AddElem( "Rtsp",rtsp[i].c_str());
				xml->OutOfElem();
			}
			xml->OutOfElem();
		}catch (exception &e){
			cout<<"Exception caught in XMLCreator:NewRtspInfo Function:"<<e.what()<<endl;
		}

}
void XMLCreator::NewFetchDeviceInfo(){
	try
	{
		vector<int> DID,status;
		vector<string> macAddress,DeviceIP,deviceName;
		DataBaseAccessObj->GetDeviceDetails(DID,macAddress,DeviceIP,status,deviceName);
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();

		xml->AddElem( "Interaction" );
		xml->AddAttrib("ActionName", "DeviceInfo");
		xml->IntoElem();
		for ( int i = 0 ; i < (int)DID.size() ; i++ ){
			xml->AddElem( "DeviceInfo" );
			xml->IntoElem();
			xml->AddElem( "DID",DID[i] );
			xml->AddElem( "DEVICENAME",deviceName[i].c_str() );
			xml->AddElem( "DEVICEIP",DeviceIP[i].c_str() );
			xml->AddElem( "MACADDRESS",macAddress[i].c_str() );
			xml->AddElem( "STATUS",status[i]);
			xml->OutOfElem();
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:CgroupINfo Function:"<<e.what()<<endl;
	}

}
void XMLCreator::NewFetchAllUser( bool isAllUser )
{
	try
	{
		vector<string> DID,UID,UName,UserName, Password,UserRole,Email, Mobile,Status;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		if ( isAllUser )
			xml->AddElem( "Response" );
		else
			xml->AddElem( "Interaction" );
		xml->AddAttrib("ActionName", "FetchAllUser");
		xml->IntoElem();
		if ( isAllUser )
			DataBaseAccessObj->GetAllUser(&UID,&UName,&UserName,&Password,&UserRole,&Email,&Mobile,&Status);
		else{
			int sySDID = DataBaseAccessObj->GetSystemDID();
			DataBaseAccessObj->GetAllUserOnlyOwners(DID,&UID,&UName,&UserName,&Password,&UserRole,&Email,&Mobile,&Status);
		}
		for( int i=0;i < (int)UID.size();i++){
			xml->AddElem("UserInfo");
			xml->IntoElem();
			if ( isAllUser )
				NewUserInfo(NULL,atoi(UID[i].c_str()),UName[i].c_str(),UserName[i].c_str(),Password[i].c_str(),UserRole[i].c_str(),Email[i].c_str(),Mobile[i].c_str(),Status[i].c_str(),isAllUser);
			else
				NewUserInfo(DID[i].c_str(),atoi(UID[i].c_str()),UName[i].c_str(),UserName[i].c_str(),Password[i].c_str(),UserRole[i].c_str(),Email[i].c_str(),Mobile[i].c_str(),Status[i].c_str(),isAllUser);
			xml->OutOfElem();
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:CgroupINfo Function:"<<e.what()<<endl;
	}

}


bool XMLCreator::FindElement( string mainString, string subString ){


	int pos = -1;
	pos = mainString.find(subString);
	if ( pos != -1 )
		return true;
	return false;
}



void XMLCreator::NewFetchAllUserUGroupRelation(bool isAllUser)
{
	try{


		vector<string> UID,UGID,UGName,UUGID,DID;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") ){
			xml->AddElem( "XMLDakshaImaging" );
		}

		xml->IntoElem();
		if ( isAllUser ){
			xml->AddElem( "Response" );

		}else
			xml->AddElem( "Interaction" );
		xml->AddAttrib("ActionName", "FetchAllUserUGroupRelation");
		xml->IntoElem();

		if ( isAllUser ){

			DataBaseAccessObj->GetAllUserUGroupRelation(&UID,&UGID,&UGName);

		}else {
			//int sysDID = DataBaseAccessObj->GetSystemDID();
			DataBaseAccessObj->GetAllUserUGroupRelationOnlyOwners(DID,&UUGID,&UID,&UGID,&UGName);
		}
		for( int i=0;i < (int)UID.size();i++ ){



			xml->AddElem("UserUGroupInfo");
			xml->IntoElem();
			if ( isAllUser ){
				xml->AddElem( "UGName",UGName[i].c_str());
				xml->AddElem("UID",UID[i].c_str());
				xml->AddElem( "UGID",UGID[i].c_str());
			}
			else
				NewUserUGroupInfo(DID[i].c_str(),UUGID[i].c_str(),UID[i].c_str(),UGID[i].c_str(),UGName[i].c_str(),isAllUser);
			xml->OutOfElem();
		}
		xml->OutOfElem();

	}
	catch (exception &e){
		cout<<"Exception caught in XMLCreator:CgroupINfo Function:"<<e.what()<<endl;
	}

}



void XMLCreator::NewFetchAllUGroup(bool isAllGroup )
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		if ( isAllGroup )
			xml->AddElem( "Response" );
		else
			xml->AddElem( "Interaction" );
		xml->AddAttrib("ActionName", "FetchAllUGroup");
		xml->IntoElem();
		vector<string> UGID;
		vector<string> UGName,DID;
		if ( isAllGroup )
			DataBaseAccessObj->GetAllUGroup(&UGID,&UGName);
		else{
			//int sysDID = DataBaseAccessObj->GetSystemDID();//(&UGID,&UGName);
			DataBaseAccessObj->GetAllUGroupOnlyUser(DID, &UGID,&UGName);
		}
		for( int i = 0 ; i < (int)UGID.size(); i++ ){
			xml->AddElem( "UGroupInfo" );
			xml->IntoElem();
			if ( isAllGroup )
				NewUGroupInfo(NULL,UGID[i].c_str(),UGName[i].c_str(),isAllGroup);
			else
				NewUGroupInfo(DID[i].c_str(),UGID[i].c_str(),UGName[i].c_str(),isAllGroup);
			xml->OutOfElem();
		}
		xml->OutOfElem();
	}
	catch (exception &e){
		cout<<"Exception caught in XMLCreator:CgroupINfo Function:"<<e.what()<<endl;
	}
}



void XMLCreator::NewFetchAllChannel(bool isAllChannel  )
{
	try
	{
		//cout<<"\nCIDSIze=0"<<endl;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();

		if ( isAllChannel )
			xml->AddElem( "Response" );
		else
			xml->AddElem( "Interaction" );

		xml->AddAttrib("ActionName", "FetchAllChannel");
		xml->IntoElem();

		vector<int> CID,DID,status,storagestatus;
		vector<string> deviceIP,ChannelName,RTSP,RetrieverMode,ChannelMode,StoragePath,StorageSpace,StorageSpaceUsed,StorageFileType,ReStream;
		vector<string>ptzControl,motion,description,ipAddress,portNumber,userName,password, make , model;
		if (isAllChannel)
			DataBaseAccessObj->GetAllChannel(&CID,&ChannelName,&RTSP,&DID,&status,&storagestatus,&RetrieverMode,&ChannelMode,&StoragePath,&StorageSpace,&StorageSpaceUsed,&StorageFileType,&ReStream,&deviceIP , &ipAddress, &userName ,  &portNumber ,  &password,  &ptzControl,  &motion, &description, &make ,&model );
		else{
			int sysDID = DataBaseAccessObj->GetSystemDID();
			DataBaseAccessObj->GetAllChannelOnlyOwner(sysDID,&CID,&ChannelName,&RTSP,&DID,&status,&storagestatus,&RetrieverMode,&ChannelMode,&StoragePath,&StorageSpace,&StorageSpaceUsed,&StorageFileType,&ReStream,&deviceIP, &ipAddress, &userName ,  &portNumber ,  &password,  &ptzControl,  &motion, &description, &make ,&model);
		}
		for(int i=0;i < (int)CID.size(); i++ )
		{	xml->AddElem( "ChannelInfo" );
		xml->IntoElem();
		NewChannelInfo( CID[i] , ChannelName[i].c_str(), RTSP[i].c_str() , DID[i] , status[i] , storagestatus[i] , RetrieverMode[i].c_str(), ChannelMode[i].c_str() ,  StoragePath[i].c_str() , StorageSpace[i].c_str(), StorageSpaceUsed[i].c_str(), StorageFileType[i].c_str() , ReStream[i].c_str(), deviceIP[i].c_str() ,ipAddress[i].c_str(),  userName[i].c_str() ,  portNumber[i].c_str() ,  password[i].c_str(),  ptzControl[i].c_str(),  motion[i].c_str(), description[i].c_str() , make[i].c_str() ,model[i].c_str(), false);
		xml->OutOfElem();
		}
		xml->OutOfElem();

	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchAllChannel Function:"<<e.what()<<endl;
	}

}


void XMLCreator::NewFetchAllChannelCGroupRelation(bool isAllChannel )
{
	try
	{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") ){
			xml->AddElem("XMLDakshaImaging");
		}
		xml->IntoElem();
		if ( isAllChannel ){
			xml->AddElem( "Response" );

		}
		else
			xml->AddElem( "Interaction" );
		xml->AddAttrib("ActionName", "FetchAllChannelCGroupRelation");
		xml->IntoElem();

		vector<string> CCGID, CID,CGID,CGName,DID;
		vector<int> UID;
		if (isAllChannel){
			DataBaseAccessObj->GetAllChannelCGroupRelation(&CID,&CGID,&CGName);

		}
		else{
			int sysDID = DataBaseAccessObj->GetSystemDID();
			DataBaseAccessObj->GetAllChannelCGroupRelationOnlyOwners(DID,&CCGID,&CID,&CGID,&CGName,&UID);
		}
		for(int i= 0;i < (int)CID.size(); i++ ){

			xml->AddElem("ChannelCGroupInfo");
			xml->IntoElem();
			if (isAllChannel){
				xml->AddElem("CID",CID[i].c_str());
				xml->AddElem( "CGID",CGID[i].c_str());
				xml->AddElem( "CGName",CGName[i].c_str());
			}
			else
				NewChannelCGroupInfo( DID[i].c_str(),CCGID[i].c_str(),CID[i].c_str(),CGID[i].c_str(),CGName[i].c_str(),UID[i],isAllChannel);
			xml->OutOfElem();
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchAllChannelCGroupRelation Function:"<<e.what()<<endl;
	}
}



void XMLCreator::NewFetchAllCGroup(bool isAllGroup )
{
	try
	{
		vector<string> CGID,CGName,DID;

		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();

		if ( isAllGroup )
			xml->AddElem( "Response" );
		else
			xml->AddElem( "Interaction" );
		xml->AddAttrib("ActionName", "FetchAllCGroup");
		xml->IntoElem();

		if( isAllGroup )
			DataBaseAccessObj->GetAllCGroup(&CGID,&CGName);
		else{
			int sysDID = DataBaseAccessObj->GetSystemDID();
			DataBaseAccessObj->GetAllCGroupOnlyOwner(DID,&CGID,&CGName);
		}
		for(int i=0;i < (int)CGID.size();i++){
			xml->AddElem("CGroupInfo");
			xml->IntoElem();
			if  ( !isAllGroup )
				NewCGroupInfo(DID[i].c_str(),atoi(CGID[i].c_str()),CGName[i].c_str(), isAllGroup, false);
			else
				NewCGroupInfo(NULL,atoi(CGID[i].c_str()),CGName[i].c_str(), isAllGroup, false);

			xml->OutOfElem();

		}
		xml->OutOfElem();

	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchAllCGroup Function:"<<e.what()<<endl;
	}

}



void XMLCreator::NewFetchAllDevice()
{
	try
	{
		vector<string> DID,DName,DeviceIP,Status,Owner,MacAddress;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "FetchAllDevice");
		xml->IntoElem();

		DataBaseAccessObj->GetAllDeviceInfo(&DID,&DName,&DeviceIP,&Status,&Owner,&MacAddress, true);

		for(int index=0;index < (int)DID.size(); index++ ){
			xml->AddElem("DeviceInfo");
			xml->IntoElem();
			NewDeviceInfo( DID[index].c_str(), DName[index].c_str(), DeviceIP[index].c_str(), Status[index].c_str() , Owner[index].c_str(),MacAddress[index].c_str() );
			xml->OutOfElem();
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:CgroupINfo Function:"<<e.what()<<endl;
	}
}




void XMLCreator::NewCGroupInfo( const char * DIDInfo, int CGID, const char* CGName,bool isAllInfo, bool isAndroidStatus)
{
	try{
		char createdRTSP[SIZEOFCHARARRAYBIGGER];
		vector<string> make, model,CID,DID,ChannelName,RTSP,status,storagestatus,RetrieverMode,ChannelMode,StoragePath,StorageSpace,StorageSpaceUsed,StorageFileType,ReStream,cameraIp , userName ,  portNumber ,password, ptzControl,  motion, description;
		xml->AddElem("CGID",CGID);
		xml->AddElem( "CGName",CGName);
		if ( !isAllInfo )
			xml->AddElem("DID",DIDInfo);
		if ( isAllInfo )
			DataBaseAccessObj->GetChannelInfoFromCGID(CGID,&CID,&ChannelName,&RTSP,&DID,&status,&storagestatus,&RetrieverMode,&ChannelMode,&StoragePath,&StorageSpace,&StorageSpaceUsed,&StorageFileType,&ReStream, &cameraIp, &userName ,  &portNumber ,  &password,  &ptzControl,  &motion, &description, &make, &model);
		else
			DataBaseAccessObj->GetChannelInfoFromCGIDInteraction(isAllInfo,CGID,&CID);
		char deviceIp[100], createRtsp[100];
		for(int index = 0 ; index < (int)CID.size(); index++ ){

			xml->AddElem( "ChannelInfo" );
			xml->IntoElem();
			if ( isAllInfo ){
				string SysDID = DataBaseAccessObj->GetSystemIPFromDID(atoi((char*)DID[index].c_str()));
				DataBaseAccessObj->GetCreatedRtsp(CID[index].c_str(),createdRTSP);
				NewChannelInfo( atoi(CID[index].c_str()),ChannelName[index].c_str(),createdRTSP,atoi(DID[index].c_str()),atoi(status[index].c_str()),atoi(storagestatus[index].c_str()), RetrieverMode[index].c_str() , ChannelMode[index].c_str(), StoragePath[index].c_str(), StorageSpace[index].c_str(), StorageSpaceUsed[index].c_str(), StorageFileType[index].c_str(), ReStream[index].c_str(),SysDID.c_str(),cameraIp[index].c_str() , userName[index].c_str() ,  portNumber[index].c_str() ,  password[index].c_str(), ptzControl[index].c_str(),  motion[index].c_str(), description[index].c_str(), make[index].c_str(),model[index].c_str() ,isAndroidStatus);
			}
			else{
				xml->AddElem( "CID",CID[index].c_str() );
			}
			xml->OutOfElem();
		}
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:NewCGroupInfo Function:"<<e.what()<<endl;
	}

}


bool XMLCreator::KeepActionUserInfo(int UID,int DID,const char *clientIP,int status ){

	AUID = -1;
	AUID = DataBaseAccessObj->CheckActiveUser(UID,DID);
	if ( AUID == -1  ){
		AUID = DataBaseAccessObj->InsertActiveUserInfo(UID,DID,clientIP,status);

		return true;
	}
	else{

		DataBaseAccessObj->UpdateActiveUserInfo(AUID,clientIP,status);

		return true;
	}
	return false;
}

void XMLCreator::NewLoginSuccess( int UID, const char * clientIP, bool isAndroidStatus )
{
	try{
		//cout<<"inside login function\n"<<endl;
		if(UID == -1){
			cout<<"Invalid UID received\n\n\n"<<endl;
			return;
		}

		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "Login");
		xml->IntoElem();


		vector<string> UName,UserName,Password,UserRole,Email,Mobile,Status;
		vector<int> CGID;
		vector<string> CGName;

		DataBaseAccessObj->GetUserInfoFromUID(UID,&UName,&UserName,&Password,&UserRole,&Email,&Mobile,&Status);
		DataBaseAccessObj->GetCGroupInfoFromUID(UID,&CGID,&CGName);

		if ( UName.size() > 0 ){
			if ( KeepActionUserInfo(UID,DataBaseAccessObj->GetSystemDID(),(const char *)clientIP,1) ){


				for(int index = 0; index < (int)UName.size(); index++ ){
					xml->AddElem("UserInfo");
					xml->IntoElem();
					NewUserInfo(NULL,UID,UName[index].c_str(),UserName[index].c_str(), Password[index].c_str(), UserRole[index].c_str(), Email[index].c_str(), Mobile[index].c_str() , Status[index].c_str(), true );
					xml->OutOfElem();
				}

				for(int index=0;index < (int)CGID.size();index++){
					xml->AddElem("CGroupInfo");
					xml->IntoElem();
					NewCGroupInfo(NULL, CGID[index],CGName[index].c_str(), true,isAndroidStatus );
					xml->OutOfElem();
				}

			}
			else
				NewErrorMsgXML("User Already Login","40004");
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:LoginSucess Function:"<<e.what()<<endl;
	}
}

void XMLCreator::NewSuccessActionWithString( char* ActionName, char* Msg, char* tagName , char * tagValue )
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", ActionName);
		xml->IntoElem();
		xml->AddElem( "Success",Msg );
		xml->AddElem(tagName,tagValue);
		xml->OutOfElem();

	}
	catch (exception &e){
		cout<<"Exception caught in XMLCreator:LoginSucess Function:"<<e.what()<<endl;
	}

}

void XMLCreator::NewSuccessActionWithID( char* ActionName, char* Msg, char* tagName , int tagValue )
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", ActionName);
		xml->IntoElem();
		xml->AddElem( "Success",Msg );
		xml->AddElem(tagName,tagValue);
		xml->OutOfElem();

	}
	catch (exception &e){
		cout<<"Exception caught in XMLCreator:LoginSucess Function:"<<e.what()<<endl;
	}

}

void XMLCreator::NewSuccessAction( char* ActionName, char* Msg)
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", ActionName);
		xml->IntoElem();
		xml->AddElem( "Success",Msg );
		xml->OutOfElem();
	}
	catch (exception &e){
		cout<<"Exception caught in XMLCreator:LoginSucess Function:"<<e.what()<<endl;
	}

}



void XMLCreator::NewSuccessAddUserAction(char* UID)
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "AddUser");
		xml->IntoElem();
		xml->AddElem("Success");
		xml->IntoElem();
		xml->AddElem("UID",UID);
		xml->OutOfElem();
		//xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:StartVOD Function:"<<e.what()<<endl;
	}

}

void XMLCreator::NewSuccessAddChannelAction(char* CID, char * deviceIp )
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "AddChannel");
		xml->IntoElem();
		xml->AddElem("Success");
		xml->IntoElem();
		xml->AddElem("CID",CID);
		xml->AddElem("DeviceIp",deviceIp);
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:SuccessAddChannelAction Function:"<<e.what()<<endl;
	}

}


void XMLCreator::NewSuccessAddDeviceAction( char* DID, char * deviceName ){
	try{
		ReInitializeXML();
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "AddDevice");
		xml->IntoElem();
		xml->AddElem("Success");
		xml->IntoElem();
		xml->AddElem("DID",DID);
		xml->AddElem("DEVICENAME",deviceName);
		xml->OutOfElem();
	}
	catch (exception &e){
		cout<<"Exception caught in XMLCreator:NewSuccessAddDeviceAction Function:"<<e.what()<<endl;
	}
}


void XMLCreator::NewSuccessVODAction(vector<string> VName,vector<int> VID,vector<string> VideoStartTime,vector<string> VideoEndTime)
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "StartVOD");
		xml->IntoElem();
		xml->AddElem("Success");
		xml->IntoElem();
		std::string str;
		int pos = 0;
		for( int index=0;index < (int)VID.size();index++ ){
			str = VName[index].substr();
			pos = str.find_last_of('/');
			str = str.substr(pos + 1,str.length());
			xml->AddElem( "VI" );
			xml->IntoElem();
			NewVideoInfo( str.c_str(), VID[index], VideoStartTime[index].c_str(),VideoEndTime[index].c_str() );
			xml->OutOfElem();
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:StartVOD Function:"<<e.what()<<endl;
	}

}



void XMLCreator::NewSuccessFetchAllVideoAction(vector<string> VName,vector<int> VID,vector<string> VideoStartTime,vector<string> VideoEndTime)
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "FetchAllVideo");
		xml->IntoElem();
		xml->AddElem("Success");
		xml->IntoElem();

		for(int index=0;index < (int)VID.size(); index++ ){
			xml->AddElem( "VI" );
			xml->IntoElem();
			NewVideoInfo(VName[index].c_str(), VID[index], VideoStartTime[index].c_str(), VideoEndTime[index].c_str() );
			xml->OutOfElem();
		}
		xml->OutOfElem();

	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:StartVOD Function:"<<e.what()<<endl;
	}

}

void XMLCreator::NewSuccessInteraction()
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "InteractionResult");
		xml->IntoElem();
		xml->AddElem("Success","ok");
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:NewSuccessInteraction Function:"<<e.what()<<endl;
	}

}

void XMLCreator::NewSuccessStartDownloadAction(const char* PortNumber)
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "StartDownload");
		xml->IntoElem();
		xml->AddElem("Success");
		xml->IntoElem();
		xml->AddElem("PortNumber",PortNumber);
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:StartVOD Function:"<<e.what()<<endl;
	}

}

void XMLCreator::NewErrorMsgXML(const char* ErrorMsg, const char* ErrorId )
{
	try{
		xml->AddElem("Error");
		xml->IntoElem();
		xml->AddElem("ErrorMsg",ErrorMsg);
		xml->AddElem("ErrorId",ErrorId);
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:ErrorMsgXML Function:"<<e.what()<<endl;
	}
}



void XMLCreator::InteractionFailureAction(char* ErrorMsg, char* ErrorId,char* ActionName){
	try
	{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "InteractionResponse" );
		xml->AddAttrib("ActionName", ActionName);
		xml->IntoElem();
		NewErrorMsgXML(ErrorMsg,ErrorId);
		xml->OutOfElem();

	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:LoginFailure Function:"<<e.what()<<endl;
	}

}

void XMLCreator::InteractionSuccessAction(char* ActionName, char* msg ){
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "InteractionResponse" );
		xml->AddAttrib("ActionName", ActionName);
		xml->IntoElem();
		xml->AddElem( "Success",msg );
		xml->OutOfElem();
		//xml->OutOfElem();
	}
	catch (exception &e){
		cout<<"Exception caught in XMLCreator:LoginSucess Function:"<<e.what()<<endl;
	}
}


void XMLCreator::SuccessRestream(int restreamID,char * actionName ){
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", actionName);
		xml->IntoElem();
		xml->AddElem( "Success" );
		xml->IntoElem();
		xml->AddElem( "RestreamID", restreamID );
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:LoginFailure Function:"<<e.what()<<endl;
	}
}

void XMLCreator::NewFailureAction(const char* ErrorMsg, const char* ErrorId,const char* ActionName)
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", ActionName);
		xml->IntoElem();
		NewErrorMsgXML(ErrorMsg,ErrorId);
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:LoginFailure Function:"<<e.what()<<endl;
	}

}


void XMLCreator::NewCreateLoginAction(char* UserName, char* Password)
{
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "Login");
		xml->IntoElem();
		xml->AddElem("UserName",UserName);
		xml->AddElem("Password",Password);
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:LoginFailure Function:"<<e.what()<<endl;
	}

}



void XMLCreator::CreateInteractionXml(){
	try{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		NewFetchDeviceInfo();
		NewFetchAllChannel(false);
		NewFetchAllCGroup(false);
		NewFetchAllChannelCGroupRelation(false);
		NewFetchAllUser(false);
		NewFetchAllUGroup(false);
		NewFetchAllUserUGroupRelation(false);
		NewFetchUGroupCGroupRelation(false);
		NewRtspInfo();
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:CreateInteractionXml Function:"<<e.what()<<endl;
	}

}


void XMLCreator::CreateGetDeviceInfo(char *DeviceIP){
	ReInitializeXML();
	xml->OutOfElem();
	if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
		xml->AddElem( "XMLDakshaImaging" );
	xml->IntoElem();
	xml->AddElem( "Action" );
	xml->AddAttrib("Name", "GetDeviceInfo");
	xml->IntoElem();
	xml->AddElem("DeviceIP",DeviceIP);
	xml->OutOfElem();
}

void XMLCreator::NewSuccessDeviceInfoAction( int DID,char *macAddress, char * deviceName){
	ReInitializeXML();
	xml->OutOfElem();
	if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
		xml->AddElem( "XMLDakshaImaging" );
	xml->IntoElem();
	xml->AddElem( "Response" );
	xml->AddAttrib("ActionName", "GetDeviceInfo");
	xml->IntoElem();
	xml->AddElem("Success");
	xml->IntoElem();
	xml->AddElem("DID",DID);
	xml->AddElem("MACADDRESS",macAddress);
	xml->AddElem("DEVICENAME",deviceName);
	xml->OutOfElem();
}



void XMLCreator::NewUGroupCGroupInfo(const char *DID,const char *UGID,const char *CGID,const char *CGNAME , const char * UGNAME,const char *UGCGID, bool isAllChannel,const char * UID ){
	try{
		if ( !isAllChannel ){
			xml->AddElem("UGCGID",UGCGID);
			xml->AddElem( "UID",UID);
			xml->AddElem("DID",DID);
		}
		else{
			xml->AddElem( "UGName",UGNAME);
			xml->AddElem( "CGName",CGNAME);
		}
		xml->AddElem("UGID",UGID);
		xml->AddElem( "CGID",CGID);
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:UGroupINfo Function:"<<e.what()<<endl;
	}
}

void XMLCreator::NewFetchUGroupCGroupRelation(bool isAllChannel )
{
	try
	{
		vector<string> UGCGID,UGID,CGID,CGNAME,UGNAME,UID,DID;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		if ( isAllChannel ){
			xml->AddElem( "Response" );
		}
		else
			xml->AddElem( "Interaction" );
		xml->AddAttrib("ActionName", "FetchUGroupCGroupRelation");
		xml->IntoElem();
		if ( isAllChannel )
			DataBaseAccessObj->GetUGroupCGroupRelation(UGID,CGID,CGNAME,UGNAME);
		else{
			DataBaseAccessObj->GetUGroupCGroupRelationOnlyOwners (UGID,CGID,UGCGID,UID,DID);
		}

		for( int i=0;i < (int)UGID.size();i++){

			xml->AddElem("UGroupCGroupInfo");
			xml->IntoElem();
			if ( isAllChannel ){
				NewUGroupCGroupInfo((const char *)NULL,UGID[i].c_str(),CGID[i].c_str(),CGNAME[i].c_str(),UGNAME[i].c_str(),(const char *)NULL, isAllChannel,(const char *)NULL );
			}else{
				NewUGroupCGroupInfo(DID[i].c_str(),UGID[i].c_str(),CGID[i].c_str(),(const char *)NULL,(const char *)NULL,UGCGID[i].c_str(), isAllChannel,UID[i].c_str() );

			}
			xml->OutOfElem();
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchUGroupCGroupRelation Function:"<<e.what()<<endl;
	}

}
void XMLCreator::NewProcessMontitoringInfo(const char *processCPU,const char *totalCPU,const char *virtualMemory,const char *physicalMemory,const char *dataSend, const char *dataReceive ){
	try{

		xml->AddElem("PCPU",processCPU);
		xml->AddElem( "OCPU",totalCPU);
		xml->AddElem( "VM",virtualMemory);
		xml->AddElem( "PM",physicalMemory);
		xml->AddElem("DS",dataSend);
		xml->AddElem( "DR",dataReceive);

	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:UGroupINfo Function:"<<e.what()<<endl;
	}
}



void XMLCreator::CreateProcessMonitoringXML(int LoggedDID,  char * fillterValue ,  char * startDate ,  char * startTime ,  char * endDate ,  char * endTime )
{
	try{
		vector<string> processCPU,totalCPU,virtualMemory,physicalMemory,dataSend,dataReceive;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "StartProcessMonitoring");
		xml->IntoElem();
		if ( strcmp(fillterValue,"avg") == 0 )
			DataBaseAccessObj->GetAvgProcessMonitoring(processCPU,totalCPU,virtualMemory,physicalMemory,dataSend, dataReceive, startDate, startTime, endDate,endTime);
		else if ( strcmp(fillterValue,"min") == 0 )
			DataBaseAccessObj->GetMinProcessMonitoring(processCPU,totalCPU,virtualMemory,physicalMemory,dataSend, dataReceive, startDate, startTime, endDate,endTime);
		else if ( strcmp(fillterValue,"max") == 0 )
			DataBaseAccessObj->GetMaxProcessMonitoring(processCPU,totalCPU,virtualMemory,physicalMemory,dataSend, dataReceive, startDate, startTime, endDate,endTime);
		else if ( strcmp(fillterValue,"live") == 0 )
			DataBaseAccessObj->GetLiveProcessMonitoring(processCPU,totalCPU,virtualMemory,physicalMemory,dataSend, dataReceive);
		if ( processCPU.size() > 0  ){
			for( int i = 0; i < (int)processCPU.size(); i++){
				xml->AddElem("ProcessInfo");
				xml->IntoElem();
				NewProcessMontitoringInfo(processCPU[i].c_str(),totalCPU[i].c_str(),virtualMemory[i].c_str(),physicalMemory[i].c_str(),dataSend[i].c_str(),dataReceive[i].c_str() );
				xml->OutOfElem();
			}
		}else{
			xml->AddElem("Error","no data available");
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchUGroupCGroupRelation Function:"<<e.what()<<endl;
	}
}

string XMLCreator::CheckBool ( bool value ){
	if ( !value )
		return "Disable";
	else
		return "Enable";
}

void XMLCreator::CreateAllChannelDetails( int DID ){
	try
	{
		vector<string> UGCGID,UGID,CGID,CGNAME,UGNAME,UID;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "GetAllChannelDetailsForDID");
		xml->IntoElem();
		vector< std::string >  cName;
		vector<bool> dumpingStatus ,activeStatus ,resendStatus ,retrievelStatus,channelModeStatus;
		DataBaseAccessObj->GetAllChannelDetailsForDID( DID , cName, dumpingStatus ,activeStatus ,resendStatus ,retrievelStatus,channelModeStatus );

		for( int i=0;i < (int)cName.size();i++){
			xml->AddElem("ChannelInfo");
			xml->IntoElem();
			xml->AddElem("ChannelName", cName[i]);
			xml->AddElem("Dumping", CheckBool(dumpingStatus[i]).c_str());
			xml->AddElem("Resend", CheckBool(!resendStatus[i]).c_str());
			xml->AddElem("Restream", CheckBool(resendStatus[i]).c_str());
			xml->AddElem("Retrieve", CheckBool(retrievelStatus[i]).c_str());
			xml->AddElem("ReplicaMode", CheckBool(channelModeStatus[i]).c_str());
			xml->AddElem("Status", CheckBool(activeStatus[i]).c_str());
			xml->OutOfElem();
		}
		if ( cName.size()  == 0 ){
			NewErrorMsgXML("Channel does not exists","12");
		}

		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchUGroupCGroupRelation Function:"<<e.what()<<endl;
	}
}

void XMLCreator::CreateOfflineAnalysisDetails( int DID, char * StartDate, char *StartTime,char *EndDate,char *EndTime )
{

	try{
		int LIMITRow = 800;
		vector<string> processCPU,totalCPU,virtualMemory,physicalMemory,dataSend,dataReceive;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "OfflinePerfomance");
		xml->IntoElem();
		DataBaseAccessObj->GetOffLinePerformanceMonitoring(processCPU,totalCPU,virtualMemory,physicalMemory,dataSend, dataReceive, StartDate, StartTime, EndDate,EndTime,DID,LIMITRow);
		if ( processCPU.size() > 0  ){
			for( int i = 0; i < (int)processCPU.size(); i++){
				xml->AddElem("ProcessInfo");
				xml->IntoElem();
				NewProcessMontitoringInfo(processCPU[i].c_str(),totalCPU[i].c_str(),virtualMemory[i].c_str(),physicalMemory[i].c_str(),dataSend[i].c_str(),dataReceive[i].c_str() );
				xml->OutOfElem();
			}
		}else{
			xml->AddElem("Error","no data available");
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchUGroupCGroupRelation Function:"<<e.what()<<endl;
	}
}

void XMLCreator::CloseUserSession(){
	cout<<"\nClosing User Session\n";
	DataBaseAccessObj->UpdateActiveUserStatus(AUID, 0);
}

void XMLCreator::NewActiveUserXML(const char *userName,int userRole, const char * lastLoginTime, const char * ip ){
	xml->AddElem("UserName", userName);
	if ( userRole == 0 )
		xml->AddElem("UserRole", "Admin");
	else
		xml->AddElem("UserRole", "Client");
	xml->AddElem("LastLogin", lastLoginTime);
	xml->AddElem("SystemIP", ip);

}




void XMLCreator::CreateStorageInfoXML(char *DID){
	try{

		vector<string> userName,lastLoginTime;
		vector<int> userRole;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "FetchStorageInfo");
		xml->IntoElem();
		xml->AddElem("StorageInfo");
		xml->IntoElem();
		xml->AddElem("MaxSpace","10000000000");
		xml->AddElem("FreeSpace","100000000");
		xml->OutOfElem();
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchUGroupCGroupRelation Function:"<<e.what()<<endl;
	}
}


void XMLCreator::CreateActiveUserXML(char *DID){
	try{

		vector<string> userName,lastLoginTime, ip;
		vector<int> userRole;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "ActiveUser");
		xml->IntoElem();
		DataBaseAccessObj->GetActiveUserInfo(atoi(DID),userName, userRole, lastLoginTime, ip);
		if ( userName.size() > 0  ){
			for( int i = 0; i < (int)userName.size(); i++){
				xml->AddElem("UserInfo");
				xml->IntoElem();
				NewActiveUserXML(userName[i].c_str(),userRole[i],lastLoginTime[i].c_str(),ip[i].c_str());
				xml->OutOfElem();
			}
		}else{
			xml->AddElem("Error","no data available");
		}
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchUGroupCGroupRelation Function:"<<e.what()<<endl;
	}
}


//*************************29 Jan 2016 ************************************/
void XMLCreator::CreateAllChannelWithoutDetail(  const char * searchText ){
	try
	{
		vector<string> UGCGID,UGID,CGID,CGNAME,UGNAME,UID;
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "FetchAllChannelWithoutDetail");
		xml->IntoElem();
		vector< std::string >  cName,cID;
		//vector<bool> dumpingStatus ,activeStatus ,resendStatus ,retrievelStatus,channelModeStatus;
		DataBaseAccessObj->GetAllChannelWithoutDetails( cID , cName,searchText);

		for( int i=0;i < (int)cName.size();i++){
			xml->AddElem("CInfo");
			xml->IntoElem();
			xml->AddElem("CName", cName[i]);
			xml->AddElem("CID",cID[i]);
			xml->OutOfElem();
		}
		if ( cName.size()  == 0 ){
			NewErrorMsgXML("Channel does not exists","12");
		}

		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchAllChannelWithoutDetail Function:"<<e.what()<<endl;
	}
}

void XMLCreator::CreateAllChannelGroupWithoutDetail( const char * searchText ){
	try
	{

		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "FetchAllChannelGroupWithoutDetail");
		xml->IntoElem();
		vector< std::string >  CGName,CGID;
		//vector<bool> dumpingStatus ,activeStatus ,resendStatus ,retrievelStatus,channelModeStatus;
		DataBaseAccessObj->GetAllChannelGroupWithoutDetails( CGID , CGName,searchText);

		for( int i=0;i < (int)CGName.size();i++){
			xml->AddElem("CGInfo");
			xml->IntoElem();
			xml->AddElem("CGName", CGName[i]);
			xml->AddElem("CGID",CGID[i]);
			xml->OutOfElem();
		}
		if ( CGName.size()  == 0 ){
			NewErrorMsgXML("ChannelGroup does not exists","12");
		}

		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchAllChannelGroupWithoutDetail Function:"<<e.what()<<endl;
	}
}


void XMLCreator::CreateAllUserWithoutDetail(  const char * searchText){
	try
	{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "FetchAllUserWithoutDetail");
		xml->IntoElem();
		vector< std::string >  UName,UID;
		DataBaseAccessObj->GetAllUserWithoutDetails( UID , UName,searchText);
		cout<<"\nUID Size ---> "<<UID.size()<<endl;
		for( int i=0;i < (int)UID.size();i++){
			xml->AddElem("UInfo");
			xml->IntoElem();
			xml->AddElem("UName", UName[i].c_str());
			xml->AddElem("UID",UID[i]);
			xml->OutOfElem();
		}
		if ( UID.size()  == 0 ){
			NewErrorMsgXML("User does not exists","12");
		}

		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:CreateAllUserWithoutDetail Function:"<<e.what()<<endl;
	}
}

void XMLCreator::CreateAllUserGroupWithoutDetail(  const char * searchText){
	try
	{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "FetchAllUserGroupWithoutDetail");
		xml->IntoElem();
		vector< std::string >  UGName,UGID;
		DataBaseAccessObj->GetAllUserGroupWithoutDetails( UGID , UGName,searchText);

		for( int i=0;i < (int)UGID.size();i++){
			xml->AddElem("UGInfo");
			xml->IntoElem();
			xml->AddElem("UGName", UGName[i].c_str());
			xml->AddElem("UGID",UGID[i]);
			xml->OutOfElem();
		}
		if ( UGID.size()  == 0 ){
			NewErrorMsgXML("User Group does not exists","12");
		}

		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchUGroupCGroupRelation Function:"<<e.what()<<endl;
	}
}

void XMLCreator::CreateAllDeviceWithoutDetail( const char * searchText){
	try
	{

		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "FetchAllDeviceWithotuDetail");
		xml->IntoElem();
		vector< std::string >  DName,DID;
		DataBaseAccessObj->GetAllDeviceWithoutDetails( DID , DName, searchText);

		for( int i=0;i < (int)DID.size();i++){
			xml->AddElem("DInfo");
			xml->IntoElem();
			xml->AddElem("DName", DName[i].c_str());
			xml->AddElem("DID",DID[i]);
			xml->OutOfElem();
		}
		if ( DID.size()  == 0 ){
			NewErrorMsgXML("Device does not exists","12");
		}

		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchAllDeviceWithotuDetail Function:"<<e.what()<<endl;
	}
}


void XMLCreator::CreateNode(vector<int> ID,vector<string> Name,const char *IDTag, const char *nameTag, const char *parentTagName ){

	for( int i = 0 ; i < (int)Name.size() ; i++ ){
		xml->AddElem(parentTagName);
		xml->IntoElem();
		xml->AddElem(nameTag, Name[i].c_str());
		xml->AddElem(IDTag,ID[i]);
		xml->OutOfElem();
	}

}

void XMLCreator::CreateRelationsXML(vector<int> IDLinkFrom,vector<int> IDLinkTo,const char *parentTag,const char *tagLinkFrom,const char *tagLinkTo){
	xml->AddElem(parentTag);
	xml->IntoElem();
	for( int i = 0 ; i < (int)IDLinkFrom.size() ; i++ ){
		xml->AddElem("LINK");
		xml->IntoElem();
		xml->AddElem(tagLinkFrom, IDLinkFrom[i]);
		xml->AddElem(tagLinkTo,IDLinkTo[i]);
		xml->OutOfElem();
	}
	xml->OutOfElem();
}

void XMLCreator::CreateRelationTableIntoXml( vector<int> relationTo,vector<int>relationFrom,vector<int>IDLinkFrom,vector<int>IDLinkTo ,const char * parentTag, const char *tagLinkFrom,const char *tagLinkTo, const char *tableName){
	DataBaseAccessObj->GetRelation( relationTo,relationFrom, IDLinkFrom, IDLinkTo, tagLinkFrom, tagLinkTo, tableName );
	CreateRelationsXML(IDLinkFrom,IDLinkTo,parentTag,tagLinkFrom,tagLinkTo);
	IDLinkFrom.clear();
	IDLinkTo.clear();
}

void XMLCreator::CreateRelationXML(vector<int>UID,vector<int>UGID,vector<int>CGID,vector<int>CID,vector<int>DID){

	vector< int > tempUID , tempUGID, tempCGID, tempCID, tempDID;
	CreateRelationTableIntoXml(UID,UGID,tempUID,tempUGID,"UIDUGIDRelation", "UID","UGID","userugrouprelation");
	CreateRelationTableIntoXml(UGID,CGID,tempUGID,tempCGID,"UGIDCGIDRelation", "UGID","CGID","ugroupcgrouprelation");
	CreateRelationTableIntoXml(CGID,CID,tempCGID,tempCID,"CGIDCIDRelation", "CGID","CID","channelcgrouprelation");
	CreateRelationTableIntoXml(CID,DID,tempCID, tempDID,"CIDDIDRelation", "CID","DID","channelinfo");
}

void XMLCreator::CreateGraphInfo(vector<int>UID,vector<int>UGID,vector<int>CGID,vector<int>CID,vector<int>DID){
	try
	{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "GetGraphInfo");
		xml->IntoElem();
		vector<string> UName, UGName,CGName,CName,DName;
		DataBaseAccessObj->GetName(UID,UGID,CGID,CID,DID,UName,UGName,CGName,CName,DName);
		CreateNode(UID, UName,"UID","UNAME","UInfo");
		CreateNode(UGID, UGName,"UGID","UGNAME","UGInfo");
		CreateNode(CGID, CGName,"CGID","CGNAME","CGInfo");
		CreateNode(CID, CName,"CID","CNAME","CInfo");
		CreateNode(DID, DName,"DID","DNAME","DInfo");
		CreateRelationXML(UID,UGID,CGID,CID,DID);
		xml->OutOfElem();

	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchAllDeviceWithotuDetail Function:"<<e.what()<<endl;
	}
}

void XMLCreator::CreateInactiveChannelXml(char *CID,char *DID, char * xmlData){
	try
	{
		CMarkup *xmlTemp = new CMarkup();
		xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Action" );
		xml->AddAttrib("Name", "InactiveChannel");
		xml->IntoElem();
		xml->AddElem( "CID",CID );
		xml->AddElem( "DID",DID );
		xml->OutOfElem();
		sprintf ( xmlData, "%s", xmlTemp->GetDoc().c_str());
		delete xmlTemp;
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:CreateInactiveChannelXml Function:"<<e.what()<<endl;
	}
}

void XMLCreator::CreateLoginSuccessXml( int UID){
	try
	{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "LoginFromQT");
		xml->IntoElem();
		vector< std::string >  CGNAME,CGID, CNAME;

		DataBaseAccessObj->GetAllChannelGroupForUID(UID,CGID,CGNAME);

		xml->AddElem("treeinfo");
		xml->IntoElem();
		xml->AddElem( "name","channelgroup" );

		if ( CGID.size() == 0 )
			xml->AddElem("channelgroup");
		for( int i=0;i < (int)CGID.size();i++){
			CNAME.clear();
			DataBaseAccessObj->GetChannelForCGID(CGID[i],CNAME);
			xml->AddElem("channelgroup");
			xml->IntoElem();
			xml->AddElem("titlename", CGNAME[i].c_str());
			xml->AddElem("name","channel");
			if ( (int)CNAME.size() > 0 ){
				for ( int  j = 0 ; j < (int)CNAME.size(); j++ ){

					xml->AddElem("channel");
					xml->IntoElem();
					xml->AddElem( "titlename", CNAME[j].c_str() );
					xml->AddElem("name","action");
					xml->AddElem("action");
					xml->IntoElem();
					xml->AddElem("titlename","Video");
					xml->OutOfElem();
					xml->AddElem("action");
					xml->IntoElem();
					xml->AddElem("titlename","Playback");
					xml->OutOfElem();
					xml->OutOfElem();
				}
				xml->OutOfElem();
			}
			else
				xml->AddElem("channel");
		}
		xml->OutOfElem();
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchAllDeviceWithotuDetail Function:"<<e.what()<<endl;
	}
}

void XMLCreator::CreateChannelInfo( char * rtsp){
	try
	{
		xml->OutOfElem();
		if ( !FindElement(xml->GetDoc(),"XMLDakshaImaging") )
			xml->AddElem( "XMLDakshaImaging" );
		xml->IntoElem();
		xml->AddElem( "Response" );
		xml->AddAttrib("ActionName", "GetChannelInfo");
		xml->IntoElem();
		xml->AddElem( "RTSP",rtsp);
		xml->OutOfElem();
	}catch (exception &e){
		cout<<"Exception caught in XMLCreator:FetchAllDeviceWithotuDetail Function:"<<e.what()<<endl;
	}
}


/*************************************************/
