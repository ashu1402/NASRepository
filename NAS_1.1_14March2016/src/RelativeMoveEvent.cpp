#include "RelativeMoveEvent.h"


RelativeMoveEvent::RelativeMoveEvent(float positionX,float positionY,float speedX,float speedY)
{
	//cout<<"Relative Move Function Called "<<"\n"<<"\n";
}
RelativeMoveEvent::~RelativeMoveEvent()
{
	//cout<<"Relative Move object deleted "<<"\n\n";
}

int RelativeMoveEvent::Relative(float positionX,float positionY ,float speedX,float speedY,OnvifClientDevice &onvifDevice)
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
						cout<<"size : "<<size;

						_tptz__RelativeMoveResponse relMoveResponse;
						tt__PTZSpeed Speed;
						tt__PTZVector Translation;
						int test1=-1;

						string profileToken;
						tt__Profile *subProfile = profiles.Profiles[0];
						ptz.Initialize();

						profileToken = subProfile->token;
						Translation.PanTilt = new tt__Vector2D();
						Speed.PanTilt = new tt__Vector2D();

						Translation.PanTilt->x=positionX;
						Translation.PanTilt->y=positionY;
						Speed.PanTilt->x=speedX;
						Speed.PanTilt->y=speedY;

						test1=ptz.RelativeMove(relMoveResponse,Translation,Speed,profileToken);	// Absolute Movement of Camera
						cout<<"Relative Move Completed"<<"\n";
			}

}
int RelativeMoveEvent::RMove(int x,OnvifClientDevice &onvifDevice)
{

	   // local variable declaration:
	   int Event = x;
	   OnvifClientDevice *onvifDeviceobj;
	   onvifDeviceobj=&onvifDevice;

	   switch(Event)
	   {
	   case 1 :
		   Relative(-0.1,0.1,0.5,0.5,*onvifDeviceobj);
		   break;
	   case 2:
		   Relative(-0.1,0.0,0.5,0.5,*onvifDeviceobj);
		   break;
	   case 3:
		   Relative(-0.1,-0.1,0.5,0.5,*onvifDeviceobj);
		   break;
	   case 4:
		   Relative(0.0,-0.1,0.5,0.5,*onvifDeviceobj);
		   break;
	   case 5:
		   Relative(0.1,-0.1,0.5,0.5,*onvifDeviceobj);
		   break;
	   case 6:
		   Relative(0.1,0.0,0.5,0.5,*onvifDeviceobj);
		   break;
	   case 7:
		   Relative(0.1,0.1,0.5,0.5,*onvifDeviceobj);
		   break;
	   case 8:
		   Relative(0.0,0.1,0.5,0.5,*onvifDeviceobj);
		   break;
	   	default :
	   		Relative(0.0,0.0,0.5,0.5,*onvifDeviceobj);
	   }

	   return 0;

}


