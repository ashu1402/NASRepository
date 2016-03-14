#include "GetRTSPUrl.h"


GetRTSPUrl::GetRTSPUrl()
{
	cout<<"Get RTSP Function Called "<<"\n"<<"\n";
}
GetRTSPUrl::~GetRTSPUrl()
{
}

bool GetRTSPUrl::GetRTSP(OnvifClientDevice &onvifDevice, char * rtsp)
{
	OnvifClientDevice *onvifDeviceobj;
	onvifDeviceobj=&onvifDevice;

	_tds__GetCapabilitiesResponse capabilitiesResponse;		// To get RTSP Url and profiles get capabilities is must
	int test1 = onvifDeviceobj->GetCapabilities(capabilitiesResponse);

	OnvifClientMedia media(*onvifDeviceobj);
	_trt__GetProfilesResponse profiles;
	media.GetProfiles(profiles);

	int size = profiles.Profiles.size();
	for (int i = 0; i < size; i++)
	{
		tt__Profile *subProfile = profiles.Profiles[i];
		cout << "subProfile->Name" << subProfile->token << endl << endl;
		_trt__GetStreamUriResponse StreamUriResponse;
		tt__StreamSetup StreamSetup;
		media.GetStreamUri(StreamUriResponse, StreamSetup, subProfile->token);
		sprintf ( rtsp, "%s", StreamUriResponse.MediaUri->Uri.c_str());
		return true;
		//cout << "RTSP URL=" << StreamUriResponse.MediaUri->Uri << endl<<endl; //to get the url of the camera
	}
		return false;
}
