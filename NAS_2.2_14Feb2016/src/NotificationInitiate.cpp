/*
 * NotificationInitiate.cpp
 *
 *  Created on: 10-Feb-2016
 *      Author: ubuntu
 */

#include "NotificationInitiate.h"


NotificationIntiate::NotificationIntiate( char * msg , char * channelName ,char * serverName, char * serverIP, int portNumber, char * DID , char * CID ){

	char xmlData[SIZEOFCHARARRAYBIGGER];
	commFunc = new CommonClass();
	CreateXML( xmlData, msg , channelName, serverName,DID,CID );
	if ( commFunc->CreateConnectionClient(serverIP, portNumber,xmlData ) ){
		cout<<"\nSuccessFully Send\n";
	}
	Delete();

}

void NotificationIntiate::CreateXML( char * xmlData , char * msg , char * channelName , char * serverName, char * DID , char * CID ){

	xml = new CMarkup();
	xml->AddElem( "XMLDakshaImaging" );
	xml->IntoElem();
	xml->AddElem( "Action" );
	xml->AddAttrib("Name", "Notification");
	xml->IntoElem();
	xml->AddElem("Msg", msg);
	xml->AddElem("ChannelName", channelName);
	xml->AddElem("ServerName", serverName);
	xml->AddElem("CID", channelName);
	xml->AddElem("DID", DID);
	xml->OutOfElem();

	string xmlDocument = ConvertXmlIntoString();
	sprintf( xmlData,"%s", xmlDocument.c_str());
}

string NotificationIntiate::ConvertXmlIntoString()
{
	return xml->GetDoc();
}



void NotificationIntiate::Delete(){
	delete this;
}

NotificationIntiate::~NotificationIntiate(){
	cout<<"\n";
	if ( xml != NULL )
		delete xml;

}




