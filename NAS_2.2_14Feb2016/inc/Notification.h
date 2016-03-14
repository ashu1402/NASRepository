/*
 * Notification.h
 *
 *  Created on: 09-Feb-2016
 *      Author: ubuntu
 */

#ifndef NOTIFICATION_H_
#define NOTIFICATION_H_


#include "CommonClass.h"

struct upload_status {
  int lines_read;
};



class Notification{

public:
	Notification();
	void SendMail(char * from ,char * to , char * cc, char *subject, char * containText, char * signature);
	void CreateMail(char * to , char * from , char * cc, char * subject,char * containText, char *signature );
	void SendMailData( char *msg, char *channelName, char *serverName,char * CID,char * DID );
};


#endif /* NOTIFICATION_H_ */
