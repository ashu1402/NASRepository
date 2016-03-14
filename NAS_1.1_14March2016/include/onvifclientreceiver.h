#ifndef __ONVIF_CLIENT_RECEIVER__
#define __ONVIF_CLIENT_RECEIVER__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "onvifclientdevice.h"
#include "soapStub.h"
#include "soapReceiverBindingProxy.h"
#include "wsseapi.h"


using namespace std;

class OnvifClientReceiver
{
public:
    OnvifClientReceiver(OnvifClientDevice &device);
    ~OnvifClientReceiver();
public:
	//Dealing with receivers
	int GetReceivers(_trv__GetReceiversResponse & receivers);
	int GetReceiver(_trv__GetReceiverResponse &receiverResponsem, string ReceiverToken);
	int CreateReceiver(_trv__CreateReceiverResponse &CreateReceiverResponse, tt__ReceiverConfiguration &ReceiverConfiguration);
	int DeleteReceiver(_trv__DeleteReceiverResponse &DeleteReceiverResponse, string ReceiverToken);
	//receiver status functions
	int ConfigureReceiver(_trv__ConfigureReceiverResponse &ConfigureReceiverResponse, string ReceiverToken,tt__ReceiverConfiguration &ReceiverConfiguration);
	int SetReceiverMode(_trv__SetReceiverModeResponse &SetReceiverModeResponse, string ReceiverToken,tt__ReceiverMode &ReceiverConfiguration);
	int GetReceiverState(_trv__GetReceiverStateResponse &GetReceiverStateResponse, string ReceiverToken);
	int GetServiceCapabilities(_trv__GetServiceCapabilitiesResponse &GetReceiverStateResponse);

private:
	OnvifClientDevice &m_Device;
	ReceiverBindingProxy  receiverProxy;

};

#endif
