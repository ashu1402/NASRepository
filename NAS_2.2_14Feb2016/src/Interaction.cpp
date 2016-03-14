/*
 * Interaction.cpp
 *
 *  Created on: 14-Dec-2015
 *      Author: ubuntu
 */
#include "Interaction.h"
#include "XMLParser.h"
#include "XMLCreator.h"

Interaction::Interaction( Failover * failoverObj){
	isBreak = false;
	failover = failoverObj;
}

void *Interaction::StartThread( void * para ){

	Interaction * interaction = (Interaction *)( para);
	interaction->dBA = new DataBaseAccess();
	cout<<"\nCreating interaction Main DBA\n";
	vector<string> nasIP;
	int sysDID = interaction->dBA->GetSystemDID();
	interaction->dBA->Close();
	delete interaction->dBA;
	cout<<"\nClose interaction Main DBA\n";
	XMLCreator * xmlCreator = new XMLCreator();
	char xmlData[200000];
	CommonClass * commFunc = new CommonClass();
	while (1){



		if ( interaction->failover->isBreak ){
			cout<<"\nCreating interaction DBA\n";
			interaction->dBA = new DataBaseAccess();
			if ( interaction->isBreak ){
				interaction->dBA->Close();
				delete interaction->dBA;
				break;
			}
			if( nasIP.size() > 0 )
				nasIP.clear();
			interaction->dBA->GetAllAvailableNASDetail(nasIP);
			cout<<"\nClose interaction DBA\n";
			interaction->dBA->Close();
			delete interaction->dBA;
			if ( nasIP.size() > 0 ){
				xmlCreator->ReInitializeXML();

				xmlCreator->CreateInteractionXml();
				string xmlInfo = xmlCreator->NewconvertToString();
				sprintf(xmlData,"%s\0", xmlCreator->NewconvertToString().c_str());

				for ( int i = 0 ; i < (int)nasIP.size(); i++ ){
					commFunc->CreateConnectionClient( (char*)nasIP[i].c_str(),tcpSocketPortNumber,xmlData);
					cout<<"\nDone Completely\n";
				}
				cout<<"\nNAS Available--->"<<nasIP.size();
			}
		}



		sleep(120);

	}
	cout<<"\nCompleted"<<endl;
	interaction = NULL;
	delete commFunc;
	return NULL;
}

void Interaction::SendInstanceInteraction(char * xmlData ){
	DataBaseAccess * dBAObj = new DataBaseAccess();
	vector<string> nasIP;
	dBAObj->GetAllAvailableNASDetail(nasIP);
	CommonClass * commFunc = new CommonClass();
	for ( int i = 0 ; i < (int)nasIP.size(); i++ ){
		commFunc->CreateConnectionClient( (char*)nasIP[i].c_str(),tcpSocketPortNumber,xmlData);
		cout<<"\nDone Completely\n";
	}
	delete commFunc;
	dBAObj->Close();
	delete dBAObj;
}


xml_node<>* Interaction::getNodeByName(const string& nodeName, xml_node<>* parentNode)
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

bool Interaction::ParserInteractionResponse( char * receData ){
	rapidxml::xml_document<> doc;
	doc.parse<0>(receData);
	xml_node<>* currNode = doc.first_node();
	string nodeName;
	nodeName = currNode->name();
	string attributeName;
	if(!currNode){
		cout<<"empty node passed into the XMLDakshaImaging method\n";
		return false;
	}
	nodeName = currNode->name();
	if(nodeName!="XMLDakshaImaging"){
		cout<<"wrong Node:  "<<nodeName<<" passed as argument to XMLDakshaImaging Method\n";
		return false;
	}
	currNode = currNode->first_node();
	nodeName = currNode->name();
	if(nodeName == "InteractionResponse"){
		cout<<"An Response request has been made from the node:"<<nodeName<<endl;
		cout<<"\nGOOD\n";
		return true;
	}
	return false;
}

void Interaction::InitializeThread( Interaction * interactionOBj ){


	if ( pthread_create( &interactionThread, NULL, StartThread, (void *)interactionOBj ) != 0){
		if ( ISDEBUG )
			cout<<"\n Interaction Thread is not able to create"<<endl;
	}
	else{
		if ( ISDEBUG )
			cout<<"\n Interaction Thread created"<<endl;
	}
}
