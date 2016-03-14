#include "AbsoluteMove.h"


AbsoluteMove::AbsoluteMove(float positionX,float positionY,float speedX,float speedY)
{
	//cout<<"Absolute Move Function Called "<<"\n"<<"\n";
}
AbsoluteMove::~AbsoluteMove()
{
	//cout<<"Absolute Move Object deleted "<<"\n"<<"\n";
}

int AbsoluteMove::Absolute(float positionX,float positionY ,float speedX,float speedY,OnvifClientDevice &onvifDevice)
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

						_tptz__AbsoluteMoveResponse AbsoluteMoveResponse;
						tt__PTZSpeed Velocity;
						tt__PTZVector position;
						int test1=-1;

						string profileToken;
						tt__Profile *subProfile = profiles.Profiles[0];
						ptz.Initialize();

						profileToken = subProfile->token;
						position.PanTilt = new tt__Vector2D();
						Velocity.PanTilt = new tt__Vector2D();

						position.PanTilt->x=positionX;
						position.PanTilt->y=positionY;
						Velocity.PanTilt->x=speedX;
						Velocity.PanTilt->y=speedY;

						test1=ptz.AbsoluteMove(AbsoluteMoveResponse,Velocity,position,profileToken);	// Absolute Movement of Camera
						//cout<<"Absolute Move Completed"<<"\n";
			}

}
