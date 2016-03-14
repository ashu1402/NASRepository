/*
 * NotificationInitiate.h
 *
 *  Created on: 10-Feb-2016
 *      Author: ubuntu
 */

#ifndef NOTIFICATIONINITIATE_H_
#define NOTIFICATIONINITIATE_H_

#include "CommonClass.h"
#include "Markup.h"


class NotificationIntiate{
	public:
	CMarkup *xml;
	CommonClass * commFunc;
	public:
	NotificationIntiate( char * msg , char * channelName ,char * serverName, char * serverIP, int portNumber, char * DID , char * CID );
	~NotificationIntiate();
	void CreateXML( char * xmlData , char * msg , char * channelName , char * serverName, char * DID , char * CID );
	string ConvertXmlIntoString();
	void Delete();
};


#endif /* NOTIFICATIONINITIATE_H_ */
