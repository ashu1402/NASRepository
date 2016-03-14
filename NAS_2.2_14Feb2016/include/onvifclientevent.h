#ifndef __ONVIF_CLIENT_EVENT__
#define __ONVIF_CLIENT_EVENT__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "onvifclientdevice.h"
#include "soapStub.h"
#include "soapPullPointSubscriptionBindingProxy.h"
#include "soapPullPointSubscriptionBindingService.h"
#include "wsseapi.h"
#include "wsaapi.h"
///#include "xmlparser.h"
#include "../rapidxml/rapidxml.hpp"

using namespace std;
//using namespace rapidxml;

class OnvifClientEventNotify : public PullPointSubscriptionBindingService
{
public:
	OnvifClientEventNotify(struct soap* pData)
	: m_EventCnt(0), PullPointSubscriptionBindingService(*pData)
	{

	}
public:
	//TODO add callback to here
	//xml_node<>* getNodeByName(const string& nodeName, xml_node<>* parentNode);
	virtual	int Notify(_wsnt__Notify *wsnt__Notify);

public:
	virtual	PullPointSubscriptionBindingService *copy()
	{
		return NULL;
	}
	/// Web service operation 'PullMessages' (returns error code or SOAP_OK)
	virtual int PullMessages(_tev__PullMessages *tev__PullMessages, _tev__PullMessagesResponse &tev__PullMessagesResponse)
	{return SOAP_OK;}

	/// Web service operation 'Seek' (returns error code or SOAP_OK)
	virtual	int Seek(_tev__Seek *tev__Seek, _tev__SeekResponse &tev__SeekResponse)
	{return SOAP_OK;}

	/// Web service operation 'SetSynchronizationPoint' (returns error code or SOAP_OK)
	virtual	int SetSynchronizationPoint(_tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, _tev__SetSynchronizationPointResponse &tev__SetSynchronizationPointResponse)
	{return SOAP_OK;}

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_tev__GetServiceCapabilities *tev__GetServiceCapabilities, _tev__GetServiceCapabilitiesResponse &tev__GetServiceCapabilitiesResponse)
	{return SOAP_OK;}

	/// Web service operation 'CreatePullPointSubscription' (returns error code or SOAP_OK)
	virtual	int CreatePullPointSubscription(_tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, _tev__CreatePullPointSubscriptionResponse &tev__CreatePullPointSubscriptionResponse)
	{return SOAP_OK;}

	/// Web service operation 'GetEventProperties' (returns error code or SOAP_OK)
	virtual	int GetEventProperties(_tev__GetEventProperties *tev__GetEventProperties, _tev__GetEventPropertiesResponse &tev__GetEventPropertiesResponse)
	{return SOAP_OK;}

	/// Web service operation 'Renew' (returns error code or SOAP_OK)
	virtual	int Renew(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse &wsnt__RenewResponse)
	{return SOAP_OK;}

	/// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
	virtual	int Unsubscribe(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse &wsnt__UnsubscribeResponse)
	{return SOAP_OK;}

	/// Web service operation 'Subscribe' (returns error code or SOAP_OK)
	virtual	int Subscribe(_wsnt__Subscribe *wsnt__Subscribe, _wsnt__SubscribeResponse &wsnt__SubscribeResponse)
	{return SOAP_OK;}

	/// Web service operation 'GetCurrentMessage' (returns error code or SOAP_OK)
	virtual	int GetCurrentMessage(_wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, _wsnt__GetCurrentMessageResponse &wsnt__GetCurrentMessageResponse)
	{return SOAP_OK;}

	/// Web service operation 'GetMessages' (returns error code or SOAP_OK)
	virtual	int GetMessages(_wsnt__GetMessages *wsnt__GetMessages, _wsnt__GetMessagesResponse &wsnt__GetMessagesResponse)
	{return SOAP_OK;}

	/// Web service operation 'DestroyPullPoint' (returns error code or SOAP_OK)
	virtual	int DestroyPullPoint(_wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, _wsnt__DestroyPullPointResponse &wsnt__DestroyPullPointResponse)
	{return SOAP_OK;}

	/// Web service one-way operation 'Notify' (return error code, SOAP_OK (no response), or send_Notify_empty_response())
	virtual	int Notify_(_wsnt__Notify *wsnt__Notify)
	{return SOAP_OK;}

	/// Web service operation 'CreatePullPoint' (returns error code or SOAP_OK)
	virtual	int CreatePullPoint(_wsnt__CreatePullPoint *wsnt__CreatePullPoint, _wsnt__CreatePullPointResponse &wsnt__CreatePullPointResponse)
	{return SOAP_OK;}

	/// Web service operation 'Renew' (returns error code or SOAP_OK)
	virtual	int Renew_(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse &wsnt__RenewResponse)
	{return SOAP_OK;}

	/// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
	virtual	int Unsubscribe_(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse &wsnt__UnsubscribeResponse)
	{return SOAP_OK;}

	/// Web service operation 'PauseSubscription' (returns error code or SOAP_OK)
	virtual	int PauseSubscription(_wsnt__PauseSubscription *wsnt__PauseSubscription, _wsnt__PauseSubscriptionResponse &wsnt__PauseSubscriptionResponse)
	{return SOAP_OK;}

	/// Web service operation 'ResumeSubscription' (returns error code or SOAP_OK)
	virtual	int ResumeSubscription(_wsnt__ResumeSubscription *wsnt__ResumeSubscription, _wsnt__ResumeSubscriptionResponse &wsnt__ResumeSubscriptionResponse)
	{return SOAP_OK;}

private:
	int m_EventCnt;
};

class OnvifClientEvent
{
public:
    OnvifClientEvent(OnvifClientDevice &device);
    ~OnvifClientEvent();
public:
	/* Add function to here */
	int Subscribe(string &notifyUrl);
	int UnSubscribe();
	int Renew();

private:
	OnvifClientDevice &m_Device;
	string m_RenewEndpoint;
	PullPointSubscriptionBindingProxy  eventBinding;

};


#endif
