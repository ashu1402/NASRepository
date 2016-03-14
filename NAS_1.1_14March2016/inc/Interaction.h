/*
 * Interaction.h
 *
 *  Created on: 14-Dec-2015
 *      Author: ubuntu
 */

#ifndef INTRACTION_H_
#define INTRACTION_H_


#include <iostream>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <vector>
#include <fstream>
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>
#include<algorithm>
#include <string.h>
#include <map>
#include <vector>
#include "../rapidxml/rapidxml.hpp"
#include "CommonClass.h"
#include "DataBaseAccess.h"
#include "Failover.h"

using namespace std;
using namespace rapidxml;

class Interaction{

public:
	bool isUpdate;
	pthread_t interactionThread;
	bool isBreak;
	//char *dBIP;
	DataBaseAccess *dBA;
	Failover * failover;
	//XMLParser *xmlParser;

public:
	Interaction(Failover *failoverObj);
	void InitializeThread(  Interaction * interactionOBj );
	static void * StartThread( void * para );
	void SendInstanceInteraction(char * xmlData );
    bool ParserInteractionResponse( char * receData );
	xml_node<>* getNodeByName(const string& nodeName, xml_node<>* parentNode);

};

#endif /* INTRACTION_H_ */
