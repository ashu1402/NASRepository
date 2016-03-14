#include "onvifclientevent.h"
#include "iostream"
using namespace std;

OnvifClientEvent::OnvifClientEvent(OnvifClientDevice &device)
: m_Device(device), eventBinding(SOAP_C_UTFSTRING)
{

}

OnvifClientEvent::~OnvifClientEvent()
{

}

int OnvifClientEvent::Subscribe(string &notifyUrl)
{
	string strUrl;
	string strUser;
	string strPass;
	_wsnt__Subscribe subscribe;
	_wsnt__SubscribeResponse  subscribeResponse;
	string strTimeOut = "PT60S";
	
	if (m_Device.GetUserPasswd(strUser, strPass) == false 
		|| m_Device.GetEventUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	
	subscribe.ConsumerReference.Address = (char *)(notifyUrl.c_str());
	subscribe.InitialTerminationTime = &strTimeOut;

	/*wsnt__FilterType *Filter = SOAP_NEW(wsnt__FilterType);
	Filter->__item = "boolean(//tt:SimpleItem[@Name='IsMotion'])";
	Filter->__any.push_back("boolean(//tt:SimpleItem[@Name='IsMotion'])");

	//= "boolean(//tt:SimpleItem[@Name=”VideoSourceConfigurationToken” and @Value=”1”]) "
	subscribe.Filter = Filter;
	*/

	eventBinding.soap_endpoint =  strUrl.c_str();
	
	soap_wsse_add_Security(&eventBinding);
	soap_wsse_add_UsernameTokenDigest(&eventBinding, "Id", 
		strUser.c_str() , strPass.c_str());
	soap_wsa_add_From(&eventBinding, "http://www.w3.org/2005/08/addressing/anonymous");
	soap_wsa_add_ReplyTo(&eventBinding, "http://www.w3.org/2005/08/addressing/anonymous");
	soap_wsa_add_FaultTo(&eventBinding, "http://www.w3.org/2005/08/addressing/anonymous");

		
	int ret = eventBinding.Subscribe(&subscribe,subscribeResponse);
	if (ret == SOAP_OK)
	{
		m_RenewEndpoint = subscribeResponse.SubscriptionReference.Address;
		printf("SubscriptionReference %s\n", m_RenewEndpoint.c_str());
	}
	return ret;
}

int OnvifClientEvent::UnSubscribe()
{
	string strUrl;
	string strUser;
	string strPass;
	_wsnt__Unsubscribe unsubscribe;
	_wsnt__UnsubscribeResponse  unsubscribeResponse;
	
	if (m_Device.GetUserPasswd(strUser, strPass) == false 
		|| m_Device.GetEventUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	
	eventBinding.soap_endpoint =  strUrl.c_str();
	
	soap_wsse_add_Security(&eventBinding);
	soap_wsse_add_UsernameTokenDigest(&eventBinding, "Id", 
		strUser.c_str() , strPass.c_str());
	soap_wsa_add_From(&eventBinding, "http://www.w3.org/2005/08/addressing/anonymous");
	soap_wsa_add_ReplyTo(&eventBinding, "http://www.w3.org/2005/08/addressing/anonymous");
	soap_wsa_add_FaultTo(&eventBinding, "http://www.w3.org/2005/08/addressing/anonymous");
	eventBinding.header->wsa5__To = const_cast<char *>(m_RenewEndpoint.c_str());
		
	return eventBinding.Unsubscribe(&unsubscribe,unsubscribeResponse);

}
int OnvifClientEvent::Renew()
{
	string strUrl;
	string strUser;
	string strPass;
	_wsnt__Renew req;
	_wsnt__RenewResponse  resp;
	string strTimeOut = "PT60S";
	
	if (m_Device.GetUserPasswd(strUser, strPass) == false 
		|| m_Device.GetEventUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	
	req.TerminationTime = &strTimeOut;
	
	eventBinding.soap_endpoint =  strUrl.c_str();
	
	soap_wsse_add_Security(&eventBinding);
	soap_wsse_add_UsernameTokenDigest(&eventBinding, "Id", 
		strUser.c_str() , strPass.c_str());
	//soap_wsa_add_From(&eventBinding, "http://www.w3.org/2005/08/addressing/anonymous");
	soap_wsa_add_ReplyTo(&eventBinding, "http://www.w3.org/2005/08/addressing/anonymous");
	//soap_wsa_add_FaultTo(&eventBinding, "http://www.w3.org/2005/08/addressing/anonymous");
	eventBinding.header->wsa5__To = const_cast<char *>(m_RenewEndpoint.c_str());

		
	int ret = eventBinding.Renew(&req,resp);
	return ret;
}


/*xml_node<>* OnvifClientEventNotify::getNodeByName(const string& nodeName, xml_node<>* parentNode)

	{
		try{
			if(parentNode == NULL)
			{
				return NULL;
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
*/
int OnvifClientEventNotify::Notify(_wsnt__Notify *wsnt__Notify)
	{
		string message;
		//printf("\nMessage size %d\n", wsnt__Notify->NotificationMessage.size());
		/*if(wsnt__Notify->NotificationMessage.size() > 0 )
		{
			m_EventCnt ++;
		//	printf("\nONVIF CPP Lib Event %d host %s __mixed: %s\n", m_EventCnt, host, wsnt__Notify->NotificationMessage[0]->Topic->__mixed);
		//	printf("\t__any: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->__any);
		//	printf("\t__anyAttribute: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->__anyAttribute);
		//	printf("\tDialect: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->Dialect.c_str());
		//	printf("\tMessage %s\n", wsnt__Notify->NotificationMessage[0]->Message.__any);
		//  mes=wsnt__Notify->NotificationMessage[0]->Message.__any;
			string message=wsnt__Notify->NotificationMessage[0]->Message.__any;
			XMLparser XMLParserObj;


			rapidxml::xml_document<> doc;
			doc.parse<0> ((char*)message.c_str());
			xml_node<>* currNode = doc.first_node();
			if ( currNode != NULL )
			{
			string nodeName = currNode->name();
			//cout<<nodeName<<"\n";
			xml_node<>* dataNode = XMLParserObj.getNodeByName("tt:Data",currNode);
				if(dataNode!=NULL)
				{
						xml_node<>* simpleNode = XMLParserObj.getNodeByName("tt:SimpleItem",dataNode);
							if(simpleNode!=NULL)
							{
								xml_attribute<>* attribute1 =simpleNode->first_attribute("Name");
								xml_attribute<>* attribute2 =simpleNode->first_attribute("Value");
								if(strcmp(attribute1->value(),"IsMotion")==0 && strcmp(attribute2->value(),"true")==0)
									cout<<"-----> MOTION STARTED <----- "<<"\n\n";
								else if(strcmp(attribute1->value(),"IsMotion")==0 && strcmp(attribute2->value(),"false")==0)
									cout<<"-----> MOTION STOPPED <----- "<<"\n\n";

								//xml_attribute<>* attribute1 = simpleNode->first_attribute();
								//xml_attribute<>* attribute2 = simpleNode->first_attribute()->next_attribute();

								//if(strcmp(attribute1->value(),"IsMotion")==0 && strcmp(attribute2->value(),"true")==0)
									//cout<<"MOTION STARTED "<<"\n";
								//else if(strcmp(attribute1->value(),"IsMotion")==0 && strcmp(attribute2->value(),"false")==0)
									//cout<<"MOTION STOPPED "<<"\n";

								//cout<<"\nStatus Name--->"<<attribute1->value()<<endl;
								//cout<<"\nStatus Value--->"<<attribute2->value()<<endl;
							}
				}
			}

		 }
*/
			return SOAP_OK;
	}
