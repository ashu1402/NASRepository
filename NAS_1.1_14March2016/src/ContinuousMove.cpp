#include "ContinuousMove.h"

ContinuousMove::ContinuousMove(float speedX,float speedY)
{
	//cout<<"Continuous Move Function Called "<<"\n"<<"\n";
}
ContinuousMove::~ContinuousMove()
{
	//cout<<"Continuous Move Object deleted "<<"\n"<<"\n";
}

int ContinuousMove::Continuous(float speedX,float speedY, LONG64 Timeout,OnvifClientDevice &onvifDevice)
{
		OnvifClientDevice *onvifDeviceobj;
		onvifDeviceobj=&onvifDevice;

			_tds__GetCapabilitiesResponse capabilitiesResponse;
			int test1 = onvifDeviceobj->GetCapabilities(capabilitiesResponse);

			if (test1 < 0)		// if not onvif compliant then return error message
			{
						cout << "Camera is not Onvif Compliant or Password is incorrect"<< endl;
						return 0;
			}
			else					// if camera is onvif compliant then perform operations ,, absolute move in this case
			{
						OnvifClientPTZ ptz(*onvifDeviceobj);
						OnvifClientMedia media(*onvifDeviceobj);
						_trt__GetProfilesResponse profiles;
						media.GetProfiles(profiles);
						int size = profiles.Profiles.size();

						_tptz__ContinuousMoveResponse ContMoveResponse;
						tt__PTZSpeed Speed;
						int test1=-1;

						string profileToken;
						tt__Profile *subProfile = profiles.Profiles[0];
						ptz.Initialize();

						profileToken = subProfile->token;
						Speed.PanTilt = new tt__Vector2D();

						Speed.PanTilt->x=speedX;
						Speed.PanTilt->y=speedY;

						test1=ptz.ContinuousMove(ContMoveResponse,Speed,Timeout,profileToken);	// Absolute Movement of Camera
						cout<<"Continuous Move Completed"<<"\n";
			}


}
