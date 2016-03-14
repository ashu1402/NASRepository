#include "GetPTZUrl.h"


GetPTZUrl::GetPTZUrl()
{
	cout<<"Get PTZ URL Function Called "<<"\n"<<"\n";
}
GetPTZUrl::~GetPTZUrl()
{
}

int GetPTZUrl::GetPTZ(string str,OnvifClientDevice &onvifDevice)
{
	OnvifClientDevice *onvifDeviceobj;
	onvifDeviceobj=&onvifDevice;

	_tds__GetCapabilitiesResponse capabilitiesResponse;		// To get PTZ Url and profiles get capabilities is must without that you wont be able to get URL
	int test1 = onvifDeviceobj->GetCapabilities(capabilitiesResponse);

	if (onvifDeviceobj->GetPTZUrl(str))
		cout << "Camera has PTZ with URl : " <<str<< endl;

		else
		 cout << "string does not have a PTZ camera " << endl;

	return 0;

}


bool GetPTZUrl::IsCameraPTZ(OnvifClientDevice &onvifDevice)
{
	OnvifClientDevice *onvifDeviceobj;
	onvifDeviceobj=&onvifDevice;
	string str;
	_tds__GetCapabilitiesResponse capabilitiesResponse;		// To get PTZ Url and profiles get capabilities is must without that you wont be able to get URL
	int test1 = onvifDeviceobj->GetCapabilities(capabilitiesResponse);

	if (onvifDeviceobj->GetPTZUrl(str))
		return true;
	else
		return false;
}
