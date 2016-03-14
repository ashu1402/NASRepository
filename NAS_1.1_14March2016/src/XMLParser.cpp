#include "XMLParser.h"

XMLParser::XMLParser(int fd,Retrieve* Obj,VOD* vodObj,Notification *notifi, char * clientIP, bool isAndroidStatus, Failover * failoverObj,Interaction * interactionInfo )
{
	sockfd= fd;
	XMLCreatorObj= NULL;

	DataBaseAccessObj = new DataBaseAccess();
	RetrieveObj = Obj;
	VODObj=vodObj;
	notification = notifi;
	CommonClassObj = new CommonClass();

	LoggedDID=DataBaseAccessObj->GetSystemDID();
	LoggedUID=-1;
	clientIp = (char*)malloc(sizeof(char)*strlen(clientIP));
	outputPath = (char*)malloc(sizeof(char)*SIZEOFCHARARRAYBIG);
	sprintf( clientIp,"%s",clientIP);
	isAndroid = isAndroidStatus;
	XMLCreatorObj = new XMLCreator();
	failover = failoverObj;
    interaction = interactionInfo;

}

XMLParser::~XMLParser()
{

	if ( DataBaseAccessObj != NULL ){
		cout<<"\n Closing DataBase object \n";
		DataBaseAccessObj->Close();
		delete DataBaseAccessObj;
		DataBaseAccessObj = NULL;
	}
	if ( XMLCreatorObj != NULL ){
		cout<<"\nClosing XMLCreator Object\n";
		delete XMLCreatorObj;
	}
}

string XMLParser::getFileContents(char* filename)
{
	char * buffer = 0;
	long length;
	FILE * f = fopen (filename, "r");

	if (f)
	{
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		fseek (f, 0, SEEK_SET);
		buffer = (char*)malloc (length);
		if (buffer)
		{
			fread (buffer, 1, length, f);
		}
		fclose (f);
	}

	string str(buffer);
	if(buffer!=NULL)
		free(buffer);
	return str;

}

xml_node<>* XMLParser::getNodeByName(const string& nodeName, xml_node<>* parentNode)
{
	try{
		if(parentNode == NULL)
		{
			throw runtime_error("parentNode is NULL");
		}

		xml_node<>* node = parentNode->first_node();

		while(node != NULL)
		{
			if(node->name() == nodeName)
			{
				return node;
			}

			node = node->next_sibling();
		}
	}catch (exception &e){
		cout<<"Exception caught in getNodeByName Function:"<<e.what()<<endl;
	}
	return NULL;
}


bool XMLParser::AddUser(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		xml_node<>* name = getNodeByName("UName",node);
		xml_node<>* username = getNodeByName("UserName",node);
		xml_node<>* password = getNodeByName("Password",node);
		xml_node<>* userrole = getNodeByName("UserRole",node);
		xml_node<>* email = getNodeByName("Email",node);
		xml_node<>* mobile = getNodeByName("ContactNo",node);
		xml_node<>* status = getNodeByName("Status",node);


		if( name==NULL || username==NULL || password==NULL || userrole==NULL ){
			cout<<"AddUser FAiled\n";
			XMLCreatorObj->NewFailureAction("The username or password or userrole you entered is NULL","101","AddUser");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","AddUser");
		}else{

			int DID = DataBaseAccessObj->GetSystemDID();
			//char* emailValue = (char*)malloc(sizeof(char)*1000);
			char emailValue[1000];
			if(email==NULL)
				sprintf(emailValue,"%s","Default@default.com");
			else
				sprintf(emailValue,"%s",email->value());

			char mobileValue[1000];
			if(mobile==NULL)
				sprintf(mobileValue,"%s","00000000000");
			else
				sprintf(mobileValue,"%s",mobile->value());

			int UID = -1;
			CreateUniqueUID(DID,UID);
			int ret = DataBaseAccessObj->InsertUserInfo(UID, name->value(),username->value(),password->value(),atoi(userrole->value()),emailValue,mobileValue,DID,atoi(status->value())	);
			if(ret){
				cout<<"User successfully inserted\n";
				xml_node<>* AddUserUGroup = getNodeByName("AddUserUGroup",node);
				if(AddUserUGroup!=NULL){
					int UUGID = 0;
					xml_node<>* UGID = getNodeByName("UGID",AddUserUGroup);
					while(UGID!=NULL){
						CreateUniqueUUGID(DID,UUGID);
						int ret1 = DataBaseAccessObj->InsertUserUGroupRelation(UUGID,UID,atoi(UGID->value()),DID);
						if(ret1){
							cout<<"AddUserUgroup Success\n";
							//XMLCreatorObj->NewSuccessAction("AddUserUgroup","Ok");
						}
						else{
							cout<<"failed to Insert the Mapping into the databases\n";
							XMLCreatorObj->NewFailureAction("Failed to insert userugroup mapping into the Database","103","AddUser");
						}
						UGID=UGID->next_sibling("UGID");
					}
				}
				cout<<"AddUser Success\n";
				char UserID[1000];
				sprintf(UserID,"%d",UID);
				XMLCreatorObj->NewSuccessAddUserAction(UserID);
			}else{
				cout<<"AddUser FAiled\n";
				XMLCreatorObj->NewFailureAction("Failed to insert userinfo  into the Database","103","AddUser");
			}
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddUser Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::UpdateUser(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		xml_node<>* UID = getNodeByName("UID",node);
		xml_node<>* name = getNodeByName("UName",node);
		xml_node<>* username = getNodeByName("UserName",node);
		xml_node<>* password = getNodeByName("Password",node);
		xml_node<>* userrole = getNodeByName("UserRole",node);
		xml_node<>* email = getNodeByName("Email",node);
		xml_node<>* mobile = getNodeByName("ContactNo",node);
		xml_node<>* status = getNodeByName("Status",node);


		if(UID==NULL || name==NULL || username==NULL || password==NULL || userrole==NULL || email==NULL || mobile==NULL || status==NULL){
			cout<<"UpdateUser FAiled\n";
			XMLCreatorObj->NewFailureAction("The username or password or userrole you entered is NULL","101","UpdateUser");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","UpdateUser");
		}else{
			int DID = DataBaseAccessObj->GetSystemDID();
			int ret = DataBaseAccessObj->UpdateUserInfo(UID->value(),name->value(),username->value(),password->value(),userrole->value(),email->value(),mobile->value(),status->value());
			if(ret){
				cout<<"User successfully updated\n";
				xml_node<>* AddUserUGroup = getNodeByName("AddUserUGroup",node);
				if(AddUserUGroup!=NULL){
					int UUGID = -1;
					xml_node<>* UGID = getNodeByName("UGID",AddUserUGroup);
					while(UGID!=NULL){
						CreateUniqueUUGID(DID,UUGID);
						int ret1 = DataBaseAccessObj->InsertUserUGroupRelation(UUGID, atoi(UID->value()),atoi(UGID->value()),DID);
						if(ret1){
							cout<<"AddUserUgroup Success\n";
							//XMLCreatorObj->NewSuccessAction("AddUserUgroup","Ok");
						}
						else{
							cout<<"failed to Insert the Mapping into the databases\n";
							XMLCreatorObj->NewFailureAction("Failed to insert userugroup mapping into the Database","103","AddUser");
						}
						UGID=UGID->next_sibling("UGID");
					}
				}
				xml_node<>* RemoveUserUGroup = getNodeByName("RemoveUserUGroup",node);
				if(RemoveUserUGroup!=NULL){
					xml_node<>* UGID = getNodeByName("UGID",RemoveUserUGroup);
					while(UGID!=NULL){
						int ret2 = DataBaseAccessObj->DeleteUserUGroupRelation(atoi(UID->value()),atoi(UGID->value()));
						if(ret2){
							cout<<"RemoveUserUgroup Success\n";
							//XMLCreatorObj->NewSuccessAction("AddUserUgroup","Ok");
						}
						else{
							cout<<"failed to Remove the Mapping into the databases\n";
							XMLCreatorObj->NewFailureAction("Failed to remove userugroup mapping into the Database","103","AddUser");
						}
						UGID=UGID->next_sibling("UGID");
					}
				}
				cout<<"UpdateUser Success\n";
				XMLCreatorObj->NewSuccessAction("UpdateUser","Ok");
			}else{
				cout<<"UpdateUser FAiled\n";
				XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","UpdateUser");
			}
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in UpdateUser Function"<<e.what()<<endl;
	}
	return false;

}


bool XMLParser::LookingForServer(char * address , int port, int &sockFd){

	struct sockaddr_in serv_addr;
	struct hostent *server;
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFd < 0){
		return false;
	}
	server = gethostbyname(address);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		return false;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(port);

	if ( connect(sockFd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		fprintf(stderr,"ERROR connecting");
		return false;
	}
	else{
		return true;
	}
}

bool XMLParser::ParseDeviceInfo(char *macValue, int &enteredDeviceID,char *xmlData, char * deviceName){
	rapidxml::xml_document<> doc;
	doc.parse<0> (xmlData);

	xml_node<>* currNode = doc.first_node();
	string nodeName;
	nodeName = currNode->name();
	if(nodeName == "XMLDakshaImaging"){
		xml_node<>* currNode1  = currNode->first_node();
		string nodeName = currNode1->name();
		if(nodeName=="Response"){
			xml_attribute<>* attribute = currNode1->first_attribute();
			//while(attribute){
			string attributeName = attribute->name();
			if(attributeName=="ActionName"){
				string attributeValue = attribute->value();
				if ( attributeValue == "GetDeviceInfo"){
					xml_node<>* Success = getNodeByName("Success",currNode1);
					xml_node<>* DID = getNodeByName("DID",Success);
					xml_node<>* macAddress = getNodeByName("MACADDRESS",Success);
					xml_node<>* deviceNameInfo = getNodeByName("DEVICENAME",Success);

					if ( DID != NULL && macAddress != NULL && deviceName != NULL ){
						enteredDeviceID = atoi(DID->value());
						sprintf( macValue ,"%s",macAddress->value());
						sprintf( deviceName ,"%s",deviceNameInfo->value());
						cout<<"\ndeviceName"<<deviceName;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool XMLParser::GetDeviceInfo( int &enteredDeviceID,char *macValue, char * DeviceIP, int sockFD, char * deviceName ){

	XMLCreatorObj->CreateGetDeviceInfo(DeviceIP);
	string xml_string = XMLCreatorObj->NewconvertToString();
	cout<<"\nSending XML--->"<< xml_string<<endl;
	int size = xml_string.size();
	char xmlData[SIZEOFCHARARRAY];

	sendData(4,&size,sockFD);
	cout<<"Sending size="<<size<<endl;
	cout<<"\nSending Data"<<xml_string.c_str();
	sendData( size,xml_string.c_str(),sockFD );
	if(recvData(&size,sizeof(int),sockFD)){
		if ( recvData(xmlData,size, sockFD) ){
			cout<<"\n Receivecd xmlData"<<xmlData;
			return ParseDeviceInfo(macValue,enteredDeviceID,xmlData,deviceName);
		}
	}
	return false;
}

bool XMLParser::AddServer(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		xml_node<>* DeviceIP = getNodeByName("DeviceIP",node);
		int ownerValue = 0, statusValue, enteredDeviceID = 0, sockFD = -1;
		char macValue[1000],deviceName[SIZEOFCHARARRAY];
		char tempIP[100];
		if ( !DataBaseAccessObj->IsServerExist( DeviceIP->value(), NULL,enteredDeviceID,tempIP) ){
			if ( LookingForServer((DeviceIP->value()),55555,sockFD) ){
				if( DeviceIP == NULL ){
					cout<<"AddDevice FAiled\n";
					XMLCreatorObj->NewFailureAction("The DeviceName or password or DeviceIP you entered is NULL","101","AddDevice");
				}else if(0){
					XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","AddDevice");
				}else{

					statusValue = 1;
					GetDeviceInfo(enteredDeviceID,macValue, DeviceIP->value(),sockFD,deviceName);

					int ret = DataBaseAccessObj->InsertDeviceInfo(enteredDeviceID, deviceName,DeviceIP->value(),statusValue,ownerValue,macValue);
					if(ret){
						cout<<"AddDevice Success\n";
						char DeviceID[1000];
						sprintf(DeviceID,"%d",enteredDeviceID);
						cout<<"DID = "<<ret<<endl<<"after ="<<DeviceID<<endl;
						XMLCreatorObj->NewSuccessAddDeviceAction(DeviceID,deviceName);
					}else{
						cout<<"AddDevice FAiled\n";
						XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","AddDevice");
					}
				}
			}
			else{
				cout<<"AddDevice FAiled\n";
				XMLCreatorObj->NewFailureAction("Server Does not Exist","108","AddDevice");
			}
		}
		else{
			cout<<"AddDevice FAiled\n";
			XMLCreatorObj->NewFailureAction("server already Exists","109","AddDevice");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddDevice Function"<<e.what()<<endl;
	}
	return false;
}



bool XMLParser::AddDevice(xml_node<>* node)
{
	/*try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		xml_node<>* DeviceName = getNodeByName("DName",node);
		xml_node<>* DeviceIP = getNodeByName("DeviceIP",node);
		xml_node<>* Status = getNodeByName("Status",node);
		xml_node<>* Owner = getNodeByName("Owner",node);
		xml_node<>* MacId = getNodeByName("MacId",node);

		if(DeviceName==NULL || DeviceIP==NULL ){
			cout<<"AddDevice FAiled\n";
			XMLCreatorObj->NewFailureAction("The username or password or userrole you entered is NULL","101","AddDevice");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
	/*	XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","AddDevice");
		}else{
			int statusValue;
			if(Status==NULL)
				statusValue = 1;
			else
				statusValue = 0;

			int ownerValue;
			int DID = DataBaseAccessObj->GetSystemDID();
			if(DID==0){
				if(Owner!=NULL)
					ownerValue = atoi(Owner->value());
				else
					ownerValue=0;
			}else
				ownerValue=0;

			//char* macValue = (char*)malloc(sizeof(char)*1000);
			char macValue[1000];
			if(MacId==NULL)
				sprintf(macValue,"%s","00:00:00:00");
			else
				sprintf(macValue,"%s",MacId->value());


			int ret = 0;//DataBaseAccessObj->InsertDeviceInfo(DeviceName->value(),DeviceIP->value(),statusValue,ownerValue,macValue);
			if(ret){
				cout<<"AddDevice Success\n";
				char DeviceID[1000];
				sprintf(DeviceID,"%d",ret);
				cout<<"DID = "<<ret<<endl<<"after ="<<DeviceID<<endl;
				XMLCreatorObj->NewSuccessAddDeviceAction(DeviceID);
			}else{
				cout<<"AddDevice FAiled\n";
				XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","AddDevice");
			}
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddDevice Function"<<e.what()<<endl;
	}*/
	return false;
}

bool XMLParser::UpdateDevice(xml_node<>* node)
{
	//try{
	/*string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		xml_node<>* DID = getNodeByName("DID",node);
		xml_node<>* DeviceName = getNodeByName("DName",node);
		xml_node<>* DeviceIP = getNodeByName("DeviceIP",node);
		xml_node<>* Status = getNodeByName("Status",node);

		if(DID==NULL || DeviceName==NULL || DeviceIP==NULL || Status==NULL ){
			cout<<"AddDevice FAiled\n";
			XMLCreatorObj->NewFailureAction("The username or password or userrole you entered is NULL","101","UpdateDevice");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
	/*		XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","UpdateDevice");
		}else{

			int ret = DataBaseAccessObj->UpdateDeviceInfo(DID->value(),DeviceName->value(),DeviceIP->value(),Status->value());
			if(ret){
				cout<<"UpdateDevice Success\n";
				XMLCreatorObj->NewSuccessAction("UpdateDevice","Ok");
			}else{
				cout<<"UpdateDevice FAiled\n";
				XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","UpdateDevice");
			}
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddDevice Function"<<e.what()<<endl;
	}*/

	XMLCreatorObj->NewFailureAction("This action is not authenticate to UI","103","UpdateDevice");
	return true;

}


bool XMLParser::ParseSuccessChannel(char * receivedXmlData,int &CID){
	rapidxml::xml_document<> doc;
	doc.parse<0> (receivedXmlData);

	xml_node<>* currNode = doc.first_node();
	string nodeName;
	nodeName = currNode->name();
	if(nodeName == "XMLDakshaImaging"){
		xml_node<>* currNode1  = currNode->first_node();
		nodeName = currNode1->name();
		if(nodeName=="Response"){
			xml_attribute<>* attribute = currNode1->first_attribute();
			//while(attribute){
			string attributeName = attribute->name();
			if(attributeName=="ActionName"){
				string attributeValue = attribute->value();
				if ( attributeValue == "AddChannel"){
					xml_node<>* Success = currNode1->first_node("Success");
					if ( Success != NULL ){
						xml_node<>* CIDValue = getNodeByName("CID",Success);
						if ( CIDValue != NULL ){
							CID = atoi(CIDValue->value());
							return false;
						}
					}
				}
			}
			return true;
		}
		else
			return true;
	}
	return true;
}

bool XMLParser::CheckForError( char *receivedXmlData, int &CID ) {

	try{
		string str = (const char *)receivedXmlData;
		int pos = -1;
		pos = str.find("Error");
		if ( pos!= str.npos ){
			cout<<"\nCheckForError return true\n";
			return true;
		}
		else{
			cout<<"\nCheckForError return false\n";

			return ParseSuccessChannel(receivedXmlData,CID);
		}
	}
	catch(std::exception e){

	}
	cout<<"\nCheckForError return true\n";
	return false;//for success
}

void XMLParser::CreateNewCID(int DID, int &CID){
	int count = DataBaseAccessObj->GetRowCount("channelinfo");
	CID = count + 65535 * DID;
	while(1){
						if ( !DataBaseAccessObj->CheckIDExistInTable("channelinfo","CID",CID) )
							break;
						CID += 1;
					}
}

//AddServer(node);
void XMLParser::CreateUniqueUID(int DID, int &UID){
	int count = DataBaseAccessObj->GetRowCount("userinfo");
	UID = count + 65535 * DID;
	while(1){
					if ( !DataBaseAccessObj->CheckIDExistInTable("userinfo","UID",UID) )
						break;
					UID += 1;
				}
}

void XMLParser::CreateUniqueUGID (int DID, int&UGID){
	int count = DataBaseAccessObj->GetRowCount("ugroupinfo");
	UGID = count + 65535 * DID;
	while(1){
				if ( !DataBaseAccessObj->CheckIDExistInTable("ugroupinfo","UGID",UGID) )
					break;
				UGID += 1;
			}
}

void XMLParser::CreateUniqueCGID( int DID, int &CGID ){
	int count = DataBaseAccessObj->GetRowCount("cgroupinfo");
	CGID = count + 65535 * DID;
	while(1){
			if ( !DataBaseAccessObj->CheckIDExistInTable("cgroupinfo","CGID",CGID) )
				break;
			CGID += 1;
		}
}

void XMLParser::CreateUniqueUUGID(int DID,int &UUGID){
	int count = DataBaseAccessObj->GetRowCount("userugrouprelation");
	UUGID = count + 65535 * DID;
	while(1){
		if ( !DataBaseAccessObj->CheckIDExistInTable("userugrouprelation","UUGID",UUGID) )
			break;
		UUGID += 1;
		cout<<"\nUUGID"<<UUGID<<endl;
	}
	cout<<"\nFinal UUGID--->"<<UUGID<<endl;
	}
void XMLParser::CreateUniqueCCGID(int DID,int &CCGID){
	int count = DataBaseAccessObj->GetRowCount("channelcgrouprelation");
	CCGID = count + 65535 * DID;
	while(1){
					if ( !DataBaseAccessObj->CheckIDExistInTable("channelcgrouprelation","CCGID",CCGID) )
						break;
					CCGID += 1;
				}
}

void XMLParser::CreateUniqueUGCGID(int DID,int &UGCGID){
	int count = DataBaseAccessObj->GetRowCount("ugroupcgrouprelation");
	UGCGID = count + 65535 * DID;
	while(1){
			if ( !DataBaseAccessObj->CheckIDExistInTable("ugroupcgrouprelation","UGCGID",UGCGID) )
				break;
			UGCGID += 1;
		}
}


bool XMLParser::AddChannelSub(xml_node<>* node , int DID, int &CID ){


	xml_node<>* ChannelName= getNodeByName("ChannelName",node);
	xml_node<>* IPAddress= getNodeByName("IPAddress",node);
	xml_node<>* UserName= getNodeByName("UserName",node);
	xml_node<>* Password= getNodeByName("Password",node);
	xml_node<>* PortNo= getNodeByName("PortNo",node);
	xml_node<>* RTSP= getNodeByName("RTSP",node);
	xml_node<>* StorageStatus= getNodeByName("StorageStatus",node);
	xml_node<>* StorageFileType= getNodeByName("StorageFileType",node);
	xml_node<>* StorageSpace= getNodeByName("StorageSpace",node);
	xml_node<>* RetrieverMode= getNodeByName("RetrieveMode",node);
	xml_node<>* Status= getNodeByName("Status",node);

	xml_node<> * PTZControl = getNodeByName("PTZ",node);
	xml_node<> * ReplicaMode = getNodeByName("ReplicaMode",node);
	xml_node<>* Make= getNodeByName("Make",node);
	xml_node<>* Model= getNodeByName("Model",node);
	xml_node<>* Desc= getNodeByName("Desc",node);
	xml_node<>* StreamMode= getNodeByName("StreamMode",node);
	xml_node<>* Motion= getNodeByName("Motion",node);

	char storagePath [1000];
	sprintf( storagePath,"/home/ubuntu/Frame");


	if ( ChannelName != NULL && IPAddress != NULL && UserName != NULL && Password != NULL && PortNo != NULL && RTSP != NULL && StorageStatus != NULL && StorageFileType != NULL && StorageSpace != NULL && RetrieverMode != NULL && Status != NULL  && PTZControl != NULL && ReplicaMode != NULL && Make != NULL && Model != NULL && Desc != NULL &&  StreamMode != NULL &&  Motion != NULL  ){

		if ( CID == -1 )
			CreateNewCID( DID, CID );

		int ret = DataBaseAccessObj->NewInsertChannelInfo(CID , ChannelName->value() , DID , Make->value() , Model->value() , UserName->value() , Password->value() , IPAddress->value() , PortNo->value() , RTSP->value() , storagePath , StorageSpace->value() , "0" , StorageFileType->value() , StorageStatus->value(),StreamMode->value() ,PTZControl->value() ,RetrieverMode->value(),ReplicaMode->value() ,Status->value(), Motion->value(), Desc->value());

		if(ret){

			cout<<"Channel successfully inserted\n";
			xml_node<>* AddChannelCGroup = getNodeByName("AddChannelCGroup",node);
			cout<<"\n5"<<endl;
			if(AddChannelCGroup!=NULL){
				int CCGID = -1;
				xml_node<>* CGID = getNodeByName("CGID",AddChannelCGroup);
				while(CGID!=NULL){
					CreateUniqueCCGID(DID,CCGID);
					int ret1 = DataBaseAccessObj->InsertchannelCGroupRelation(CCGID, CID,atoi(CGID->value()),DID,LoggedUID);
					if(ret1){
						cout<<"AddChannelCGroup Success\n";
					}
					else{
						cout<<"failed to Insert the Mapping into the databases\n";
						XMLCreatorObj->NewFailureAction("Failed to insert channelcgroup mapping into the Database","103","AddChannel");
					}
					CGID=CGID->next_sibling("CGID");
				}
				return true;
			}
			cout<<"AddChannel Success\n";
			char ChannelID[1000];
			sprintf(ChannelID,"%d",CID);
			char deviceIp[100];
			DataBaseAccessObj->GetDeviceIP(deviceIp,CID);
			XMLCreatorObj->NewSuccessAddChannelAction(ChannelID,deviceIp);

			int CID = ret;
			struct channelInfo channelObj;
			if( RetrieveObj == NULL || CommonClassObj == NULL)
				cout<<"Retriever or  Obj is NULL\n";
			else{
				CommonClassObj->copyChannelInfos(&channelObj,CID,ChannelName->value(),RTSP->value(),DID,storagePath,atoi(RetrieverMode->value()),(StorageSpace->value()),"0",StorageFileType->value(),atoi(StorageStatus->value()),atoi(StreamMode->value()),IPAddress->value(),PortNo->value(),UserName->value(), Password->value());
				RetrieveObj->CreateNewChannel(channelObj);
			}
		}
		return true;
	}
	else{
		cout<<"AddChannel FAiled\n";
		XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","AddChannel");
	}
	return false;
}

bool XMLParser::AddChannel(xml_node<>* node,char *fileContents)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		xml_node<>* DID= getNodeByName("DID",node);
		if(0){
			cout<<"AddChannel FAiled\n";
			XMLCreatorObj->NewFailureAction("The channelname you entered is NULL","101","AddChannel");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","AddChannel");
		}else{

			if ( DID != NULL ){
				int sockFD = -1 ,sysDID = DataBaseAccessObj->GetSystemDID(),CID = -1;
				int enteredDeviceID = atoi( DID->value() );
				if ( enteredDeviceID !=  sysDID ){
					char DeviceIP[100];
					if ( DataBaseAccessObj->IsServerExist( (char *)NULL, DID->value() ,enteredDeviceID, DeviceIP) ){
						if ( LookingForServer(DeviceIP,55555,sockFD) ){
							int receivedData = 0;
							char receivedXMLData[100000];
							int size = 100000;
							if ( sendData(sizeof(int),&size, sockFD) )
								if (sendData(size,fileContents, sockFD) )
									if ( recvData(&receivedData,sizeof(int),sockFD) )
										if ( recvData( receivedXMLData,receivedData,sockFD ) )
											if ( !CheckForError(receivedXMLData,CID) )
												AddChannelSub(node , enteredDeviceID,CID);
											else
												XMLCreatorObj->NewFailureAction("Unable To Insert","110","AddChannel");
						}
						else
							XMLCreatorObj->NewFailureAction("Server Does not Exists","110","AddChannel");
					}
					else
						XMLCreatorObj->NewFailureAction("Server Does not Exists","110","AddChannel");
				}
				else{
					cout<<"\nSame Device\n";
					if ( AddChannelSub(node , atoi(DID->value()),CID) ){
						if( CID == -1 ){
							XMLCreatorObj->NewFailureAction("Unable To Insert","110","AddChannel");
						}
						else{
							cout<<"\nSuccessFully Added channel\n";
							failover->isBreak = true;
							cout<<"\nBread failover thread ----> now onwards this system is not backup system\n"<<endl;
							XMLCreatorObj->NewSuccessActionWithID("AddChannel","ok","CID",CID);
						}
					}
				}
			}
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddChannel Function"<<e.what()<<endl;
	}
	return false;
}




void XMLParser::RestartChannel(int StartCID)
{
	vector<int> CID;
	vector<string> ChannelName;
	vector<string> RTSP;
	vector<int> DID;
	vector<string> StoragePath;
	vector<int> Retrivermode;
	vector<string> StorageSpace;
	vector<string> StorageSpaceUsed;
	vector<string> StorageFileType;
	vector<int> StorageStatus;
	vector<int> ReStream;
	vector<string> IpAddress;
	vector<string> portNumber ;
	vector<string> userName;
	vector<string>  password;


	int SysDID = DataBaseAccessObj->GetSystemDID();
	DataBaseAccessObj->GetActiveChannelInfoFromCID(StartCID,&CID,&ChannelName,&RTSP,&DID, &StoragePath,&Retrivermode,&StorageSpace,&StorageSpaceUsed,&StorageFileType,&StorageStatus,&ReStream,SysDID,&IpAddress,&portNumber,&userName,&password);

	for(int i = 0; i < (int)CID.size(); i++){
		struct channelInfo channelObj;
		CommonClassObj->copyChannelInfo(&channelObj,CID[i],ChannelName[i],RTSP[i],DID[i], StoragePath[i],Retrivermode[i],StorageSpace[i],StorageSpaceUsed[i],StorageFileType[i],StorageStatus[i],ReStream[i],IpAddress[i],portNumber[i],userName[i],password[i]);
		DataBaseAccessObj->ClearRtspInfo(CID[i]);
		RetrieveObj->RestartChannel(channelObj);

	}
}

bool XMLParser::UpdateChannelInSameMachine(xml_node<>* node, char *CID, int DID)
{
	//xml_node<>* CID = getNodeByName("CID",node);
	//xml_node<>* StoragePath = getNodeByName("StoragePath",node);
	xml_node<>* ChannelName= getNodeByName("ChannelName",node);
	xml_node<>* IPAddress= getNodeByName("IPAddress",node);
	xml_node<>* UserName= getNodeByName("UserName",node);
	xml_node<>* Password= getNodeByName("Password",node);
	xml_node<>* PortNo= getNodeByName("PortNo",node);
	xml_node<>* RTSP= getNodeByName("RTSP",node);
	xml_node<>* StorageStatus= getNodeByName("StorageStatus",node);
	xml_node<>* StorageFileType= getNodeByName("StorageFileType",node);
	xml_node<>* StorageSpace= getNodeByName("StorageSpace",node);
	xml_node<>* RetrieverMode= getNodeByName("RetrieveMode",node);
	xml_node<>* Status= getNodeByName("Status",node);
	xml_node<> * PTZControl = getNodeByName("PTZ",node);
	xml_node<> * ReplicaMode = getNodeByName("ReplicaMode",node);
	xml_node<>* Make= getNodeByName("Make",node);
	xml_node<>* Model= getNodeByName("Model",node);
	xml_node<>* Desc= getNodeByName("Desc",node);
	xml_node<>* StreamMode= getNodeByName("StreamMode",node);
	xml_node<>* Motion= getNodeByName("Motion",node);
	if ( ChannelName != NULL && IPAddress != NULL && UserName != NULL && Password != NULL && PortNo != NULL && RTSP != NULL && StorageStatus != NULL && StorageFileType != NULL && StorageSpace != NULL && RetrieverMode != NULL && Status != NULL  && PTZControl != NULL && ReplicaMode != NULL && Make != NULL && Model != NULL && Desc != NULL &&  StreamMode != NULL &&  Motion != NULL  ){
		int ret = DataBaseAccessObj->UpdateChannelInfo(DID, CID , ChannelName->value() , Make->value() , Model->value() , UserName->value() , Password->value() , IPAddress->value() , PortNo->value() , RTSP->value()  , StorageSpace->value() , "0" , StorageFileType->value() , StorageStatus->value(),StreamMode->value() ,PTZControl->value() ,RetrieverMode->value(),ReplicaMode->value() ,Status->value(), Motion->value(), Desc->value());
		if(ret){
			return true;
		}
		return false;
	}
}

bool XMLParser::UpdateChannelGroupInfo(xml_node<>* node,char *CID, int DID,int LoggedUID){
	try{
		xml_node<>* AddChannelCGroup = getNodeByName("AddChannelCGroup",node);
		if(AddChannelCGroup!=NULL){
			xml_node<>* CGID = getNodeByName("CGID",AddChannelCGroup);
			while(CGID!=NULL){
				int CCGID = -1;
				CreateUniqueCCGID(DID,CCGID);
				int ret1 = DataBaseAccessObj->InsertchannelCGroupRelation(CCGID, atoi(CID),atoi(CGID->value()),DID,LoggedUID);
				if(ret1){
					cout<<"AddChannelCgroup Success\n";
				}
				else{
					cout<<"failed to Insert the Mapping into the databases\n";
					XMLCreatorObj->NewFailureAction("Failed to insert channelcgroup mapping into the Database","103","UpdateChannel");
				}
				CGID=CGID->next_sibling("CGID");
			}
		}
		xml_node<>* RemoveChannelCGroup = getNodeByName("RemoveChannelCGroup",node);
		if(RemoveChannelCGroup!=NULL){
			xml_node<>* CGID = getNodeByName("CGID",RemoveChannelCGroup);
			while(CGID!=NULL){
				int ret2 = DataBaseAccessObj->DeleteChannelCGroupRelation(atoi(CID),atoi(CGID->value()));
				if(ret2){
					cout<<"RemoveChannelCgroup Success\n";
					//XMLCreatorObj->NewSuccessAction("AddUserUgroup","Ok");
				}
				else{
					cout<<"failed to Remove the Mapping into the databases\n";
					XMLCreatorObj->NewFailureAction("Failed to remove channelcgroup mapping into the Database","103","UpdateChannel");
				}
				CGID=CGID->next_sibling("CGID");
			}
		}
		return true;
	}
	catch (Exception e) {
		cout<<"\nError Found In Update channel Group\n";
	}
	return false;
}

bool XMLParser::SendXmlToNASUpdateChannel( int DID , char * CID, char * fileContents ){

	char deviceIP[100], receivedXMLData[100000];
	int sockFD = -1;
	int size = 200000,receivedDataSize  = 0;
	DataBaseAccessObj->GetDeviceIP(deviceIP,DID);
	if ( LookingForServer(deviceIP,55555,sockFD) ){
		if ( sendData(sizeof(int),&size, sockFD)){
			if ( sendData(size,fileContents, sockFD)){
				if ( recvData(&receivedDataSize,sizeof(int),sockFD) ){
					if ( recvData(receivedXMLData,receivedDataSize,sockFD) ){
						XMLCreatorObj->CreateXMLCreator( receivedXMLData );
						return true;
					}
				}
			}
		}
		return false;
	}

	return false;
}


bool XMLParser::SendXmlToOtherNAS( int DID , char * CID, char * fileContents ){

	char deviceIP[100], receivedXMLData[100000];
	int sockFD = -1;
	int size = 200000,receivedDataSize  = 0;
	DataBaseAccessObj->GetDeviceIP(deviceIP,DID);
	if ( LookingForServer(deviceIP,55555,sockFD) ){
		if ( sendData(sizeof(int),&size, sockFD)){
			if ( sendData(size,fileContents, sockFD)){
				if ( recvData(&receivedDataSize,sizeof(int),sockFD) ){
					if ( recvData(receivedXMLData,receivedDataSize,sockFD) ){
						cout<<"\nreceivedXMLData---->"<<receivedXMLData<<endl;
						return true;
					}
				}
			}
		}
		return false;
	}

	return false;
}

bool XMLParser::SendInactiveChannelXml(int sockFD, char *CID, char *DID){
	char xmlData[SIZEOFCHARARRAYBIGGER];
	XMLCreatorObj->CreateInactiveChannelXml(CID,DID,xmlData);
}

bool XMLParser::UpdateChannel(xml_node<>* node,char * fileContents)
{
	try{
		string nodeName;
		string attributeName;

		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		xml_node<>* DID = getNodeByName("DID",node);
		xml_node<>*CID = getNodeByName("CID",node);


		if ( DID == NULL || CID == NULL ){
			cout<<"UpdateChannel FAiled\n";
			XMLCreatorObj->NewFailureAction("The channelname you entered is NULL","154","UpdateChannel");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","UpdateChannel");
		}else{
			int sysDID = DataBaseAccessObj->GetSystemDID();
			if ( sysDID == atoi( DID->value() ) ){
				if ( DataBaseAccessObj->CheckChannelExists(CID->value(),sysDID ) ){
					if ( UpdateChannelInSameMachine( node,CID->value(),sysDID ) ){
						XMLCreatorObj->NewSuccessAction("UpdateChannel","Ok");
						RestartChannel(atoi(CID->value()));
					}
					else
						XMLCreatorObj->NewFailureAction("Unable to Update","102","UpdateChannel");
				}
				else{
					char iP[100];
					int sockFD = -1,mainDID = -1,newCID = -1;
					DataBaseAccessObj->GetDIDFromCID(mainDID,CID->value());
					DataBaseAccessObj->GetDeviceIP(iP, mainDID);
					if ( LookingForServer(iP,55555,sockFD ) ){
						if ( SendInactiveChannelXml(sockFD, CID->value(), DID->value()) ){
							AddChannelSub( node, atoi(DID->value()), newCID);
							XMLCreatorObj->NewSuccessActionWithID("UpdateChannel","ok","CID",newCID);
						}
						else{
							XMLCreatorObj->NewFailureAction("unable to send xml","102","UpdateChannel");
						}
					}
					else
					{
						XMLCreatorObj->NewFailureAction("server does not exists","102","UpdateChannel");
					}
				}
			}
			else{
				char iP[100];
				int sockFD = -1;
				DataBaseAccessObj->GetDeviceIP(iP, atoi(DID->value()));
				if ( LookingForServer(iP,55555,sockFD ) ){
					if ( SendXmlToNASUpdateChannel(atoi(DID->value()),CID->value(),fileContents) ){

					}
				}
				//if ( LookingForServer(iP,55555,sockFD ) ){
				//;
			}
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in UpdateChannel Function"<<e.what()<<endl;
	}
	return false;
}








//if ( ChannelName != NULL && IPAddress != NULL && UserName != NULL && Password != NULL && PortNo != NULL && RTSP != NULL && StorageStatus != NULL && StorageFileType != NULL && StorageSpace != NULL && RetrieverMode != NULL && Status != NULL  && PTZControl != NULL && ReplicaMode != NULL && Make != NULL && Model != NULL && Desc != NULL &&  StreamMode != NULL &&  Motion != NULL  ){
/*		cout<<"UpdateChannel FAiled\n";
			XMLCreatorObj->NewFailureAction("The channelname you entered is NULL","101","UpdateChannel");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
/*		XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","UpdateChannel");
		}else{
			int DID = DataBaseAccessObj->GetSystemDID();
			if ( DID == atoi( DIDInfo->value() ) ){
				if ( UpdateChannelInSameMachine(CID->value(),ChannelName->value(),RTSP->value(),Status->value(),RetrieverMode->value(),StorageSpace->value(),StorageFileType->value(),StorageStatus->value(),DID) ){
					if ( UpdateChannelGroupInfo(node,CID->value(),DID,LoggedUID) ){
						cout<<"\nChannel successfully updated\n";
						XMLCreatorObj->NewSuccessAction("UpdateChannel","Ok");
						RestartChannel(atoi(CID->value()));
					}
				}else{
					cout<<"\nInsert will be here\n";
					AddChannel(node, fileContents);
				}
			}
			else{
				if ( SendXmlToOtherNAS( atoi(DIDInfo->value()),CID->value(),fileContents) ){
					if ( DataBaseAccessObj->InactiveChannel(CID->value()) ){
						cout<<"\nChannel successfully updated\n";
						XMLCreatorObj->NewSuccessAction("UpdateChannel","Ok");
					}
				}
				else
					XMLCreatorObj->NewFailureAction("unable to Update","129","UpdateChannel");
			}
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in UpdateChannel Function"<<e.what()<<endl;
	}
	return false;

}*/


bool XMLParser::AddUGroup(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser
		xml_node<>* UGroupInfo = getNodeByName("UGroupInfo",node);
		xml_node<>* UGName = getNodeByName("UGNAME",UGroupInfo);

		if(UGName==NULL ){
			cout<<"AddUGroup FAiled\n";
			XMLCreatorObj->NewFailureAction("The UGName you entered is NULL","101","AddUGroup");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","AddUGroup");
		}else{
			int UGID = 0, DID = DataBaseAccessObj->GetSystemDID();
			CreateUniqueUGID(DID,UGID);
			int ret = DataBaseAccessObj->InsertUGroupInfo(UGName->value(),DID,UGID);
			if(ret){
				xml_node<>* UID = getNodeByName("UID",UGroupInfo);
				int UUGID = -1;
				while(UID!=NULL){
					CreateUniqueUUGID(DID,UUGID);
					int ret2 = DataBaseAccessObj->InsertUserUGroupRelation(UUGID, atoi(UID->value()),UGID,DID);
					if(!ret2){
						XMLCreatorObj->NewFailureAction("Fail to Insert User Ugroup Relation","103","AddUGroup");
					}
					UID=UID->next_sibling("UID");
				}
				cout<<"AddUGroup Success\n";
				XMLCreatorObj->NewSuccessActionWithID("AddUGroup","Ok","UGID",UGID);
			}else{
				cout<<"AddUGroup FAiled\n";
				XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","AddUGroup");
			}
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddUGroup Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::AddCGroup(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser
		xml_node<>* ChannelGroupInfo = getNodeByName("CGroupInfo",node);
		xml_node<>* CGName = getNodeByName("CGNAME",ChannelGroupInfo);

		if(CGName==NULL ){
			cout<<"AddCGroup FAiled\n";
			XMLCreatorObj->NewFailureAction("The CGName you entered is NULL","101","AddCGroup");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","AddCGroup");
		}else{
			int CGID = 0,DID = DataBaseAccessObj->GetSystemDID();
			CreateUniqueCGID(DID,CGID);
			int ret = DataBaseAccessObj->InsertCGroupInfo(CGID, CGName->value(),DID);
			if(ret){
				int CCGID = -1;
				xml_node<>* CID = getNodeByName("CID",ChannelGroupInfo);
				while(CID!=NULL){

					CreateUniqueCCGID(DID,CCGID);
					int ret2 = DataBaseAccessObj->InsertchannelCGroupRelation( CCGID, atoi(CID->value()),CGID,DID,LoggedUID);
					if(!ret2){
						cout<<"failed to Remove the Mapping into the databases\n";
						XMLCreatorObj->NewFailureAction("Failed to remove channelcgroup mapping into the Database","103","AddCGroup");
					}
					CID=CID->next_sibling("CID");
				}
				cout<<"AddCGroup Success\n";
				XMLCreatorObj->NewSuccessActionWithID("AddCGroup","Ok","CGID",CGID);
			}else{
				cout<<"AddCGroup FAiled\n";
				XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","AddCGroup");
			}
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddCGroup Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::AddUserUGroupRelation(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		xml_node<>* UID = getNodeByName("UID",node);
		xml_node<>* UGID = getNodeByName("UGID",node);
		if(UID==NULL  || UGID ==NULL){
			cout<<"AddUserUGroupRelation FAiled\n";
			XMLCreatorObj->NewFailureAction("The UID or UGID you entered is NULL","101","AddUserUGroupRelation");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","AddUserUGroupRelation");
		}else{
			int UUGID = -1 , DID = DataBaseAccessObj->GetSystemDID();
			CreateUniqueUUGID(DID, UUGID);
			int ret = DataBaseAccessObj->InsertUserUGroupRelation(UUGID, atoi(UID->value()),atoi(UGID->value()),DID);
			if(ret){
				cout<<"AddUserUGroupRelation Success\n";
				XMLCreatorObj->NewSuccessAction("AddUserUGroupRelation","Ok");
			}else{
				cout<<"AddUserUGroupRelation FAiled\n";
				XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","AddUserUGroupRelation");
			}
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddUserUGroupRelation Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::AddChannelCGroupRelation(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		xml_node<>* CID = getNodeByName("CID",node);
		xml_node<>* CGID = getNodeByName("CGID",node);
		xml_node<>* UID = getNodeByName("UID",node);



		if(CID==NULL  || UID==NULL || CGID ==NULL){
			cout<<"AddchannelCGroupRelation FAiled\n";
			XMLCreatorObj->NewFailureAction("The UID or UGID you entered is NULL","101","AddchannelCGroupRelation");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","AddchannelCGroupRelation");
		}else{
			int CCGID = -1 , DID = DataBaseAccessObj->GetSystemDID();
			CreateUniqueCCGID(DID,CCGID);
			int ret = DataBaseAccessObj->InsertchannelCGroupRelation(CCGID,atoi(CID->value()),atoi(CGID->value()),DID,atoi(UID->value()));
			if(ret){
				cout<<"AddchannelCGroupRelation Success\n";
				XMLCreatorObj->NewSuccessAction("AddchannelCGroupRelation","Ok");
			}else{
				cout<<"AddchannelCGroupRelation FAiled\n";
				XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","AddchannelCGroupRelation");
			}


		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddchannelCGroupRelation Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::AddUGroupCGroupRelation(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		xml_node<>* UGID = getNodeByName("UGID",node);
		xml_node<>* CGID = getNodeByName("CGID",node);
		xml_node<>* UID = getNodeByName("UID",node);


		if(UGID==NULL ||  UID ==NULL || CGID ==NULL){
			cout<<"AddUGroupCGroupRelation FAiled\n";
			XMLCreatorObj->NewFailureAction("The UGID or CGID you entered is NULL","101","AddUGroupCGroupRelation");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","AddUGroupCGroupRelation");
		}else{
			int UGCGID = - 1, DID = DataBaseAccessObj->GetSystemDID();
			CreateUniqueUGCGID(DID,UGCGID);
			int ret = DataBaseAccessObj->InsertUGroupCGroupRelation(UGCGID,atoi(UGID->value()),atoi(CGID->value()),atoi(UID->value()),DID);

			if(ret){
				cout<<"AddUGroupCGroupRelation Success\n";
				XMLCreatorObj->NewSuccessAction("AddUserUGroupChannelCGroupRelation","Ok");
			}else{
				cout<<"AddUGroupCGroupRelation FAiled\n";
				XMLCreatorObj->NewFailureAction("Failed to insert into the Database","103","AddUGroupCGroupRelation");
			}
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in AddUGroupCGroupRelation Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::Login(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Login method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		xml_node<>* username = getNodeByName("UserName",node);
		xml_node<>* password = getNodeByName("Password",node);

		if(username==NULL || password==NULL ){
			cout<<"Login Failed\n";
			XMLCreatorObj->NewFailureAction("The username or password you entered is NULL","101","Login");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","Login");
		}else{
			int UID = -1;
			bool LoginSuccess = DataBaseAccessObj->AuthenticateUser(username->value(),password->value(),&UID);
			if(LoginSuccess){
				XMLCreatorObj->NewLoginSuccess(UID,clientIp, isAndroid);
				LoggedUID = UID;
			}
			else{
				cout<<"Login Failed\n";
				XMLCreatorObj->NewFailureAction("The username and password you entered don't match.","103","Login");
			}
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in Login Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::FetchAllUser(xml_node<>* node){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser
		if(0){
			cout<<"FetchAllUser FAiled\n";
			XMLCreatorObj->NewFailureAction("The you entered is NULL","101","FetchAllUser");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchAllUser");
		}else{
			XMLCreatorObj->NewFetchAllUser(true);
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in FetchChaFetchAllUsernnel Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::FetchAllUserUGroupRelation(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		if(0){
			cout<<"FetchAllUser FAiled\n";
			XMLCreatorObj->NewFailureAction("The you entered is NULL","101","FetchAllUser");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchAllUser");
		}else{
			XMLCreatorObj->NewFetchAllUserUGroupRelation(true);
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in FetchChaFetchAllUsernnel Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::FetchAllUGroup(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		if(0){
			cout<<"FetchAllUGroup FAiled\n";
			XMLCreatorObj->NewFailureAction("The you entered is NULL","101","FetchAllUGroup");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchAllUGroup");
		}else{

			XMLCreatorObj->NewFetchAllUGroup(true);
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in FetchAllUGroup Function"<<e.what()<<endl;
	}
	return false;
}


bool XMLParser::FetchAllChannel(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		if(0){
			cout<<"FetchAllChannel FAiled\n";
			XMLCreatorObj->NewFailureAction("The you entered is NULL","101","FetchAllChannel");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchAllChannel");
		}else{

			XMLCreatorObj->NewFetchAllChannel(true);
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in FetchAllChannel Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::FetchAllCGroup(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		if(0){
			cout<<"FetchAllUGroup FAiled\n";
			XMLCreatorObj->NewFailureAction("The you entered is NULL","101","FetchAllCGroup");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchAllCGroup");
		}else{

			XMLCreatorObj->NewFetchAllCGroup(true);
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in FetchAllCGroup Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::FetchAllChannelCGroupRelation(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		if(0){
			cout<<"FetchAllChannelCGroupRelation FAiled\n";
			XMLCreatorObj->NewFailureAction("The you entered is NULL","101","FetchAllChannelCGroupRelation");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchAllChannelCGroupRelation");
		}else{
			XMLCreatorObj->NewFetchAllChannelCGroupRelation(true);
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in FetchAllChannelCGroupRelation Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::FetchAllDevice(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		if(0){
			cout<<"FetchAllDevice FAiled\n";
			XMLCreatorObj->NewFailureAction("The you entered is NULL","101","FetchAllDevice");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchAllDevice");
		}else{

			XMLCreatorObj->NewFetchAllDevice();
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in FetchAllUGroup Function"<<e.what()<<endl;
	}
	return false;

}


bool XMLParser::FetchAllVideo(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser
		xml_node<>* CID = getNodeByName("CID",node);
		xml_node<>* DID = getNodeByName("DID",node);
		xml_node<>* StartTime = getNodeByName("StartTime",node);
		xml_node<>* EndTime = getNodeByName("EndTime",node);


		if(CID==NULL || DID==NULL || StartTime==NULL || EndTime==NULL){
			cout<<"FetchAllVideo FAiled\n";
			XMLCreatorObj->NewFailureAction("The you entered is NULL","101","FetchAllVideo");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchAllVideo");
		}else{

			vector<string> VName;
			vector<int> VID;
			vector<string> VideoStartTime;
			vector<string> VideoEndTime;
			DataBaseAccessObj->GetVIDForChannelAccordingTime(VName,VID,VideoStartTime,VideoEndTime,atoi(CID->value()),atoi(DID->value()),StartTime->value(),EndTime->value());
			if(VName.size()>0){
				XMLCreatorObj->NewSuccessFetchAllVideoAction(VName,VID,VideoStartTime,VideoEndTime);
			}
			else
				XMLCreatorObj->NewFailureAction("No Video Found","105","FetchAllVideo");
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in FetchAllUGroup Function"<<e.what()<<endl;
	}
	return false;

}
bool XMLParser::StartVOD(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the StartVOD method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		xml_node<>* CID = getNodeByName("CID",node);
		xml_node<>* DID = getNodeByName("DID",node);
		xml_node<>* StartTime = getNodeByName("StartTime",node);
		xml_node<>* EndTime = getNodeByName("EndTime",node);
		xml_node<>* MountURL = getNodeByName("MountURL",node);


		if(CID==NULL || DID==NULL || StartTime==NULL || EndTime==NULL || MountURL==NULL ){
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","StartVOD");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","StartVOD");
		}else{
			int UID = -1;
			vector<int> VID;
			vector<string> VName;
			vector<string> VideoStartTime;
			vector<string> VideoEndTime;

			int ret = DataBaseAccessObj->GetVIDForChannelAccordingTime(VName,VID,VideoStartTime,VideoEndTime,atoi(CID->value()),atoi(DID->value()),StartTime->value(),EndTime->value());
			if(ret){
				if(VName.size()>0){
					VODObj->AddVideoFiles(MountURL->value(),VName,VID);
					XMLCreatorObj->NewSuccessVODAction(VName,VID,VideoStartTime,VideoEndTime);
				}
				else
					XMLCreatorObj->NewFailureAction("No Video Found","105","StartVOD");
			}else{
				XMLCreatorObj->NewFailureAction("Failed to Fetch Details From the Database","111","StartVOD");
			}
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in StartVOD Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::StartDumping(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the StartDumping method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		xml_node<>* CID = getNodeByName("CID",node);
		xml_node<>* DID = getNodeByName("DID",node);
		if(CID==NULL || DID==NULL ){
			cout<<"StartDumping Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","StartDumping");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","StartDumping");
		}else{
			RetrieveObj->StartDumping(atoi(CID->value()),atoi(DID->value()));
			DataBaseAccessObj->UpdateDumpingStatus(CID->value(),DID->value(),1);
			XMLCreatorObj->NewSuccessAction("StartDumping","Dumping Successfully Started");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in StartDumping Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::StopDumping(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the StopDumping method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		xml_node<>* CID = getNodeByName("CID",node);
		xml_node<>* DID = getNodeByName("DID",node);

		if( CID == NULL || DID == NULL ){

			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","StopDumping");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","StopDumping");
		}else{
			RetrieveObj->StopDumping( atoi(CID->value()), atoi(DID->value()) );
			DataBaseAccessObj->UpdateDumpingStatus( CID->value(), DID->value(), 0);
			XMLCreatorObj->NewSuccessAction("StopDumping","Dumping Successfully Stopped");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in StopDumping Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::StartDownload(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the StartDumping method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"StartDumping Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","StartDownload");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","StartDownload");
		}else{
			xml_node<>* VideoInfo = node->first_node("VideoInfo");

			vector<string> VName;
			vector<string> VID;

			while(VideoInfo!=NULL){
				xml_node<>* VIDnode = getNodeByName("VID",VideoInfo);
				xml_node<>* VNamenode = getNodeByName("VName",VideoInfo);

				if(VIDnode!=NULL && VNamenode!=NULL){
					VName.push_back((const char*)VNamenode->value());
					VID.push_back((const char* )VIDnode->value());
				}
				VideoInfo= VideoInfo->next_sibling("VideoInfo");
			}
			char portNo[10000];
			Download * downloadObj = new Download();
			downloadObj->StartDownloadThread(downloadObj,VName);

			int port= downloadObj->getPort();
			sprintf(portNo,"%d",port);
			XMLCreatorObj->NewSuccessStartDownloadAction(portNo);

		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in StartDownload Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::ExtractUserInfo(xml_node<>*node){
	return false;
}
bool XMLParser::ExtractUserGroupInfo(xml_node<>*node){
	return false;
}
bool XMLParser::ExtractChannelInfo(xml_node<>*node){
	return false;
}
bool XMLParser::ExtractChannelGroupInfo(xml_node<>*node){
	return false;
}
bool XMLParser::ExtractChannelCGroupInfo(xml_node<>*node){
	return false;
}
bool XMLParser::ExtractUserUGroupInfo(xml_node<>*node){
	return false;
}


bool XMLParser::ParseInteractionResponse(xml_node<>*node){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Interaction method");
		}
		nodeName = node->name();
		if(nodeName!="Response"){
			throw runtime_error("wrong Node passed as argument to Response Method");
		}
		// Needs to cross check if the  attribute name has value Login
		if(0){
			cout<<"Interaction Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","Interaction");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","Interaction");
		}else{

			if( ExtractUserInfo(node) ) {
				if ( ExtractUserGroupInfo(node) ){
					if(ExtractChannelInfo(node) ){
						if(ExtractChannelGroupInfo(node)){
							if(ExtractChannelCGroupInfo(node)){
								if(ExtractUserUGroupInfo(node)){
									XMLCreatorObj->NewSuccessInteraction();
								}
							}
						}
					}
				}
			}
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","Interaction");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in StartDownload Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::Response(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			cout<<"empty node passed into the Action method\n";
			return false;
		}
		nodeName = node->name();
		if(nodeName!="Response"){
			cout<<"wrong Node:  "<<nodeName<<" passed as argument to Action Method\n";
			return false;
		}
		xml_attribute<>* attribute = node->first_attribute();

		/*	while(attribute){
			attributeName=attribute->name();
		/*	if(attributeName=="ActionName"){
				string attributeValue = attribute->value();
				if(attributeValue == "Interaction"){
					cout<<"An Interaction Request has been made\n";
					ParseInteractionResponse(node);
				}
			}*/
		//attribute=attribute->next_attribute();
		/*}*/
		return true;
	}catch (exception &e){
		cout<<"Exception caught in Action Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::InteractionResponse(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			cout<<"empty node passed into the Action method\n";
			return false;
		}
		nodeName = node->name();
		if(nodeName!="InteractionResponse"){
			cout<<"wrong Node:  "<<nodeName<<" passed as argument to Action Method\n";
			return false;
		}
		xml_attribute<>* attribute = node->first_attribute();

		while(attribute){
			attributeName=attribute->name();
			if(attributeName=="ActionName"){
				string attributeValue = attribute->value();
				if(attributeValue == "DeviceInfo"){
					cout<<"An DeviceInfo InteractionResponse Request has been made\n";
					// DeviceInfoInteraction(node);
				}else if(attributeValue == "FetchAllUser"){
					cout<<"An FetchAllUser InteractionResponse Request has been made\n";
					//FetchAllUserInteraction(node);
				}else if (attributeValue == "FetchAllUGroup"){
					cout<<"An FetchAllUser InteractionResponse Request has been made\n";
					//FetchAllUGroupInteraction(node);
				}else if (attributeValue == "FetchAllChannel"){
					cout<<"An FetchAllUser InteractionResponse Request has been made\n";
					//FetchAllChannelInteraction(node);
				}else if (attributeValue == "FetchAllCGroup"){
					cout<<"An FetchAllCGroup InteractionResponse Request has been made\n";
					//FetchAllCGroupInteraction(node);
				}else if (attributeValue == "FetchAllUserUGroupRelation"){
					cout<<"An FetchAllUserUGroupRelation InteractionResponse Request has been made\n";
					//FetchAllUserUGroupRelationInteraction(node);
				}else if (attributeValue == "FetchAllChannelCGroupRelation"){
					cout<<"An FetchAllChannelCGroupRelation InteractionResponse Request has been made\n";
					//FetchAllChannelCGroupRelationInteraction(node);
				}else{
					cout<<"\nInteraction request does not exist for this NAS\n";
				}
				attribute=attribute->next_attribute();
			}
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in Action Function"<<e.what()<<endl;
	}
	return false;
}



bool XMLParser::StartInteraction(xml_node<>* node){
	try{
		string nodeName;
			if(!node){
				throw runtime_error("empty node passed into the Response method");
			}
			nodeName = node->name();
			if(nodeName!="Interaction"){
				throw runtime_error("wrong Node passed as argument to Response Method");
			}

			if(0 ){
				cout<<"StartInteraction FAiled\n";
				XMLCreatorObj->NewFailureAction("StartInteraction is NULL","101","StartInteraction");
			}else if(0){
				XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","StartInteraction");
			}else{
				XMLCreatorObj->CreateInteractionXml();
				string xmlData = XMLCreatorObj->NewconvertToString();
				XMLCreatorObj->ReInitializeXML();
				XMLCreatorObj->NewSuccessAction("StartInteraction","ok");

			}
			return true;
		}
		catch (exception &e){
			cout<<"Exception caught in DeviceInfoInteraction Function"<<e.what()<<endl;
		}
		return false;

}

bool XMLParser::RtspInfo(xml_node<>* node){
	try{
			string nodeName;
			string attributeName;
			if(!node){
				throw runtime_error("empty node passed into the Response method");
			}
			nodeName = node->name();
			if(nodeName!="Interaction"){
				throw runtime_error("wrong Node passed as argument to Response Method");
			}

			if(0 ){
				cout<<"RtspInfo FAiled\n";
				XMLCreatorObj->NewFailureAction("RtspInfo is NULL","101","RtspInfo");
			}else if(0){
				XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","RtspInfo");
			}else{
				int sysDID = DataBaseAccessObj->GetSystemDID();
				for (;node && (node != NULL);node=node->next_sibling("RtspInfo") ){
					xml_node<>* DID = getNodeByName("DID",node);
					xml_node<>* CID = getNodeByName("CID",node);
					xml_node<>* rtsp = getNodeByName("Rtsp",node);
					if ( DID != NULL && CID != NULL && rtsp != NULL  ){
						if ( atoi(DID->value()) != sysDID ){
							int ret = DataBaseAccessObj->CheckAlreadyExist(CID->value(),"rtspinfo","CID");
							int result = 0;
							if ( ! ret ){
								result = DataBaseAccessObj->InsertCreatedRtsp(atoi(CID->value()),atoi(DID->value()),rtsp->value());
								cout<<"\nInsert Value---->"<<rtsp->value();
							}
							else{
								result = DataBaseAccessObj->UpdateRtspInfo(CID->value(),DID->value(),rtsp->value());
							}
							if ( result )
								XMLCreatorObj->InteractionSuccessAction("RtspInfo", "ok");
							else
								XMLCreatorObj->InteractionFailureAction("Unable To Process","122","RtspInfo");//("DeviceInfo", "ok");
						}
					}
				}
			}
			return true;
		}
		catch (exception &e){
			cout<<"Exception caught in DeviceInfoInteraction Function"<<e.what()<<endl;
		}
		return false;

}
//(node);

bool XMLParser::DeviceInfoInteraction(xml_node<>* node){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Response method");
		}
		nodeName = node->name();
		if(nodeName!="Interaction"){
			throw runtime_error("wrong Node passed as argument to Response Method");
		}
		// Needs to cross check if the  attribute name has value AddUser
		xml_node<>* DeviceInfo = node->first_node("DeviceInfo");
		if(DeviceInfo==NULL ){
			cout<<"FetchAllUserInteraction FAiled\n";
			XMLCreatorObj->NewFailureAction("UserInfo is NULL","101","DeviceInfo");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","DeviceInfo");
		}else{
			int sysDID = DataBaseAccessObj->GetSystemDID();
			for (;DeviceInfo && (DeviceInfo != NULL);DeviceInfo=DeviceInfo->next_sibling("DeviceInfo") ){
				xml_node<>* DID = getNodeByName("DID",DeviceInfo);
				xml_node<>* DeviceIp = getNodeByName("DEVICEIP",DeviceInfo);
				xml_node<>* DeviceName = getNodeByName("DEVICENAME",DeviceInfo);
				xml_node<>* macAddress = getNodeByName("MACADDRESS",DeviceInfo);
				xml_node<>* Status = getNodeByName("STATUS",DeviceInfo);
				if ( DID != NULL && DeviceIp != NULL && DeviceName != NULL && macAddress != NULL && Status != NULL ){
					if ( atoi(DID->value()) != sysDID ){
						int ret = DataBaseAccessObj->CheckAlreadyExist(macAddress->value(),"deviceinfo","MACADDRESS");
						int result = 0;
						if ( ! ret ){
							result = DataBaseAccessObj->InsertDeviceInfo(atoi(DID->value()),DeviceName->value(),DeviceIp->value(),atoi(Status->value()),0,macAddress->value());
							cout<<"\nInsert Value---->"<<DeviceIp->value();
						}
						else{
							cout<<"\nUpdate Value---->"<<DeviceIp->value();
							cout<<"\n DeviceIp"<<DeviceIp->value()<<endl;
							cout<<"\nmacAddress"<<macAddress->value()<<endl;
							cout<<"\nDID"<<DID->value()<<endl;
							cout<<"\nDID"<<DID->value()<<endl;
							result = DataBaseAccessObj->UpdateDeviceInfo(DID->value(),DeviceName->value(),DeviceIp->value(),Status->value());
						}
						if ( result )
							XMLCreatorObj->InteractionSuccessAction("DeviceInfo", "ok");
						else
							XMLCreatorObj->InteractionFailureAction("Unable To Process","122","DeviceInfo");//("DeviceInfo", "ok");
					}
				}
			}
		}
		return true;
	}
	catch (exception &e){
		cout<<"Exception caught in DeviceInfoInteraction Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::FetchAllUserInteraction(xml_node<>* node){

	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Response method");
		}
		nodeName = node->name();
		if(nodeName!="Interaction"){
			throw runtime_error("wrong Node passed as argument to Response Method");
		}
		// Needs to cross check if the  attribute name has value AddUser
		xml_node<>* UserInfo = node->first_node("UserInfo");
		if(UserInfo==NULL ){
			cout<<"FetchAllUserInteraction FAiled\n";
			XMLCreatorObj->InteractionFailureAction("UserInfo is NULL","101","FetchAllUserInteraction");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->InteractionFailureAction("Permission denied to perform this Action","102","FetchAllUserInteraction");
		}else{
			int sysDID = DataBaseAccessObj->GetSystemDID();


			for (;UserInfo && (UserInfo != NULL);UserInfo=UserInfo->next_sibling("UserInfo") ){
				xml_node<>* UID = getNodeByName("UID",UserInfo);
				xml_node<>* UName = getNodeByName("UName",UserInfo);
				xml_node<>* UserName = getNodeByName("UserName",UserInfo);
				xml_node<>* Password = getNodeByName("Password",UserInfo);
				xml_node<>* UserRole = getNodeByName("UserRole",UserInfo);
				xml_node<>* Email = getNodeByName("Email",UserInfo);
				xml_node<>* ContactNo = getNodeByName("ContactNo",UserInfo);
				xml_node<>* Status = getNodeByName("Status",UserInfo);
				xml_node<>* DID = getNodeByName("DID",UserInfo);

				if ( DID != NULL && UID != NULL && UName != NULL && UserName != NULL && Password != NULL && UserRole != NULL && Email != NULL && ContactNo != NULL && Status != NULL ){
					if ( atoi(DID->value()) != sysDID ){
						int ret = DataBaseAccessObj->CheckUserInfo(UserName->value(),UID->value());
						int result = 0;
						if ( ! ret )
							result = DataBaseAccessObj->InsertUserInfo(atoi(UID->value()),UName->value(),UserName->value(),Password->value(),atoi(UserRole->value()),Email->value(),ContactNo->value(),atoi(DID->value()),atoi(Status->value()));
						else{
							char uiDInfo[100];
							sprintf( uiDInfo,"%d",ret);
							result = DataBaseAccessObj->UpdateUserInfo(UID->value(), UName->value(),UserName->value(),Password->value(),UserRole->value(),Email->value(),ContactNo->value(),Status->value());
						}
						if ( result )
							XMLCreatorObj->InteractionSuccessAction("FetchAllUser", "ok");
						else
							XMLCreatorObj->InteractionFailureAction("Unable To Process","122","FetchAllUser");//("DeviceInfo", "ok");
					}
				}
			}
		}
		return true;
	}
	catch (exception &e){
		cout<<"Exception caught in FetchAllUserInteraction Function"<<e.what()<<endl;
	}
	return false;

}
bool XMLParser::FetchAllCGroupInteraction(xml_node<>* node){

	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Response method");
		}
		nodeName = node->name();
		if(nodeName!="Interaction"){
			throw runtime_error("wrong Node passed as argument to Response Method");
		}
		// Needs to cross check if the  attribute name has value AddUser
		xml_node<> * cGroupInfo = getNodeByName( "CGroupInfo", node);
		if(cGroupInfo==NULL ){
			cout<<"FetchAllUGroupIneraction FAiled\n";
			XMLCreatorObj->InteractionFailureAction("uGroupInfo is NULL","101","FetchAllCGroupIneraction");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->InteractionFailureAction("Permission denied to perform this Action","102","FetchAllCGroupIneraction");
		}else{
			int sysDID = DataBaseAccessObj->GetSystemDID();
			int result = 0,ret = 0;

			for ( ; cGroupInfo && (cGroupInfo != NULL); cGroupInfo = cGroupInfo->next_sibling("CGroupInfo") )
			{
				xml_node<> * CGID=  getNodeByName("CGID", cGroupInfo);
				xml_node<> * CGName =  getNodeByName("CGName", cGroupInfo);
				xml_node<> * DID =  getNodeByName("DID", cGroupInfo);

				if ( CGID != NULL && CGName != NULL && DID != NULL ){
					if ( atoi(DID->value()) != sysDID ){
						ret = DataBaseAccessObj->CheckAlreadyExist(CGName->value(),"cgroupinfo","CGNAME");
						if( !ret ){
							result = DataBaseAccessObj->InsertCGroupInfo(atoi(CGID->value()),CGName->value(),atoi(DID->value() ));
						}
						else{
							result = DataBaseAccessObj->UpdateCGroupInfo(atoi(CGID->value()), CGName->value(),atoi(DID->value()));
						}
						if ( result )
							XMLCreatorObj->InteractionSuccessAction("FetchAllCGroup", "ok");
						else
							XMLCreatorObj->InteractionFailureAction("Unable To Process","122","FetchAllCGroup");//("DeviceInfo", "ok");
					}
				}
			}
		}
		return true;
	}
	catch (exception &e){
		cout<<"Exception caught in FetchAllCGroupInteraction Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::FetchAllUGroupInteraction(xml_node<>* node){

	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Response method");
		}
		nodeName = node->name();
		if(nodeName!="Interaction"){
			throw runtime_error("wrong Node passed as argument to Response Method");
		}
		// Needs to cross check if the  attribute name has value AddUser
		xml_node<> * uGroupInfo = getNodeByName( "UGroupInfo", node);
		if(uGroupInfo==NULL ){
			cout<<"FetchAllUGroupIneraction FAiled\n";
			XMLCreatorObj->InteractionFailureAction("uGroupInfo is NULL","101","FetchAllUGroupIneraction");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->InteractionFailureAction("Permission denied to perform this Action","102","FetchAllUGroupIneraction");
		}else{
			int result = 0, sysDID = DataBaseAccessObj->GetSystemDID();


			for ( ; uGroupInfo && (uGroupInfo != NULL); uGroupInfo = uGroupInfo->next_sibling("UGroupInfo") )
			{
				xml_node<> * UGID=  getNodeByName("UGID", uGroupInfo);
				xml_node<> * UGName =  getNodeByName("UGName", uGroupInfo);
				xml_node<> * DID =  getNodeByName("DID", uGroupInfo);
				if ( UGID != NULL && UGName != NULL && DID != NULL ){
					if ( atoi(DID->value()) != sysDID ){
						int ret = DataBaseAccessObj->CheckAlreadyExist(UGName->value(),"ugroupinfo","UGNAME");
						if( !ret ){
							result =DataBaseAccessObj->InsertUGroupInfo(UGName->value(),atoi(DID->value()),atoi(UGID->value()));
						}
						else{
							result = DataBaseAccessObj->UpdateUGroupInfo(atoi(UGID->value()), UGName->value(),atoi(DID->value()));
						}
						if ( result )
							XMLCreatorObj->InteractionSuccessAction("FetchAllUGroup", "ok");
						else
							XMLCreatorObj->InteractionFailureAction("Unable To Process","122","FetchAllUGroup");//("DeviceInfo", "ok");
					}
				}
			}
		}
		return true;
	}
	catch (exception &e){
		cout<<"Exception caught in FetchAllUGroupInteraction Function"<<e.what()<<endl;
	}
	return false;

}


bool XMLParser::FetchAllChannelInteraction(xml_node<>* node){

	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Response method");
		}
		nodeName = node->name();
		if(nodeName!="Interaction"){
			throw runtime_error("wrong Node passed as argument to Response Method");
		}
		// Needs to cross check if the  attribute name has value AddUser
		xml_node<> * channelInfo = getNodeByName( "ChannelInfo", node);
		if(channelInfo==NULL ){
			cout<<"FetchAllChannelInteraction FAiled\n";
			XMLCreatorObj->InteractionFailureAction("ChannelInfo is NULL","101","FetchAllChannelInteraction");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->InteractionFailureAction("Permission denied to perform this Action","102","FetchAllChannelInteraction");
		}else{
			int sysDID = DataBaseAccessObj->GetSystemDID();
			for ( ; channelInfo && (channelInfo != NULL); channelInfo = channelInfo->next_sibling("ChannelInfo") )
			{
				xml_node<>* CID = getNodeByName("CID",channelInfo);
				xml_node<>* ChannelName= getNodeByName("ChannelName",channelInfo);
				xml_node<>* RTSP= getNodeByName("RTSP",channelInfo);
				xml_node<>* Status= getNodeByName("Status",channelInfo);
				xml_node<>* DID= getNodeByName("DID",channelInfo);
				xml_node<>* IPAddress= getNodeByName("IPAddress",channelInfo);
				xml_node<>* UserName= getNodeByName("UserName",channelInfo);
				xml_node<>* Password= getNodeByName("Password",channelInfo);
				xml_node<>* PortNo= getNodeByName("PortNo",channelInfo);
				xml_node<>* StorageStatus= getNodeByName("StorageStatus",channelInfo);
				xml_node<>* RetrieverMode= getNodeByName("RetrieverMode",channelInfo);
				xml_node<> * ReplicaMode = getNodeByName("ReplicaMode",channelInfo);
				xml_node<>* StoragePath = getNodeByName("StoragePath",channelInfo);
				xml_node<>* StorageFileType= getNodeByName("StorageFileType",channelInfo);
				xml_node<>* StorageSpace= getNodeByName("StorageSpace",channelInfo);
				xml_node<>* StorageSpaceUsed= getNodeByName("StorageSpaceUsed",channelInfo);
				xml_node<>* StreamMode= getNodeByName("StreamMode",channelInfo);
				xml_node<> * PTZControl = getNodeByName("PTZ",channelInfo);
				xml_node<>* Make= getNodeByName("Make",channelInfo);
				xml_node<>* Model= getNodeByName("Model",channelInfo);
				xml_node<>* Desc= getNodeByName("Decs",channelInfo);
				xml_node<>* Motion= getNodeByName("Motion",channelInfo);

				if ( StorageSpaceUsed != NULL && CID != NULL && ChannelName != NULL && IPAddress != NULL && UserName != NULL && Password != NULL && PortNo != NULL && RTSP != NULL && StorageStatus != NULL && StorageFileType != NULL && StoragePath != NULL && StorageSpace != NULL && RetrieverMode != NULL && Status != NULL  && PTZControl != NULL && ReplicaMode != NULL && Make != NULL && Model != NULL && Desc != NULL &&  StreamMode != NULL &&  Motion != NULL  ){
					if ( sysDID != atoi(DID->value())){
					int result = 0,ret = DataBaseAccessObj->CheckAlreadyExist(CID->value(),"channelinfo","CID");
					if( !ret )
						result =DataBaseAccessObj->NewInsertChannelInfo(atoi(CID->value()) , ChannelName->value() ,atoi( DID->value()) , Make->value() , Model->value() , UserName->value() , Password->value() , IPAddress->value() , PortNo->value() , RTSP->value() , StoragePath->value() , StorageSpace->value() , StorageSpaceUsed->value() , StorageFileType->value() , StorageStatus->value(),StreamMode->value() ,PTZControl->value() ,RetrieverMode->value(),ReplicaMode->value() ,Status->value(), Motion->value(), Desc->value());
					else
						result = DataBaseAccessObj->UpdateChannelInfo(atoi(DID->value()), CID->value() , ChannelName->value() , Make->value() , Model->value() , UserName->value() , Password->value() , IPAddress->value() , PortNo->value() , RTSP->value()  , StorageSpace->value() , StorageSpaceUsed->value() , StorageFileType->value() , StorageStatus->value(),StreamMode->value() ,PTZControl->value() ,RetrieverMode->value(),ReplicaMode->value() ,Status->value(), Motion->value(), Desc->value());
					if ( result )
						XMLCreatorObj->InteractionSuccessAction("FetchAllChannelInteraction", "ok");
					else
						XMLCreatorObj->InteractionFailureAction("Unable To Process","122","FetchAllChannelInteraction");//("DeviceInfo", "ok");
				}
				}
				else
					XMLCreatorObj->InteractionFailureAction("Unable To Process","1402","FetchAllChannelInteraction");//("DeviceInfo", "ok");
			}
		}
		return true;
	}
	catch (exception &e){
		cout<<"Exception caught in FetchAllChannelInteraction Function"<<e.what()<<endl;
	}
	return false;

}



bool XMLParser::FetchAllUGroupCGroupRelation(xml_node<>* node){

	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Interaction method");
		}
		nodeName = node->name();
		if(nodeName!="Interaction"){
			throw runtime_error("wrong Node passed as argument to Interaction Method");
		}

		xml_node<> * UGroupCGroupInfo = getNodeByName( "UGroupCGroupInfo", node);
		if(UGroupCGroupInfo==NULL ){
			cout<<"FetchAllUGroupCGroupRelation FAiled\n";
			XMLCreatorObj->InteractionFailureAction("ChannelCGroupInfo is NULL","101","FetchAllUGroupCGroupRelation");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->InteractionFailureAction("Permission denied to perform this Action","102","FetchAllUGroupCGroupRelation");
		}else{
			int sysDID = DataBaseAccessObj->GetSystemDID();

			for ( ; UGroupCGroupInfo && (UGroupCGroupInfo != NULL); UGroupCGroupInfo = UGroupCGroupInfo->next_sibling("UGroupCGroupInfo") )
			{

				xml_node<>* UGCGID = getNodeByName("UGCGID",UGroupCGroupInfo);
				xml_node<>* UGID = getNodeByName("UGID",UGroupCGroupInfo);
				xml_node<>* CGID = getNodeByName("CGID",UGroupCGroupInfo);
				xml_node<>* UID = getNodeByName("UID",UGroupCGroupInfo);
				xml_node<>* DID = getNodeByName("DID",UGroupCGroupInfo);

				if ( DID != NULL && UGID != NULL && CGID != NULL && UID != NULL && CGID != NULL ){
					if ( atoi(DID->value()) != sysDID ){
						int result = 0,ret = DataBaseAccessObj->CheckUGroupCGroupInfoExist( UGID->value() , CGID->value(),atoi(DID->value()),atoi(UID->value()) );
						cout<<"\nret---"<<ret<<endl;
						if( !ret ){
							result = DataBaseAccessObj->InsertUGroupCGroupRelation(atoi(UGCGID->value()),atoi(UGID->value()),atoi(CGID->value()),atoi(UID->value()),atoi(DID->value()));
							cout<<"\nInsert Result---"<<result<<endl;
						}
						else{
							result = DataBaseAccessObj->UpdateUGroupCGroupRelation(ret,UGID->value(),CGID->value(),atoi(DID->value()),UID->value());
							cout<<"\nUpdate Result----"<<result<<endl;
						}
						if ( result )
							XMLCreatorObj->InteractionSuccessAction("FetchAllUGroupCGroupRelation", "ok");
						else
							XMLCreatorObj->InteractionFailureAction("Unable To Process","122","FetchAllUGroupCGroupRelation");//("DeviceInfo", "ok");
					}
				}
			}
		}
		return true;
	}
	catch (exception &e){
		cout<<"Exception caught in FetchAllChannelCGroupRelationInteraction Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::FetchAllChannelCGroupRelationInteraction(xml_node<>* node){

	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Interaction method");
		}
		nodeName = node->name();
		if(nodeName!="Interaction"){
			throw runtime_error("wrong Node passed as argument to Interaction Method");
		}
		xml_node<> * ChannelCGroupInfo = getNodeByName( "ChannelCGroupInfo", node);
		if(ChannelCGroupInfo==NULL ){
			cout<<"FetchAllChannelInteraction FAiled\n";
			XMLCreatorObj->InteractionFailureAction("ChannelCGroupInfo is NULL","101","FetchAllChannelCGroupRelationInteraction");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->InteractionFailureAction("Permission denied to perform this Action","102","FetchAllChannelCGroupRelationInteraction");
		}else{
			int sysDID = DataBaseAccessObj->GetSystemDID();

			for ( ; ChannelCGroupInfo && (ChannelCGroupInfo != NULL); ChannelCGroupInfo = ChannelCGroupInfo->next_sibling("ChannelCGroupInfo") ){
				xml_node<>* CCGID = getNodeByName("CCGID",ChannelCGroupInfo);
				xml_node<>* CID = getNodeByName("CID",ChannelCGroupInfo);
				xml_node<>* CGID = getNodeByName("CGID",ChannelCGroupInfo);
				xml_node<>* CGName = getNodeByName("CGName",ChannelCGroupInfo);
				xml_node<>* UID = getNodeByName("UID",ChannelCGroupInfo);
				xml_node<>* DID = getNodeByName("DID",ChannelCGroupInfo);

				if ( CCGID != NULL && DID!= NULL && CID != NULL &&CGID != NULL && CGName != NULL && UID != NULL ){// && Status!= NULL && RetrieverMode !=NULL && ChannelMode != NULL && StorageFileType != NULL && StorageStatus != NULL && ReStream != NULL&& StoragePath != NULL&& StorageSpace != NULL&& DID != NULL&& StorageSpaceUsed != NULL){
					if ( atoi(DID->value()) != sysDID ){
						int result = 0,ret = DataBaseAccessObj->CheckChannelCGroupInfoExist( CGID->value() , CID->value(), UID->value(),atoi( DID->value()) );
						if( !ret ){
							result = DataBaseAccessObj->InsertchannelCGroupRelation(atoi(CCGID->value()),atoi(CID->value()),atoi(CGID->value()),atoi( DID->value()) ,atoi(UID->value()));
						}
						else{
							result = DataBaseAccessObj->UpdateChannelCGroupRelation(ret , CID->value(),CGID->value(),atoi( DID->value()) ,UID->value());
						}
						if ( result )
							XMLCreatorObj->InteractionSuccessAction("FetchAllChannelCGroupRelation", "ok");
						else
							XMLCreatorObj->InteractionFailureAction("Unable To Process","122","FetchAllChannelCGroupRelation");//("DeviceInfo", "ok");
					}
				}
			}
		}
		return true;
	}
	catch (exception &e){
		cout<<"Exception caught in FetchAllChannelCGroupRelationInteraction Function"<<e.what()<<endl;
	}
	return false;

}
bool XMLParser::FetchAllUserUGroupRelationInteraction(xml_node<>* node){

	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Interaction method");
		}
		nodeName = node->name();
		if(nodeName!="Interaction"){
			throw runtime_error("wrong Node passed as argument to Interaction Method");
		}
		xml_node<> * UserUGroupInfo = getNodeByName( "UserUGroupInfo", node);
		if( UserUGroupInfo == NULL ){
			cout<<"FetchAllUserUGroupRelationInteraction FAiled\n";
			XMLCreatorObj->InteractionFailureAction("UserUGroupInfo is NULL","101","FetchAllUserUGroupRelationInteraction");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->InteractionFailureAction("Permission denied to perform this Action","102","FetchAllUserUGroupRelationInteraction");
		}else{
			int sysDID = DataBaseAccessObj->GetSystemDID();
			int result = 0,ret = 0;
			for ( ; UserUGroupInfo && (UserUGroupInfo != NULL); UserUGroupInfo = UserUGroupInfo->next_sibling("UserUGroupInfo") )
			{
				xml_node<>* UUGID = getNodeByName("UUGID",UserUGroupInfo);
				xml_node<>* UID = getNodeByName("UID",UserUGroupInfo);
				xml_node<>* UGID = getNodeByName("UGID",UserUGroupInfo);
				xml_node<>* UGName = getNodeByName("UGName",UserUGroupInfo);
				xml_node<>* DID = getNodeByName("DID",UserUGroupInfo);

				if ( DID != NULL && UID != NULL &&UGID != NULL && UGName != NULL && UUGID != NULL){// && Status!= NULL && RetrieverMode !=NULL && ChannelMode != NULL && StorageFileType != NULL && StorageStatus != NULL && ReStream != NULL&& StoragePath != NULL&& StorageSpace != NULL&& DID != NULL&& StorageSpaceUsed != NULL){
					if ( atoi( DID->value() )  != sysDID ){
						ret = DataBaseAccessObj->CheckUserUGroupInfoExist( UGID->value() ,UID->value(),atoi(DID->value()) );
						if( !ret ){
							result = DataBaseAccessObj->InsertUserUGroupRelation(atoi(UUGID->value()),atoi(UID->value()),atoi(UGID->value()),atoi(DID->value()));
						}
						else{
							result = DataBaseAccessObj->UpdateUserUGroupRelation(ret , UID->value(),UGID->value(),atoi(DID->value()));
						}
						if ( result )
							XMLCreatorObj->InteractionSuccessAction("FetchAllUserUGroupRelationInteraction", "ok");
						else
							XMLCreatorObj->InteractionFailureAction("Unable To Process","122","FetchAllUserUGroupRelationInteraction");//("DeviceInfo", "ok");
					}
				}
			}
		}
		return true;
	}
	catch (exception &e){
		cout<<"Exception caught in FetchAllUserUGroupRelationInteraction Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::Interactions(xml_node<>* node)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			cout<<"empty node passed into the Action method\n";
			return false;
		}
		nodeName = node->name();
		if(nodeName!="Interaction"){
			cout<<"wrong Node:  "<<nodeName<<" passed as argument to Action Method\n";
			return false;
		}
		xml_attribute<>* attribute = node->first_attribute();

		while(attribute){
			attributeName=attribute->name();
			if(attributeName=="ActionName"){
				string attributeValue = attribute->value();


			if(attributeValue == "StartInteraction"){
				cout<<"An StartInteraction Interactions Request has been made\n";
				StartInteraction(node);
			}
			else if(attributeValue == "RtspInfo"){
					cout<<"An RtspInfo Interactions Request has been made\n";
					RtspInfo(node);
			}
			else if(attributeValue == "DeviceInfo"){
					cout<<"An DeviceInfo Interactions Request has been made\n";
					DeviceInfoInteraction(node);
				}else if(attributeValue == "FetchAllUser"){
					cout<<"An FetchAllUser Interactions Request has been made\n";
					FetchAllUserInteraction(node);
				}else if (attributeValue == "FetchAllUGroup"){
					cout<<"An FetchAllUGroup Interactions Request has been made\n";
					FetchAllUGroupInteraction(node);
				}else if (attributeValue == "FetchAllChannel"){
					cout<<"An FetchAllChannel Interactions Request has been made\n";
					FetchAllChannelInteraction(node);
				}else if (attributeValue == "FetchAllCGroup"){
					cout<<"An FetchAllCGroup Interactions Request has been made\n";
					FetchAllCGroupInteraction(node);
				}else if (attributeValue == "FetchAllUserUGroupRelation"){
					cout<<"An FetchAllUserUGroupRelation Interactions Request has been made\n";
					FetchAllUserUGroupRelationInteraction(node);
				}else if (attributeValue == "FetchAllChannelCGroupRelation"){
					cout<<"An FetchAllChannelCGroupRelation Interactions Request has been made\n";
					FetchAllChannelCGroupRelationInteraction(node);
				}else if ( attributeValue == "FetchUGroupCGroupRelation"){
					cout<<"A UGroupCGroupRelation Interactions Request has been made\n";
					FetchAllUGroupCGroupRelation(node);
				}
				else{
					cout<<"\nInteraction request does not exist for this NAS\n";
				}
				attribute=attribute->next_attribute();
			}
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in Action Function"<<e.what()<<endl;
	}
	return false;
}


bool XMLParser::FetchStorageInfo(xml_node<>* node, char * fileContents){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the FetchStorageInfo method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"FetchStorageInfo Failed\n"<<endl;
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","FetchStorageInfo");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchStorageInfo");
		}else{
			cout<<"FetchStorageInfo Getting Data\n"<<endl;
			xml_node<>* DID = getNodeByName("DID",node);

			if ( DID != NULL  ){

				int SYSDID = DataBaseAccessObj->GetSystemDID();
				if ( atoi(DID->value()) == SYSDID ){
					XMLCreatorObj->CreateStorageInfoXML(DID->value());
				}
				else{
					char deviceIp[100];
					int sockFD ;
					DataBaseAccessObj->GetDeviceIP(deviceIp , atoi(DID->value()) );
					if ( LookingForServer(deviceIp,55555,sockFD )){
						//Send same XML
						int size = 100000;
						sendData(sizeof(int),&size, sockFD);
						sendData(size,fileContents, sockFD);
						int receivedData = 0;
						char receivedXMLData[100000];
						if ( recvData(&receivedData,sizeof(int),sockFD) ){
							if ( recvData(receivedXMLData,receivedData,sockFD) )
								XMLCreatorObj->CreateXMLCreator((char *)receivedXMLData);
						}
						else
							XMLCreatorObj->NewFailureAction("Server Doesnot exist","1232","FetchStorageInfo");
					}
					else
						XMLCreatorObj->NewFailureAction("Server Doesnot exist","1232","FetchStorageInfo");
				}
			}
			else
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","1232","FetchStorageInfo");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in OfflinePerformance Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::ActiveUser(xml_node<>* node, char * fileContents){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the ActiveUser method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"ActiveUser Failed\n"<<endl;
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","ActiveUser");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","ActiveUser");
		}else{
			cout<<"ActiveUser Getting Data\n"<<endl;
			xml_node<>* DID = getNodeByName("DID",node);

			if ( DID != NULL  ){
				cout<<"\ActiveUser ----1"<<endl;
				int SYSDID = DataBaseAccessObj->GetSystemDID();
				if ( atoi(DID->value()) == SYSDID ){
					XMLCreatorObj->CreateActiveUserXML(DID->value());
				}
				else{
					char deviceIp[100];
					int sockFD ;
					DataBaseAccessObj->GetDeviceIP(deviceIp , atoi(DID->value()) );
					if ( LookingForServer(deviceIp,55555,sockFD )){
						//Send same XML
						int size = 100000;
						sendData(sizeof(int),&size, sockFD);
						sendData(size,fileContents, sockFD);
						int receivedDataSize = 0;
						char receivedXMLData[100000];
						if ( recvData(&receivedDataSize,sizeof(int),sockFD) ){
							if ( recvData(receivedXMLData,receivedDataSize,sockFD) ){
								cout<<"\nTrying To get Information\n"<<endl;
								XMLCreatorObj->CreateXMLCreator((char *)receivedXMLData);

							}
						}
						else
							XMLCreatorObj->NewFailureAction("Server Doesnot exist","1232","ActiveUser");
					}
					else
						XMLCreatorObj->NewFailureAction("Server Doesnot exist","1232","ActiveUser");
				}
			}
			else
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","1232","ActiveUser");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in OfflinePerformance Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::OfflinePerformance(xml_node<>* node, char * fileContents){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the OfflinePerformance method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"GetAllChannelDetailsForDID Failed\n"<<endl;
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","OfflinePerformance");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","OfflinePerformance");
		}else{
			cout<<"OfflinePerformance Getting Data\n"<<endl;
			xml_node<>* DID = getNodeByName("DID",node);
			xml_node<>* StartDate = getNodeByName("StartDate",node);
			xml_node<>* StartTime = getNodeByName("StartTime",node);
			xml_node<>* EndDate = getNodeByName("EndDate",node);
			xml_node<>* EndTime = getNodeByName("EndTime",node);

			if ( DID != NULL && StartDate != NULL && StartTime != NULL  && EndDate != NULL && EndTime != NULL ){
				cout<<"\nOfflinePerformance ----1"<<endl;
				XMLCreatorObj->CreateOfflineAnalysisDetails(atoi(DID->value()),StartDate->value(),StartTime->value(),EndDate->value(),EndTime->value());
				//XMLCreatorObj->NewFailureAction("unable to fetch Data","1232","OfflinePerformance");
			}
			else
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","1232","OfflinePerformance");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in OfflinePerformance Function"<<e.what()<<endl;
	}
	return false;
}

///29 jan 2016
bool XMLParser::FetchAllDeviceWithoutDetail(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the GetAllChannelDetailsForDID method");
		}

		nodeName = node->name();

		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"GetAllChannelDetailsForDID Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","GetAllChannelDetailsForDID");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","GetAllChannelDetailsForDID");
		}else{

			xml_node<>* SearchText = getNodeByName("SearchText",node);
			if ( SearchText == NULL )
				XMLCreatorObj->CreateAllDeviceWithoutDetail("");
			else
				XMLCreatorObj->CreateAllDeviceWithoutDetail(SearchText->value());
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in DeviceInfo Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::FetchAllChannelGroupWithoutDetail(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the GetAllChannelDetailsForDID method");
		}

		nodeName = node->name();

		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"GetAllChannelDetailsForDID Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","GetAllChannelDetailsForDID");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","GetAllChannelDetailsForDID");
		}else{
			xml_node<>* SearchText = getNodeByName("SearchText",node);
			if ( SearchText == NULL )
				XMLCreatorObj->CreateAllChannelGroupWithoutDetail("");
			else
				XMLCreatorObj->CreateAllChannelGroupWithoutDetail(SearchText->value());
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in DeviceInfo Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::FetchAllUserGroupWithoutDetail(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the GetAllChannelDetailsForDID method");
		}

		nodeName = node->name();

		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"GetAllChannelDetailsForDID Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","GetAllChannelDetailsForDID");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","GetAllChannelDetailsForDID");
		}else{
			xml_node<>* SearchText = getNodeByName("SearchText",node);
			if ( SearchText == NULL )
				XMLCreatorObj->CreateAllUserGroupWithoutDetail("");
			else
				XMLCreatorObj->CreateAllUserGroupWithoutDetail(SearchText->value());
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in DeviceInfo Function"<<e.what()<<endl;
	}
	return false;
}

void XMLParser::GetGraphDetail(xml_node<>* pNode , const char * tagName , vector<int> &value ){
	xml_node<>* cNode = getNodeByName(tagName,pNode);
	while(cNode!=NULL){
		value.push_back(atoi(cNode->value()));
		cout<<"\nvalue--->"<<cNode->value()<<endl;
		cNode=cNode->next_sibling(tagName);
	}
}

//@22 feb 2016 By Ashu Agarwal//
//For Image Interaction//

void XMLParser::ReadImageData(xml_node<>* node, char *outputPath, char * fileDescriptor, char * imageName){
	try{
		xml_node<>* Row = getNodeByName("Row",node);
		xml_node<>* Col = getNodeByName("Col",node);
		xml_node<>* Data = getNodeByName("Data",node);
		if ( Row != NULL && Col != NULL && Data != NULL ){
			char imagepath[SIZEOFCHARARRAYBIGGER];
			sprintf ( imagepath,"%s/%s", outputPath, imageName);
			cv::Mat image(atoi(Col->value()),atoi(Row->value()),CV_8UC3,(Data->value()));
			//imshow("ASasas",image);
			//waitKey(0);
			imwrite( imagepath,image);
			image.release();
			DataBaseAccessObj->InsertImageInfo(imageName, imagepath,imageRelatedCID,imageRelatedDID, imageRelatedUID);
		}
	}
	catch( cv::Exception e ){

	}
	catch( std::exception e ){

	}
}


bool XMLParser::StoreImage(xml_node<>* node, char * fileDescriptor ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the StoreImage method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"\n StoreImage Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","StoreImage");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","StoreImage");
		}else{
			xml_node<>* Time = getNodeByName("Time",node);
			xml_node<>* ImageName = getNodeByName("ImageName",node);
			xml_node<>* ImageData = getNodeByName("ImageData",node);


			if ( Time != NULL && ImageData != NULL && ImageName != NULL ){
				if ( imageRelatedCID != -1 && imageRelatedDID  != -1 && imageRelatedUID  != -1 && outputPath != NULL )
				{
					ReadImageData(ImageData, outputPath, fileDescriptor, ImageName->value());
				}
				else
					XMLCreatorObj->NewFailureAction("Please Set Image Informations","102","StoreImage");
			}
			else
				XMLCreatorObj->NewFailureAction("Information is not proper","102","StoreImage");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in StoreImage Function"<<e.what()<<endl;
	}
	return false;
}


bool XMLParser::SetImageInfo(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the SetImageInfo method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"\SetImageInfo Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","SetImageInfo");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","SetImageInfo");
		}else{
			xml_node<>* CGName = getNodeByName("CGName",node);
			xml_node<>* CName = getNodeByName("CName",node);
			xml_node<>* UID = getNodeByName("UID",node);
			xml_node<>* UserName = getNodeByName("UserName",node);

			if( CGName != NULL && CName != NULL && ( UID != NULL || UserName != NULL ) ){

				DataBaseAccessObj->GetChannelID( CGName->value(),CName->value(),imageRelatedCID,imageRelatedDID );
				DataBaseAccessObj->GetOutputFolder( outputPath ,imageRelatedCID, imageRelatedDID );
				sprintf( outputPath,"%s/CID%dDID%d",outputPath,imageRelatedCID,imageRelatedDID);
				CommonClassObj->CreateDirectory(outputPath);
				if ( UID != NULL )
					imageRelatedUID = atoi(UID->value());
				else
					DataBaseAccessObj->GetUserID(UserName->value(),imageRelatedUID);
				XMLCreatorObj->NewSuccessAction( "SetImageInfo" , "Ok" );
			}
			else
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","2030","SetImageInfo");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in SetImageInfo Function"<<e.what()<<endl;
	}
	return false;
}




//@22 feb 2016 By Ashu Agarwal//
//For QT Interaction//



bool XMLParser::GetChannelInfo(xml_node<>* node, char * fileContents ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the GetChannelInfo method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"\nLoginFromQT Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","GetChannelInfo");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","GetChannelInfo");
		}else{
			xml_node<>* CGName = getNodeByName("CGName",node);
			xml_node<>* CName = getNodeByName("CName",node);
			if ( CGName != NULL && CName != NULL ){
				//cout<<"\n1111"<<endl;
				int CID = -1, DID = -1;
				DataBaseAccessObj->GetChannelID(CGName->value(), CName->value(),CID,DID);
				//cout<<"\n2222"<<endl;
				if ( CID != -1 && DID != -1 ){
					int sysDID = DataBaseAccessObj->GetSystemDID();
					//	cout<<"\n3333"<<endl;
					if ( sysDID == DID ){
						char rtsp[SIZEOFCHARARRAYBIG];
						//	cout<<"\n3344"<<endl;
						if ( RetrieveObj->GetChannelRtsp(CID,DID,rtsp) )
							XMLCreatorObj->CreateChannelInfo(rtsp);
						else
							XMLCreatorObj->NewFailureAction("Resending disable","102","GetChannelInfo");
					}
					else{
						//	cout<<"\n4444"<<endl;
						char sysDIDIP[SIZEOFCHARARRAYBIGGER];
						DataBaseAccessObj->GetDeviceIP(sysDIDIP,sysDID);
						int sockInter = -1;
						if ( LookingForServer(sysDIDIP,55555,sockInter)){
							if ( sockInter != -1 ){
								int size = 1000;
								sendData(sizeof(int),&size, sockInter);
								sendData(size,fileContents, sockInter);
								int receivedData = 0;
								char receivedXMLData[100000];
								if ( recvData(&receivedData,sizeof(int),sockInter) )
									if ( recvData(receivedXMLData,receivedData,sockInter) ){
										XMLCreatorObj->CreateXMLCreator((char *)receivedXMLData);
									}
							}
							else
								XMLCreatorObj->NewFailureAction("Server does not exist","102","GetChannelInfo");
						}
						else
							XMLCreatorObj->NewFailureAction("Server does not exist","103","GetChannelInfo");
					}
				}
				else{
					XMLCreatorObj->NewFailureAction("Channel does not exist","102","GetChannelInfo");
				}

			}
			else
				XMLCreatorObj->NewFailureAction("XML is null","102","GetChannelInfo");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in GetChannelInfo Function"<<e.what()<<endl;
	}
	return false;
}
/*
bool XMLParser::ChannelRtspRequestQT(xml_node<>* node ){
	string nodeName;
	string attributeName;
	if(!node){
		throw runtime_error("empty node passed into the ChannelRtspRequestQT method");
	}
	nodeName = node->name();
	if(nodeName!="Action"){
		throw runtime_error("wrong Node passed as argument to Action Method");
	}

	if(0){
		cout<<"\nLoginFromQT Failed\n";
		XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","ChannelRtspRequestQT");
	}else if(0){
		XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","ChannelRtspRequestQT");
	}else{
		xml_node<>* UserName = getNodeByName("UserName",node);
		xml_node<>* Password = getNodeByName("Password",node);

		//cout<<"\nUserName--->"<<UserName->value();
		//cout<<"\n1"<<endl;
		if ( UserName != NULL && Password != NULL ){
			int UID = -1;
			//cout<<"\n21"<<endl;
			bool LoginSuccess = DataBaseAccessObj->AuthenticateUser ( UserName->value(),Password->value(),&UID );
		//	cout<<"\n3"<<endl;
			//cout<<"\nUID--->"<<UID<<endl;
			if(LoginSuccess){
			//	cout<<"\n4"<<endl;
				XMLCreatorObj->CreateLoginSuccessXml(UID);
				LoggedUID = UID;
			}
			else{
				//cout<<"\n5"<<endl;
				XMLCreatorObj->NewFailureAction("User does not exists","102","LoginFromQT");

			}
		}
		else{
			//cout<<"\6"<<endl;
			XMLCreatorObj->NewFailureAction("XML is null","102","LoginFromQT");
		}
	}
	return true;
}catch (exception &e){
	cout<<"Exception caught in LoginFromQT Function"<<e.what()<<endl;
}
return false;

}

*/
bool XMLParser::LoginFromQT(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the LoginFromQT method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"\nLoginFromQT Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","LoginFromQT");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","LoginFromQT");
		}else{
			xml_node<>* UserName = getNodeByName("UserName",node);
			xml_node<>* Password = getNodeByName("Password",node);

			//cout<<"\nUserName--->"<<UserName->value();
			//cout<<"\n1"<<endl;
			if ( UserName != NULL && Password != NULL ){
				int UID = -1;
				//cout<<"\n21"<<endl;
				bool LoginSuccess = DataBaseAccessObj->AuthenticateUser ( UserName->value(),Password->value(),&UID );
			//	cout<<"\n3"<<endl;
				//cout<<"\nUID--->"<<UID<<endl;
				if(LoginSuccess){
				//	cout<<"\n4"<<endl;
					XMLCreatorObj->CreateLoginSuccessXml(UID);
					LoggedUID = UID;
				}
				else{
					//cout<<"\n5"<<endl;
					XMLCreatorObj->NewFailureAction("User does not exists","102","LoginFromQT");

				}
			}
			else{
				//cout<<"\6"<<endl;
				XMLCreatorObj->NewFailureAction("XML is null","102","LoginFromQT");
			}
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in LoginFromQT Function"<<e.what()<<endl;
	}
	return false;
}


bool XMLParser::NotificationEvent(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Notification method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"VerificationCamera Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","Notification");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","Notification");
		}else{
			xml_node<>* msg = getNodeByName("Msg",node);
			xml_node<>* channelName = getNodeByName("ChannelName",node);
			xml_node<>* serverName = getNodeByName("ServerName",node);
			xml_node<>* CID = getNodeByName("CID",node);
			xml_node<>* DID = getNodeByName("DID",node);

			if ( msg != NULL && channelName != NULL && serverName != NULL ){
				notification->SendMailData(msg->value(), channelName->value(),serverName->value(), CID->value(), DID->value());
				XMLCreatorObj->NewSuccessAction("Notification","ok");
			}
			else
				XMLCreatorObj->NewFailureAction("XML is null","102","Notification");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in Notification Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::VerificationCamera(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the VerificationCamera method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"VerificationCamera Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","VerificationCamera");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","VerificationCamera");
		}else{
			xml_node<>* cameraIP = getNodeByName("IP",node);
			xml_node<>* portNumber = getNodeByName("PortNumber",node);
			xml_node<>* userName = getNodeByName("UserName",node);
			xml_node<>* password = getNodeByName("Password",node);
			if ( cameraIP != NULL && portNumber != NULL && userName != NULL && password != NULL ){
				char rtsp[1000];
				if ( RetrieveObj->VerificationCamera(cameraIP->value(),portNumber->value(),userName->value(),password->value(), rtsp)){
					XMLCreatorObj->NewSuccessActionWithString("VerificationCamera","Ok","RTSP",rtsp);
				}
				else
					XMLCreatorObj->NewFailureAction("Camera does not support onvif","1402","VerificationCamera");
			}
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in VerificationCamera Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::ContinuousMove(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the ContinuousMove method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"ContinuousMove Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","ContinuousMove");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","ContinuousMove");
		}else{
			xml_node<>* channelName = getNodeByName("ChannelName",node);
			if ( channelName != NULL  ){
				int CID = -1, DID = -1;
				DataBaseAccessObj->GetChannelInfo(channelName->value(),CID, DID);
				if ( RetrieveObj->ContinuousMove(CID,DID )  ){
					XMLCreatorObj->NewSuccessAction("ContinuousMove","OK");
				}
				else
					XMLCreatorObj->NewFailureAction("camera does not support move","102","ContinuousMove");
			}
			else
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","102","ContinuousMove");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in ContinuousMove Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::RelativeMove (xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the RelativeMove method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"RelativeMove Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","RelativeMove");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","RelativeMove");
		}else{
			xml_node<>* channelName = getNodeByName("ChannelName",node);
			xml_node<>* index = getNodeByName("Index",node);

			if ( channelName != NULL && index != NULL ){
				int CID = -1, DID = -1;
				DataBaseAccessObj->GetChannelInfo(channelName->value(),CID, DID);
				if ( RetrieveObj->RelativeMove( CID, DID,atoi(index->value()) ) )
					XMLCreatorObj->NewSuccessAction("RelativeMove","OK");
				else
					XMLCreatorObj->NewFailureAction("camera does not support move","102","RelativeMove");
			}
			else
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","102","RelativeMove");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in RelativeMove Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::GetGraphInfo(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the GetGraphInfo method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"GetGraphInfo Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","GetGraphInfo");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","GetGraphInfo");
		}else{
			//XMLCreatorObj->CreateAllUserWithoutDetail();
			xml_node<>* DInfo = getNodeByName("DInfo",node);
			xml_node<>* UInfo = getNodeByName("UInfo",node);
			xml_node<>* UGInfo = getNodeByName("UGInfo",node);
			xml_node<>* CGInfo = getNodeByName("CGInfo",node);
			xml_node<>* CInfo = getNodeByName("CInfo",node);
			xml_node<>* USInfo = getNodeByName("USERSHOW",node);
			xml_node<>* UGSInfo = getNodeByName("USERGROUPSHOW",node);
			xml_node<>* CGSInfo = getNodeByName("CHANNELGROUPSHOW",node);
			xml_node<>* CSInfo = getNodeByName("CHANNELSHOW",node);
			xml_node<>* DSInfo = getNodeByName("DEVICESHOW",node);
			vector<int> inputUID, inputUGID, inputCGID, inputCID, inputDID;
			vector<int> tmpUID, tmpUGID, tmpCGID, tmpCID, tmpDID;
			vector<int> resultUID, resultUGID, resultCGID, resultCID, resultDID;
			GetGraphDetail(UInfo,"UID", inputUID);
			GetGraphDetail(UGInfo,"UGID", inputUGID);
			GetGraphDetail(CGInfo,"CGID", inputCGID);
			GetGraphDetail(CInfo,"CID", inputCID);
			GetGraphDetail(DInfo,"DID", inputDID);
			bool isUIDSelected , isUGIDSelected, isCGIDSelected, isCIDSelected, isDIDSelected;

			isUIDSelected = atoi(USInfo->value());
			isUGIDSelected = atoi(UGSInfo->value());
			isCGIDSelected = atoi(CGSInfo->value());
			isCIDSelected = atoi(CSInfo->value());
			isDIDSelected = atoi(DSInfo->value());

			// user based graph
			resultUID = inputUID;
			resultUGID = inputUGID;
			resultCGID = inputCGID;
			resultCID = inputCID;
			resultDID = inputDID;

			if ( inputUID.size() > 0 && isUIDSelected){
				tmpUID = inputUID;
				tmpUGID.clear();
				tmpCGID.clear();
				tmpCID.clear();
				tmpDID.clear();
				if(isUGIDSelected){
					DataBaseAccessObj->GetUGIDRelatedToUID(tmpUGID, tmpUID);
					if(isCGIDSelected){
						DataBaseAccessObj->GetCGIDRelatedToUGID(tmpCGID, tmpUGID);
						if(isCIDSelected){
							DataBaseAccessObj->GetCIDRelatedToCGID(tmpCID, tmpCGID);
							if(isDIDSelected){
								DataBaseAccessObj->GetDIDRelatedToCID(tmpDID, tmpCID);
							}
						}
					}
				}
				copy(tmpUID.begin(), tmpUID.end(), back_inserter(resultUID));
				copy(tmpUGID.begin(), tmpUGID.end(), back_inserter(resultUGID));
				copy(tmpCGID.begin(), tmpCGID.end(), back_inserter(resultCGID));
				copy(tmpCID.begin(), tmpCID.end(), back_inserter(resultCID));
				copy(tmpDID.begin(), tmpDID.end(), back_inserter(resultDID));
			}

			//user group
			if ( inputUGID.size() > 0  && isUGIDSelected){
				tmpUGID = inputUGID;
				tmpUID.clear();
				tmpCGID.clear();
				tmpCID.clear();
				tmpDID.clear();
				if(isUIDSelected){
					DataBaseAccessObj->GetUIDRelatedToUGID(tmpUID, tmpUGID);
				}

				if(isCGIDSelected){
					DataBaseAccessObj->GetCGIDRelatedToUGID(tmpCGID, tmpUGID);
					if(isCIDSelected){
						DataBaseAccessObj->GetCIDRelatedToCGID(tmpCID, tmpCGID);
						if(isDIDSelected){
							DataBaseAccessObj->GetDIDRelatedToCID(tmpDID, tmpCID);
						}
					}
				}
				copy(tmpUID.begin(), tmpUID.end(), back_inserter(resultUID));
				copy(tmpUGID.begin(), tmpUGID.end(), back_inserter(resultUGID));
				copy(tmpCGID.begin(), tmpCGID.end(), back_inserter(resultCGID));
				copy(tmpCID.begin(), tmpCID.end(), back_inserter(resultCID));
				copy(tmpDID.begin(), tmpDID.end(), back_inserter(resultDID));
			}

			//channel group
			if ( inputCGID.size() > 0  && isCGIDSelected){
				tmpCGID = inputCGID;
				tmpUID.clear();
				tmpUGID.clear();
				tmpCID.clear();
				tmpDID.clear();
				if(isUGIDSelected){
					DataBaseAccessObj->GetUGIDRelatedToCGID(tmpUGID,tmpCGID);
					if(isUIDSelected){
						DataBaseAccessObj->GetUIDRelatedToUGID(tmpUID, tmpUGID);
					}
				}

				if(isCIDSelected){
					DataBaseAccessObj->GetCIDRelatedToCGID(tmpCID, tmpCGID);
					if(isDIDSelected){
						DataBaseAccessObj->GetDIDRelatedToCID(tmpDID, tmpCID);
					}
				}
				copy(tmpUID.begin(), tmpUID.end(), back_inserter(resultUID));
				copy(tmpUGID.begin(), tmpUGID.end(), back_inserter(resultUGID));
				copy(tmpCGID.begin(), tmpCGID.end(), back_inserter(resultCGID));
				copy(tmpCID.begin(), tmpCID.end(), back_inserter(resultCID));
				copy(tmpDID.begin(), tmpDID.end(), back_inserter(resultDID));
			}

			//channel
			if ( inputCID.size() > 0  && isCIDSelected){
				tmpCID = inputCID;
				tmpUID.clear();
				tmpUGID.clear();
				tmpCGID.clear();
				tmpDID.clear();
				if(isCGIDSelected){
					DataBaseAccessObj->GetCGIDRelatedToCID (tmpCGID, tmpCID);
					if(isUGIDSelected){
						DataBaseAccessObj->GetUGIDRelatedToCGID(tmpUGID,tmpCGID);
						if(isUIDSelected){
							DataBaseAccessObj->GetUIDRelatedToUGID(tmpUID, tmpUGID);
						}
					}
				}

				if(isDIDSelected){
					DataBaseAccessObj->GetDIDRelatedToCID(tmpDID, tmpCID);
				}
				copy(tmpUID.begin(), tmpUID.end(), back_inserter(resultUID));
				copy(tmpUGID.begin(), tmpUGID.end(), back_inserter(resultUGID));
				copy(tmpCGID.begin(), tmpCGID.end(), back_inserter(resultCGID));
				copy(tmpCID.begin(), tmpCID.end(), back_inserter(resultCID));
				copy(tmpDID.begin(), tmpDID.end(), back_inserter(resultDID));
			}

			//Server
			if ( inputDID.size() > 0 && isDIDSelected){
				tmpDID = inputDID;
				tmpUID.clear();
				tmpUGID.clear();
				tmpCGID.clear();
				tmpCID.clear();
				if(isCIDSelected){
					DataBaseAccessObj->GetCIDRelatedToDID(tmpCID,tmpDID);
					if(isCGIDSelected){
						DataBaseAccessObj->GetCGIDRelatedToCID (tmpCGID, tmpCID);
						if(isUGIDSelected){
							DataBaseAccessObj->GetUGIDRelatedToCGID(tmpUGID,tmpCGID);
							if(isUIDSelected){
								DataBaseAccessObj->GetUIDRelatedToUGID(tmpUID, tmpUGID);
							}
						}
					}
				}

				copy(tmpUID.begin(), tmpUID.end(), back_inserter(resultUID));
				copy(tmpUGID.begin(), tmpUGID.end(), back_inserter(resultUGID));
				copy(tmpCGID.begin(), tmpCGID.end(), back_inserter(resultCGID));
				copy(tmpCID.begin(), tmpCID.end(), back_inserter(resultCID));
				copy(tmpDID.begin(), tmpDID.end(), back_inserter(resultDID));
			}


			sort(resultUID.begin(), resultUID.end());
			resultUID.erase((unique(resultUID.begin(), resultUID.end())), resultUID.end());
			sort(resultUGID.begin(), resultUGID.end());
			resultUGID.erase((unique(resultUGID.begin(), resultUGID.end())), resultUGID.end());
			sort(resultCGID.begin(), resultCGID.end());
			resultCGID.erase((unique(resultCGID.begin(), resultCGID.end())), resultCGID.end());
			sort(resultCID.begin(), resultCID.end());
			resultCID.erase((unique(resultCID.begin(), resultCID.end())), resultCID.end());
			sort(resultDID.begin(), resultDID.end());
			resultDID.erase((unique(resultDID.begin(), resultDID.end())), resultDID.end());


			/*

			if ( UGID.size() > 0 )
			{
				if ( !isUIDSelected )
					DataBaseAccessObj->GetUIDRelatedToUGID(UID,UGID);

				DataBaseAccessObj->GetCGIDRelatedToUGID(CGID,UGID);
			}

			if ( CGID.size() > 0 ){

				DataBaseAccessObj->GetCIDRelatedToCGID(CID,CGID);
				if ( !isUIDSelected && !isUGIDSelected ){
					DataBaseAccessObj->GetUGIDRelatedToCGID(UGID,CGID);
					DataBaseAccessObj->GetUIDRelatedToUGID(UID,UGID);
				}
			}

			if ( CID.size() > 0 ){
				DataBaseAccessObj->GetDIDRelatedToCID(DID,CID);
				if ( !isUIDSelected && !isUGIDSelected && !isCGIDSelected ){
					DataBaseAccessObj->GetCGIDRelatedToCID (CGID,CID);
					DataBaseAccessObj->GetUGIDRelatedToCGID (UGID,CGID);
					DataBaseAccessObj->GetUIDRelatedToUGID (UID,UGID);
				}
			}

			if ( DID.size() > 0 ){
				if ( !isUIDSelected && !isUGIDSelected && !isCGIDSelected && !isCIDSelected ){
					DataBaseAccessObj->GetCIDRelatedToDID(CID,DID);
					DataBaseAccessObj->GetCGIDRelatedToCID(CGID,CID);
					DataBaseAccessObj->GetUGIDRelatedToCGID(UGID,CGID);
					DataBaseAccessObj->GetUIDRelatedToUGID(UID,UGID);
				}
			}

			*/
			XMLCreatorObj->CreateGraphInfo(resultUID, resultUGID, resultCGID, resultCID, resultDID);
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in GetGraphInfo Function"<<e.what()<<endl;
	}
	return false;
}

/*
bool XMLParser::GetGraphInfo(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the GetGraphInfo method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"GetGraphInfo Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","GetGraphInfo");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","GetGraphInfo");
		}else{
			//XMLCreatorObj->CreateAllUserWithoutDetail();
			xml_node<>* DInfo = getNodeByName("DInfo",node);
			xml_node<>* UInfo = getNodeByName("UInfo",node);
			xml_node<>* UGInfo = getNodeByName("UGInfo",node);
			xml_node<>* CGInfo = getNodeByName("CGInfo",node);
			xml_node<>* CInfo = getNodeByName("CInfo",node);
			vector<int> UGID,UID,CGID,CID,DID;
			GetGraphDetail(UInfo,"UID",UID);
			GetGraphDetail(UGInfo,"UGID",UGID);
			GetGraphDetail(CGInfo,"CGID",CGID);
			GetGraphDetail(CInfo,"CID",CID);
			GetGraphDetail(DInfo,"DID",DID);
			bool isUIDSelected , isUGIDSelected, isCGIDSelected, isCIDSelected, isDIDSelected;
			if ( UID.size() > 0 )
				isUIDSelected = true;
			if ( UGID.size() > 0 )
				isUGIDSelected = true;
			if ( CGID.size() > 0 )
				isCGIDSelected = true;
			if ( CID.size() > 0 )
				isCIDSelected = true;
			if ( DID.size() > 0 )
				isDIDSelected = true;

			if ( UID.size() > 0 ){
				DataBaseAccessObj->GetUGIDRelatedToUID(UGID,UID);
			}

			if ( UGID.size() > 0 )
			{
				if ( !isUIDSelected )
					DataBaseAccessObj->GetUIDRelatedToUGID(UID,UGID);

				DataBaseAccessObj->GetCGIDRelatedToUGID(CGID,UGID);
			}

			if ( CGID.size() > 0 ){

				DataBaseAccessObj->GetCIDRelatedToCGID(CID,CGID);
				if ( !isUIDSelected && !isUGIDSelected ){
					DataBaseAccessObj->GetUGIDRelatedToCGID(UGID,CGID);
					DataBaseAccessObj->GetUIDRelatedToUGID(UID,UGID);
				}
			}

			if ( CID.size() > 0 ){
				DataBaseAccessObj->GetDIDRelatedToCID(DID,CID);
				if ( !isUIDSelected && !isUGIDSelected && !isCGIDSelected ){
					DataBaseAccessObj->GetCGIDRelatedToCID (CGID,CID);
					DataBaseAccessObj->GetUGIDRelatedToCGID (UGID,CGID);
					DataBaseAccessObj->GetUIDRelatedToUGID (UID,UGID);
				}
			}

			if ( DID.size() > 0 ){
				if ( !isUIDSelected && !isUGIDSelected && !isCGIDSelected && !isCIDSelected ){
					DataBaseAccessObj->GetCIDRelatedToDID(CID,DID);
					DataBaseAccessObj->GetCGIDRelatedToCID(CGID,CID);
					DataBaseAccessObj->GetUGIDRelatedToCGID(UGID,CGID);
					DataBaseAccessObj->GetUIDRelatedToUGID(UID,UGID);
				}
			}
			XMLCreatorObj->CreateGraphInfo(UID,UGID,CGID,CID,DID);
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in GetGraphInfo Function"<<e.what()<<endl;
	}
	return false;
}
*/
bool XMLParser::FetchAllUserWithoutDetail(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the GetAllChannelDetailsForDID method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"GetAllChannelDetailsForDID Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","GetAllChannelDetailsForDID");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","GetAllChannelDetailsForDID");
		}else{
			xml_node<>* SearchText = getNodeByName("SearchText",node);
			if ( SearchText == NULL )
				XMLCreatorObj->CreateAllUserWithoutDetail("");
			else
				XMLCreatorObj->CreateAllUserWithoutDetail(SearchText->value());
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in DeviceInfo Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::FetchAllChannelWithoutDetail(xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the GetAllChannelDetailsForDID method");
		}

		nodeName = node->name();

		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"GetAllChannelDetailsForDID Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","GetAllChannelDetailsForDID");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","GetAllChannelDetailsForDID");
		}else{
			xml_node<>* SearchText = getNodeByName("SearchText",node);
		if ( SearchText == NULL )
			XMLCreatorObj->CreateAllChannelWithoutDetail("");
		else
			XMLCreatorObj->CreateAllChannelWithoutDetail(SearchText->value());
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in DeviceInfo Function"<<e.what()<<endl;
	}
	return false;
}


///*****************************/



bool XMLParser::GetAllChannelDetailsForDID(xml_node<>* node, char * fileContents){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the GetAllChannelDetailsForDID method");
		}

		nodeName = node->name();

		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"GetAllChannelDetailsForDID Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","GetAllChannelDetailsForDID");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","GetAllChannelDetailsForDID");
		}else{

			//		cout<<"\nstart Process Monitoring\n"<<endl;
			xml_node<>* DID = getNodeByName("DID",node);
			if ( DID != NULL ){
				XMLCreatorObj->CreateAllChannelDetails(atoi(DID->value()));
			}
			else
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","1232","GetAllChannelDetailsForDID");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in GetAllChannelDetailsForDID Function"<<e.what()<<endl;
	}
	return false;
}



bool XMLParser::StartProcessMonitoring(xml_node<>* node, char * fileContents){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the StartProcessMonitoring method");
		}

		nodeName = node->name();

		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"StartProcessMonitoring Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","StartProcessMonitoring");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","StartProcessMonitoring");
		}else{

			cout<<"\nstart Process Monitoring\n"<<endl;
			xml_node<>* DID = getNodeByName("DID",node);
			xml_node<>* fillterValue = getNodeByName("FillterValue",node);
			xml_node<>* StartDate = getNodeByName("StartDate",node);
			xml_node<>* StartTime = getNodeByName("StartTime",node);
			xml_node<>* EndDate = getNodeByName("EndDate",node);
			xml_node<>* EndTime = getNodeByName("EndTime",node);

			if ( DID != NULL && fillterValue != NULL && StartDate != NULL && StartTime != NULL && EndDate != NULL && EndTime != NULL){
				if ( atoi(DID->value()) == DataBaseAccessObj->GetSystemDID() ){
					if ( strcmp(fillterValue->value() , "live") == 0 )
						XMLCreatorObj->CreateProcessMonitoringXML( atoi(DID->value()), fillterValue->value(),(char*)NULL ,(char*)NULL,(char*)NULL,(char*)NULL);
					else
						XMLCreatorObj->CreateProcessMonitoringXML( atoi(DID->value()), fillterValue->value(),StartDate->value(),StartTime->value(),EndDate->value(),EndTime->value() );
				}
				else{
					cout<<"\nProcessMonitoring Else\n"<<endl;
					char deviceIp[SIZEOFCHARARRAY];
					int sockFD = -1;
					DataBaseAccessObj->GetDeviceIP(deviceIp, atoi(DID->value()));
					if ( LookingForServer(deviceIp,55555,sockFD ) ){
						//Send same XML
						int size = 100000;
						sendData(sizeof(int),&size, sockFD);
						sendData(size,fileContents, sockFD);
						int receivedData = 0;
						char receivedXMLData[100000];
						if ( recvData(&receivedData,sizeof(int),sockFD) )
							if ( recvData(receivedXMLData,receivedData,sockFD) )
								XMLCreatorObj->CreateXMLCreator((char *)receivedXMLData);

					}else
						XMLCreatorObj->NewFailureAction("Server not exists","102","StartProcessMonitoring");
				}
			}else
				XMLCreatorObj->NewFailureAction("Information is missing","102","StartProcessMonitoring");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in StartProcessMonitoring Function"<<e.what()<<endl;
	}
	return false;
}


bool XMLParser::StartRestream(xml_node<>* node){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the StartRestream method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"StartRestream Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","StartRestream");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","StartRestream");
		}else{

			xml_node<>* restreamInfo = getNodeByName("RestreamInfo",node);
			if ( restreamInfo != NULL ){
				int restreamID = 0;
				xml_node<> *CID,*DID, *width, *height, *frameRate, *bitRate;
				CID = getNodeByName("CID",restreamInfo);
				DID = getNodeByName("DID",restreamInfo);
				width = getNodeByName("Width",restreamInfo);
				height = getNodeByName("Height",restreamInfo);
				frameRate = getNodeByName("FrameRate",restreamInfo);
				bitRate  = getNodeByName("BitRate",restreamInfo);

				if ( CID != NULL && DID != NULL && width != NULL && height != NULL && frameRate != NULL && bitRate != NULL  ){
					restreamID =  DataBaseAccessObj->InsertReStream(CID->value(), DID->value(), width->value(), height->value(), frameRate->value(), bitRate->value());
					if ( restreamID == 0 ){
						XMLCreatorObj->NewFailureAction("Unable to Insert into dataBase","133","StartRestream");
					}
					else{
						XMLCreatorObj->SuccessRestream(restreamID,"StartRestream");
						cout<<"\nStarting Restreaming\n";
						RetrieveObj->AddReStreaming(atoi(CID->value()),atoi(DID->value()),width->value(),height->value(),frameRate->value(),bitRate->value(),restreamID);
					}
				}
				else
					XMLCreatorObj->NewFailureAction("Information is missing","102","StartRestream");
			}
			else
				XMLCreatorObj->NewFailureAction("Unable to Find RestreamInfo","102","StartRestream");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in StartProcessMonitoring Function"<<e.what()<<endl;
	}
	return false;
}




bool XMLParser::DeviceInfo(xml_node<>* node){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the DeviceInfo method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}

		if(0){
			cout<<"DeviceInfo Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","DeviceInfo");
		}else if(0){
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","DeviceInfo");
		}else{
			char macAddress[SIZEOFCHARARRAY], deviceName[SIZEOFCHARARRAY];
			int DID;
			xml_node<>* DeviceIP = getNodeByName("DeviceIP",node);

			if ( DataBaseAccessObj->GetDeviceInfo(deviceName, DeviceIP->value(),DID, macAddress) )
				XMLCreatorObj->NewSuccessDeviceInfoAction(DID,macAddress,deviceName);
			else
				XMLCreatorObj->NewFailureAction("Unable to Find serverInfo","102","DeviceInfo");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in DeviceInfo Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::Action(xml_node<>* node, char * fileContents)
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			cout<<"empty node passed into the Action method\n";
			return false;
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			cout<<"wrong Node:  "<<nodeName<<" passed as argument to Action Method\n";
			return false;
		}
		xml_attribute<>* attribute = node->first_attribute();
		while(attribute){
			attributeName=attribute->name();
			if(attributeName=="Name"){
				string attributeValue = attribute->value();
				if ( attributeValue == "StoreImage"){
					cout<<"An StoreImage Request has been made\n";
					StoreImage( node , fileContents);
				}
				else  if ( attributeValue == "SetImageInfo"){
					cout<<"An SetImageInfo Request has been made\n";
					SetImageInfo(node);
				}
				else if ( attributeValue == "GetChannelInfo"){
					cout<<"An GetChannelInfo Request has been made\n";
					GetChannelInfo(node, fileContents);
				}
				else if (attributeValue == "LoginFromQT"){
					cout<<"An LoginFromQT Request has been made\n";
					LoginFromQT(node);
				}
				/*else if (attributeValue == "ChannelRtspRequestQT"){
					cout<<"An LoginFromQT Request has been made\n";
					ChannelRtspRequestQT(node);
				}*/
				else if (attributeValue == "Notification"){
					cout<<"An Notification Request has been made\n";
					NotificationEvent(node);
				}
				else if (attributeValue == "VerificationCamera"){
					cout<<"An VerificationCamera Request has been made\n";
					VerificationCamera(node);
				}
				else if (attributeValue == "RelativeMove"){
					cout<<"An RelativeMove Request has been made\n";
					RelativeMove(node);
				}
				else if (attributeValue == "ContinuousMove"){
					cout<<"An ContinousMove Request has been made\n";
					ContinuousMove(node);
				}
				else if (attributeValue == "GetGraphInfo"){
					cout<<"An GetGraphInfo Request has been made\n";
					GetGraphInfo(node);
				}
				else if (attributeValue == "FetchAllUserWithoutDetail"){
					cout<<"An FetchAllUserWithoutDetail Request has been made\n";
					FetchAllUserWithoutDetail(node);
				}
				else if (attributeValue == "FetchAllDeviceWithoutDetail"){
					cout<<"An FetchAllDeviceWithoutDetail Request has been made\n";
					FetchAllDeviceWithoutDetail(node);
				}
				else if (attributeValue == "FetchAllChannelWithoutDetail"){
					cout<<"An FetchAllChannelWithoutDetail Request has been made\n";
					FetchAllChannelWithoutDetail(node);
				}
				else if (attributeValue == "FetchAllChannelGroupWithoutDetail"){
					cout<<"An FetchAllChannelGroupWithoutDetail Request has been made\n";
					FetchAllChannelGroupWithoutDetail(node);
				}
				else if (attributeValue == "FetchAllUserGroupWithoutDetail"){
					cout<<"An FetchAllUserGroupWithoutDetail Request has been made\n";
					FetchAllUserGroupWithoutDetail(node);
				}
				else if (attributeValue == "FetchStorageInfo"){
					cout<<"An FetchStorageInfo Request has been made\n";
					FetchStorageInfo(node, fileContents);
				}
				else if (attributeValue == "ActiveUser"){
					cout<<"An ActiveUser Request has been made\n";
					ActiveUser(node, fileContents);
				}
				else if (attributeValue == "OfflinePerformance"){
					cout<<"An OfflinePerfomance Request has been made\n";
					OfflinePerformance(node, fileContents);
				}
				else if (attributeValue == "GetAllChannelDetailsForDID"){
					cout<<"An StartRestream Request has been made\n";
					GetAllChannelDetailsForDID(node, fileContents);
				}
				else if (attributeValue == "StartProcessMonitoring"){
					cout<<"An StartRestream Request has been made\n";
					StartProcessMonitoring(node, fileContents);
				}
				else if (attributeValue == "StartRestream"){
					cout<<"An StartRestream Request has been made\n";
					StartRestream(node);
				}else if (attributeValue == "GetDeviceInfo"){
					cout<<"An GetDeviceInfo Request has been made\n";
					DeviceInfo(node);
				}else if(attributeValue == "AddUser"){
					cout<<"An Adduser Request has been made\n";
					AddUser(node);
				}else if(attributeValue == "AddDevice"){
					cout<<"An AddDevice Request has been made\n";
					AddServer(node);
				}else if(attributeValue == "Login"){
					cout<<"A Login Request has been made\n";
					Login(node);
				}else if(attributeValue == "AddUGroup"){
					cout<<"A AddUGroup Request has been made\n";
					AddUGroup(node);
				}else if(attributeValue == "AddCGroup"){
					cout<<"A AddCGroup Request has been made\n";
					AddCGroup(node);
				} else if(attributeValue == "AddUserUGroupRelation"){
					cout<<"A AddUserUGroupRelation Request has been made\n";
					AddUserUGroupRelation(node);
				} else if(attributeValue == "AddChannelCGroupRelation"){
					cout<<"A AddChannelCGroupRelation Request has been made\n";
					AddChannelCGroupRelation(node);
				}  else if(attributeValue == "AddUGroupCGroupRelation"){
					cout<<"A AddUGroupCGroupRelation Request has been made\n";
					AddUGroupCGroupRelation(node);
				} else if(attributeValue == "AddChannel"){
					cout<<"A AddChannel Request has been made\n";
					AddChannel(node,fileContents);
				}else if(attributeValue == "FetchAllUser"){
					cout<<"A FetchAllUser Request has been made\n";
					FetchAllUser(node);
				}else if(attributeValue == "FetchAllDevice"){
					cout<<"A FetchAllDevice Request has been made\n";
					FetchAllDevice(node);
				}else if(attributeValue == "StartVOD"){
					cout<<"A StartVOD Request has been made\n";
					StartVOD(node);
				}else if(attributeValue == "StartDumping"){
					cout<<"A StartDumping Request has been made\n";
					StartDumping(node);
				}else if(attributeValue == "StopDumping"){
					cout<<"A StopDumping Request has been made\n";
					StopDumping(node);
				}else if(attributeValue == "FetchAllUGroup"){
					cout<<"A FetchAllUGroup Request has been made\n";
					FetchAllUGroup(node);
				}else if(attributeValue == "UpdateUser"){
					cout<<"A UpdateUser Request has been made\n";
					UpdateUser(node);
				}else if(attributeValue == "UpdateDevice"){
					cout<<"A UpdateDevice Request has been made\n";
					UpdateDevice(node);
				}else if(attributeValue == "FetchAllUserUGroupRelation"){
					cout<<"A FetchAllUserUGroupRelation Request has been made\n";
					FetchAllUserUGroupRelation(node);
				}else if(attributeValue == "FetchAllVideo"){
					cout<<"A FetchAllVideo Request has been made\n";
					FetchAllVideo(node);
				}else if(attributeValue == "StartDownload"){
					cout<<"A StartDownload Request has been made\n";
					StartDownload(node);
				}else if(attributeValue == "FetchAllChannel"){
					cout<<"A FetchAllChannel Request has been made\n";
					FetchAllChannel(node);
				}else if(attributeValue == "FetchAllCGroup"){
					cout<<"A FetchAllCGroup Request has been made\n";
					FetchAllCGroup(node);
				}else if(attributeValue == "FetchAllChannelCGroupRelation"){
					cout<<"A FetchAllChannelCGroupRelation Request has been made\n";
					FetchAllChannelCGroupRelation(node);
				}else if(attributeValue == "UpdateChannel"){
					cout<<"A UpdateChannel Request has been made\n";
					UpdateChannel(node,fileContents);
				}else if(attributeValue == "EditUGroup"){
					cout<<"A EditUGroup Request has been made\n";
					EditUGroup(node);
				}else if(attributeValue == "EditCGroup"){
					cout<<"A EditCGroup Request has been made\n";
					EditCGroup(node);
				}else if (attributeValue == "FetchUGroupCGroupRelation"){
					cout<<"\nA FetchUGroupCGroupRelation Request has been made\n";
					FetchUGroupCGroupRelation(node);
				}
				else if ( attributeValue == "UGroupCGroupRelation"){
					cout<<"A UGroupCGroupRelation Request has been made\n";
					UGroupCGroupRelation(node);
				}else{
					cout<<"An unknown attribute Value " <<attributeValue<<"  in the attribute :"<<attributeName<<endl;
				}
			}else{
				cout<<"An unknown attribute in the node :"<<nodeName<<"  with attribute name: "<<attributeName<<endl;
			}
			attribute=attribute->next_attribute();
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in Action Function"<<e.what()<<endl;
	}
	return false;
}

bool XMLParser::XMLDakshaImaging(xml_node<>* node, char * fileContents, int &interactionValue )
{
	try{
		string nodeName;
		string attributeName;
		if(!node){
			cout<<"empty node passed into the XMLDakshaImaging method\n";
			return false;
		}
		nodeName = node->name();
		if(nodeName!="XMLDakshaImaging"){
			cout<<"wrong Node:  "<<nodeName<<" passed as argument to XMLDakshaImaging Method\n";
			return false;
		}
		node = node->first_node();
		interactionValue = 0;
		// While is used to run the parser on each node/Block
		while(node){
			nodeName = node->name();
			if(nodeName=="Action"){
				cout<<"An action request has been made from the node:"<<nodeName<<endl;
				Action(node, fileContents);
			}else if(nodeName == "Response"){
				cout<<"An Response request has been made from the node:"<<nodeName<<endl;
				Response(node);
			}else if(nodeName == "Interaction"){
				cout<<"An Interaction request has been made from the node:"<<nodeName<<endl;
				Interactions(node);
				interactionValue = 1;
			}
			else if(nodeName == "InteractionResponse"){
				cout<<"An InteractionResponse request has been made from the node:"<<nodeName<<endl;
				InteractionResponse(node);
				interactionValue = 1;
			}
			else{
				cout<<"An unknown node exists in the node: "<<node->parent()->name()<<"  with name: "<<nodeName<<endl;
			}
			node = node->next_sibling();
		}
		return true;
	}
	catch (exception &e){
		cout<<"Exception caught in XMLDakshaImaging Function"<<e.what()<<endl;
	}
	return false;
}


void XMLParser::parse()
{
	try{
			if ( isAndroid )
				ParserForAndroid();
			else
				ParserForDesktop();
			//delete XMLCreatorObj;
}
	catch (exception &e){
		cout<<"Exception caught in main Function. Possible Reason ( Improper XML format)"<<e.what()<<endl<<endl;
	}

}


void XMLParser::ParserForAndroid()
{
	try{
		rapidxml::xml_document<> doc;
		int temp = 0;
				while(1){
					cout<<"\nIn side ParserForAndroid \n";
					XMLCreatorObj->ReInitializeXML();

					char buffer[256];
					recv(sockfd,buffer,sizeof(int),0);
					int xmlSize = atoi(buffer);
					if ( xmlSize == 0 )
							return;
					cout<<"\nBuffer--->"<<buffer;
					cout<<"xml="<<xmlSize<<endl;
					char tempfileContents[100000];
					char fileContents[10000];
					recv(sockfd,fileContents,xmlSize,0);
					doc.parse<0> (fileContents);

					xml_node<>* currNode = doc.first_node();
					string nodeName;
					nodeName = currNode->name();
					if(nodeName == "XMLDakshaImaging"){

						if(!XMLDakshaImaging(currNode,tempfileContents,temp )){
							cout<<"Error Parsing the XML";
						}
					}else{
						cout<<"Not a Valid XML.Exiting\n";
					}


					if(XMLCreatorObj!=NULL){
						string xml_string=XMLCreatorObj->NewconvertToString();

						int size = xml_string.size();
						sendData(4,&size,sockfd);
						cout<<"\n\nSending size="<<size<<endl;
						if(size<100000){
						sendData(size,xml_string.c_str(),sockfd);
						//	cout<<"string="<<xml_string<<endl;
						}
					}
					else
						cout<<"XMLCreatorObj is NULL\n";
	}
	}
	catch (exception &e){
		cout<<"Exception caught in main Function. Possible Reason ( Improper XML format)"<<e.what()<<endl<<endl;
	}

}


void XMLParser::ParserForDesktop()
{
	try{
		rapidxml::xml_document<> doc;
		//string fileContents;
		int interactionValue = 0;
		while(1){
		//	cout<<"\nIn side ParserForDesktop \n";

			XMLCreatorObj->ReInitializeXML();
			int xmlSize=0;
			if(!recvData(&xmlSize,sizeof(int))){
				return;
			}
			if ( xmlSize == 0 )
				return;
			cout<<"\n received size -->"<<xmlSize<<endl;
			char * fileContents = (char*)malloc(sizeof(char)*xmlSize);
			char tempfileContents[100000];
			if(!recvData(fileContents,xmlSize)){
				return;
			}
			sprintf(tempfileContents,"%s %d", fileContents, strlen(fileContents));
			cout<<"\nReceived xml --->"<<fileContents<< " Length " <<strlen(fileContents)<<endl;
			doc.parse<0> (fileContents);
			xml_node<>* currNode = doc.first_node();
			string nodeName;
			nodeName = currNode->name();
			if(nodeName == "XMLDakshaImaging"){
			//	cout<<"Valid XML to Process \n";
				if(!XMLDakshaImaging(currNode, tempfileContents,interactionValue)){
					cout<<"Error Parsing the XML";
				}
			}else{
				cout<<"Not a Valid XML.Exiting\n";
			}


			if(XMLCreatorObj!=NULL){
				string xml_string = XMLCreatorObj->NewconvertToString();

				int size = xml_string.size();
				cout<<"\n\nSending size="<<size<<endl;
				sendData(4,&size,sockfd);

				sendData(size,xml_string.c_str(),sockfd);

				//cout<<"\nResponed xml --->"<<xml_string.c_str()<<endl;

			}
			else
				cout<<"XMLCreatorObj is NULL\n";

			if(fileContents!=NULL)
				free(fileContents);

			if ( interactionValue == 1 )
				break;

		}
	}
	catch (exception &e){
		cout<<"Exception caught in main Function. Possible Reason ( Improper XML format )"<<e.what()<<endl<<endl;
	}

}


bool XMLParser::recvData(void* data,int size, int sockFD)
{
	int temp = 0, ret = 0;
	ret = recv(sockFD, data, size, 0);
	cout<<"\nret"<<ret <<"\tsize"<<size;
	cout<<"\n";
	if ( ret >= 0 ){
		while ( ret != size ){
			temp = recv(sockFD, ( data + ret ), size, 0);
			if ( temp < 0 )
				break;
			ret += temp;
			if ( ret == size )
				break;
		}
	}
	if(ret>0 && ret!=size){
		cout<<"Error: Incomplete data received\n";
		cout<<"Closing the socket.\n";

		return false;
	}
	if(ret<0){
		switch(errno){
		case EAGAIN: cout<<"Receive Timeout\n";
		break;
		case ECONNRESET: cout<<"Connection forcibly closed by peer\n";
		break;
		default:
			printf("Failed to receive the file.\n");
			cout<<strerror(errno)<<endl;
			break;
		}
		cout<<"Closing the socket.\n";

		return false;
	}
	return true;

}
bool XMLParser::recvData(void* data,int size)
{
	int temp = 0, ret = 0;
	ret = recv(sockfd, data, size, 0);
	//cout<<"\nret"<<ret <<"\tsize"<<size;
	//cout<<"\n";pa
	if ( ret >= 0 ){
		while ( ret != size ){
			temp = recv(sockfd, ( data + ret ), size, 0);
			if ( temp < 0 )
				break;
			ret += temp;
			if ( ret == size )
				break;
		}
	}
	if(ret>0 && ret!=size){
		cout<<"Error: Incomplete data received\n";
		cout<<"Closing the socket.\n";

		return false;
	}
	if(ret<0){
		switch(errno){
		case EAGAIN: cout<<"Receive Timeout\n";
		break;
		case ECONNRESET: cout<<"Connection forcibly closed by peer\n";
		break;
		default:
			printf("Failed to receive the file.\n");
			cout<<strerror(errno)<<endl;
			break;
		}
		cout<<"Closing the socket.\n";

		return false;
	}
	return true;

}

bool XMLParser::sendData(int size,const void *data,int fd)
{
	int temp = 0, ret = 0;
	do{
		ret = send(fd,data,size,0);
		size = size - ret;
		data = data+ret;
	}while(size!=0);
	if(ret<=0){
		switch(errno){
		case EAGAIN: cout<<"\nSend Timeout\n";
		break;
		case ECONNRESET: cout<<"\nConnection forcibly closed by peer:";
		break;
		default:
			printf("\nFailed to send the file. Equal to zero. Closing the socket.\n");
			cout<<strerror(errno)<<endl;
			break;
		}
		cout<<"\nClosing the socket and deleting the entry:\n";
		close(fd);
		return false;
	}

	return true;

}

bool XMLParser::EditCGroup( xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the EditCGroup method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value Login
		if(0){
			cout<<"EditCGroup Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","EditCGroup");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","EditCGroup");
		}else{
			xml_node<>* CGroupInfo = getNodeByName("CGroupInfo",node);
			if ( CGroupInfo == NULL ){
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","EditCGroup");
				return true;
			}
			xml_node<>* CGID = getNodeByName("CGID",CGroupInfo);
			xml_node<>* AddChannel = getNodeByName("AddChannel",CGroupInfo);

			if ( CGID == NULL ){
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","301","EditCGroup");
				return true;
			}
			vector<int> addCID;
			if ( AddChannel != NULL ){
				xml_node<>* CID = getNodeByName("CID",AddChannel);
				while(CID != NULL){
					cout<<"\nAdd Channel To Group\n";
					addCID.push_back(atoi(CID->value()));
					CID=CID->next_sibling("CID");
					//AddChannel= AddChannel->next_sibling("AddChannel");
				}
			}
			xml_node<>* RemoveChannel = getNodeByName("RemoveChannel",CGroupInfo);
			vector<int> removeCID;
			if ( RemoveChannel != NULL ){
				xml_node<>* CID = getNodeByName("CID",RemoveChannel);
				while(CID!=NULL){
					cout<<"\nRemove Channel from Group\n";
					removeCID.push_back(atoi(CID->value()));
					CID=CID->next_sibling("CID");
					//RemoveChannel= RemoveChannel->next_sibling("RemoveUser");
				}
			}
			int DID = -1;
			DID=DataBaseAccessObj->GetSystemDID();
			if ( DataBaseAccessObj->AddChannelToCGID(atoi(CGID->value()), addCID,DID,LoggedUID) ){
				if ( DataBaseAccessObj->RemoveChannelFromCGID(atoi(CGID->value()), removeCID,LoggedUID)){
					XMLCreatorObj->NewSuccessAction("EditCGroup","ok");
				}
				else
					XMLCreatorObj->NewFailureAction("Unable to Remove User To UserGroup","112","EditCGroup");
			}
			else
				XMLCreatorObj->NewFailureAction("Unable to Add User To UserGroup","113","EditCGroup");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in EditCGroup Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::EditUGroup(xml_node<>* node){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the EditUGroup method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value Login
		if(0){
			cout<<"EditUGroup Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","EditUGroup");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","EditUGroup");
		}else{
			xml_node<>* UGroupInfo = getNodeByName("UGroupInfo",node);
			if ( UGroupInfo == NULL ){
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","EditUGroup");
				return true;
			}
			xml_node<>* UGID = getNodeByName("UGID",UGroupInfo);
			xml_node<>* AddUser = getNodeByName("AddUser",UGroupInfo);
			if ( UGID == NULL  ){
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","301","EditUGroup");
				return true;
			}
			vector<int> addUID;
			if (AddUser != NULL ){
				xml_node<>* UID = getNodeByName("UID",AddUser);
				while(UID!=NULL){
					cout<<"1\n";
					addUID.push_back(atoi(UID->value()));
					UID=UID->next_sibling("UID");
				}
			}

			xml_node<>* RemoveUser = getNodeByName("RemoveUser",UGroupInfo);
			vector<int> removeUID;
			if ( RemoveUser != NULL  ){
				xml_node<>* UID = getNodeByName("UID",RemoveUser);
				while(UID!=NULL){
					cout<<"\nRemove User From group\n";
					removeUID.push_back(atoi(UID->value()));
					UID=UID->next_sibling("UID");
					//RemoveUser= RemoveUser->next_sibling("AddUser");
				}
			}
			else{
				cout<<"\nRemoveUser is NULL\n";
			}
			int DID = -1;
			DID=DataBaseAccessObj->GetSystemDID();
			if ( DataBaseAccessObj->AddUserToUGID(atoi(UGID->value()), addUID,DID) ){
				if ( DataBaseAccessObj->RemoveUserFromUGID(atoi(UGID->value()), removeUID)){
					XMLCreatorObj->NewSuccessAction("EditUGroup","ok");
				}
				else
					XMLCreatorObj->NewFailureAction("Unable to Remove User To UserGroup","112","EditUGroup");
			}
			else
				XMLCreatorObj->NewFailureAction("Unable to Add User To UserGroup","113","EditUGroup");
		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in EditUGroup Function"<<e.what()<<endl;
	}
	return false;

}

bool XMLParser::GetAllCGroupRelationInfo(int &CGIDInfo,xml_node<>* CGroupRelation,vector<int> &addUGID,vector<int> &removeUGID){

	xml_node<>* CGroupInfo = getNodeByName("CGroupInfo",CGroupRelation);
	if ( CGroupInfo != NULL ){
		xml_node<>* CGID = getNodeByName("CGID",CGroupInfo);
		if ( CGID != NULL ){
			CGIDInfo = atoi(CGID->value());
			xml_node<>* AddUGroup = getNodeByName("AddUGroup",CGroupInfo);
			if (AddUGroup != NULL ){
				xml_node<>* UGID = getNodeByName("UGID",AddUGroup);
				while(UGID!=NULL){
					addUGID.push_back(atoi(UGID->value()));
					UGID = UGID->next_sibling("UGID");
				}
			}
			xml_node<>* RemoveUGroup = getNodeByName("RemoveUGroup",CGroupInfo);
			if (RemoveUGroup != NULL ){
				xml_node<>* UGID = getNodeByName("UGID",RemoveUGroup);
				while(UGID!=NULL){
					removeUGID.push_back(atoi(UGID->value()));
					UGID = UGID->next_sibling("UGID");
				}
			}
		}
		else
			return false;
	}else
		return false;
	return true;

}


bool XMLParser::GetAllUGroupRelationInfo(int &UGIDInfo, xml_node<>* UGroupRelation,vector<int> &addCGID,vector<int> &removeCGID){

	cout<<"\nGetAllUGroupRelationInfo"<<endl;
	xml_node<>* UGroupInfo = getNodeByName("UGroupInfo",UGroupRelation);
	if ( UGroupInfo != NULL ){
		xml_node<>* UGID = getNodeByName("UGID",UGroupInfo);
		if ( UGID != NULL ){
			UGIDInfo = atoi(UGID->value());
			xml_node<>* AddCGroup = getNodeByName("AddCGroup",UGroupInfo);
			if (AddCGroup != NULL ){
				xml_node<>* CGID = getNodeByName("CGID",AddCGroup);
				for ( ; CGID && (CGID != NULL); CGID = CGID->next_sibling("CGID") ){
					addCGID.push_back(atoi(CGID->value()));
				}
			}
			xml_node<>* RemoveCGroup = getNodeByName("RemoveCGroup",UGroupInfo);
			if (RemoveCGroup != NULL ){
				//cout<<"\n1122"<<endl;
				xml_node<>* CGID = getNodeByName("CGID",RemoveCGroup);
				//cout<<"\n111222"<<endl;
				while(CGID!=NULL){
					//cout<<"\n1111"<<endl;
					removeCGID.push_back(atoi(CGID->value()));
					CGID = CGID->next_sibling("CGID");
				}
			}
		}
		else
			return false;
	}
	else
		return false;
	return true;
}

bool XMLParser::UGroupCGroupRelation( xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the UGroupCGroupRelation method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value Login
		if(0){
			cout<<"UGroupCGroupRelation Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","UGroupCGroupRelation");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","UGroupCGroupRelation");
		}else{
			bool result = false;
			int sysDID = DataBaseAccessObj->GetSystemDID();
			xml_node<>* UGroupRelation = getNodeByName("UGroupRelation",node);
			xml_node<>* CGroupRelation = getNodeByName("CGroupRelation",node);
			xml_node<>* UIDInfo = getNodeByName("UID",node);

			if ( UGroupRelation != NULL ){
				int UGID = 0,UID;
			if ( UIDInfo != NULL )
				UID = atoi(UIDInfo->value());
			else
				UID = LoggedUID;
				vector<int>addCGID, removeCGID;
				if ( GetAllUGroupRelationInfo(UGID,UGroupRelation,addCGID,removeCGID) )
					if ( DataBaseAccessObj->InsertUGroupRelationWithCGroup(UGID,addCGID,removeCGID,UID,sysDID))
						result = true;
			}
			else if ( CGroupRelation != NULL ){
				int CGID= -1;
				vector<int>addUGID, removeUGID;
				if (GetAllCGroupRelationInfo(CGID,CGroupRelation,addUGID,removeUGID) )
					if ( DataBaseAccessObj->InsertCGroupRelationWithUGroup(CGID,addUGID,removeUGID,LoggedUID,sysDID) )
						result = true;

			}
			else
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","UGroupCGroupRelation");

			if (result){
				XMLCreatorObj->NewSuccessAction("UGroupCGroupRelation","Ok");
				cout<<"\n"<<XMLCreatorObj->NewconvertToString();
			}
			else
				XMLCreatorObj->NewFailureAction("Unable to Process","402","UGroupCGroupRelation");


		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in EditCGroup Function"<<e.what()<<endl;
	}
	return false;

}
/*

bool XMLParser::UGroupCGroupRelation( xml_node<>* node ){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the EditCGroup method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value Login
		if(0){
			cout<<"UGroupCGroupRelation Failed\n";
			XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","UGroupCGroupRelation");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
/*		XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","UGroupCGroupRelation");
		}else{
			bool result = false;
			xml_node<>* UGroupRelation = getNodeByName("UGroupRelation",node);
			xml_node<>* CGroupRelation = getNodeByName("CGroupRelation",node);
			if ( UGroupRelation != NULL ){
				int UGID = 0;
				vector<int>addCGID, removeCGID;
				if ( GetAllUGroupRelationInfo(UGID,UGroupRelation,addCGID,removeCGID) )
					if ( DataBaseAccessObj->InsertUGroupRelationWithCGroup(UGID,addCGID,removeCGID,LoggedUID,LoggedDID))
						result = true;
			}
			else if ( CGroupRelation != NULL ){
				int CGID= -1;
				vector<int>addUGID, removeUGID;
				if (GetAllCGroupRelationInfo(CGID,CGroupRelation,addUGID,removeUGID) )
					if ( DataBaseAccessObj->InsertCGroupRelationWithUGroup(CGID,addUGID,removeUGID,LoggedUID,LoggedDID) )
						result = true;

			}
			else
				XMLCreatorObj->NewFailureAction("The information you entered is NULL","101","UGroupCGroupRelation");

			if (result)
				XMLCreatorObj->NewSuccessAction("UGroupCGroupRelation","Ok");
			else
			  XMLCreatorObj->NewFailureAction("Unable to Process","402","UGroupCGroupRelation");


		}
		return true;
	}catch (exception &e){
		cout<<"Exception caught in EditCGroup Function"<<e.what()<<endl;
	}
	return false;

}
 */
bool XMLParser::FetchUGroupCGroupRelation(xml_node<>* node){
	try{
		string nodeName;
		string attributeName;
		if(!node){
			throw runtime_error("empty node passed into the Action method");
		}
		nodeName = node->name();
		if(nodeName!="Action"){
			throw runtime_error("wrong Node passed as argument to Action Method");
		}
		// Needs to cross check if the  attribute name has value AddUser

		if(0){
			cout<<"FetchAllUser FAiled\n";
			XMLCreatorObj->NewFailureAction("The you entered is NULL","101","FetchUGroupCGroupRelation");
		}else if(0){   /*check if the action can be performed by the user*/ /* Currently disabled in this function */
			XMLCreatorObj->NewFailureAction("Permission denied to perform this Action","102","FetchUGroupCGroupRelation");
		}else{

			XMLCreatorObj->NewFetchUGroupCGroupRelation(true );
		}

		return true;
	}catch (exception &e){
		cout<<"Exception caught in FetchChaFetchAllUsernnel Function"<<e.what()<<endl;
	}
	return false;
}

void XMLParser::CloseUserSession(){
	XMLCreatorObj->CloseUserSession();
}



