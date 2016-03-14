#include "StopContinuous.h"

StopContinuous::StopContinuous()
{
	//cout<<"Stop Function Called "<<"\n"<<"\n";
}
StopContinuous::~StopContinuous()
{
}

int StopContinuous::Stop(OnvifClientDevice &onvifDevice)
{
		OnvifClientDevice *onvifDeviceobj;
		onvifDeviceobj=&onvifDevice;

		OnvifClientPTZ ptz(*onvifDeviceobj);

		ptz.Initialize();

		OnvifClientMedia media(*onvifDeviceobj);
		_trt__GetProfilesResponse profiles;
		media.GetProfiles(profiles);
		int size = profiles.Profiles.size();

		_tptz__StopResponse StopResponse;
		bool PanTilt;
		bool Zoom;
		string profileToken;
		int test1=-1;

		tt__Profile *subProfile = profiles.Profiles[0];
		profileToken = subProfile->token;

		test1=ptz.Stop(StopResponse,PanTilt,Zoom,profileToken);
		cout<<"stop function called"<<"\n";



}
