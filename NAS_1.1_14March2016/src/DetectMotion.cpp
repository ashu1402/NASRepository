#include "DetectMotion.h".h"


DetectMotion::DetectMotion()
{
	cout<<"Detect Motion Function Called "<<"\n"<<"\n";
}
DetectMotion::~DetectMotion()
{
}

int DetectMotion::GetMotion(string Notify,OnvifClientDevice &onvifDevice)
{
			OnvifClientDevice *onvifDeviceobj;
			onvifDeviceobj=&onvifDevice;

			_tds__GetCapabilitiesResponse capabilitiesResponse;			// calling get capability function is must for detecting motion
			int test1 = onvifDeviceobj->GetCapabilities(capabilitiesResponse);

			int ret=-1;
			string eventNotify = Notify;
			OnvifClientReceiver onvifReceiver(*onvifDeviceobj);
			_trv__GetReceiversResponse receivers;
			onvifReceiver.GetReceivers(receivers);
			for (vector<tt__Receiver * >::iterator iter =
					receivers.Receivers.begin();
					iter < receivers.Receivers.end(); ++iter)
			{
				tt__Receiver * pReceivers = *iter;
				printf("pReceivers %s\n", pReceivers->Token.c_str());
			}

			OnvifClientEvent onvifEvent(*onvifDeviceobj);
			string str;
			onvifDeviceobj->GetMediaUrl(str);
			onvifEvent.Subscribe(eventNotify);

			struct soap* soapObj = soap_new();
			OnvifClientEventNotify notify(soapObj);

		    if ((ret = soap_bind(&notify, NULL, 9090, 100) == SOAP_INVALID_SOCKET ) )
			{
		        printf("OnvifClientEventNotify::soap_bind Binding on %d port failed\n\n",9090);
		        return 0;
		    }
			int timeStart = time(NULL);
			int currentTime = 0;

			//Loop to recevie the event
			while(1)
			{
				currentTime = time(NULL);
				if (currentTime - timeStart > 30)
				{
					onvifEvent.Renew();
				}
				//printf("soap_accept accepting\n");
				if( (ret = soap_accept(&notify)) == SOAP_INVALID_SOCKET)
				{
					printf("soap_accept accepting timeout\n");
					return 0;
				}

		        if ( (soap_begin_serve(&notify)) != SOAP_OK)
		        {
		            printf("soap_begin_serve serve %d failed\n\n", ret);
		            return 0;
		        }

		        ret = notify.dispatch();
				//soap_wchar msg = soap_get(soapObj);
			}

	return 0;

}
