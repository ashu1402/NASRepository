#include "GetPTZStatus.h"


GetPTZStatus::GetPTZStatus()
{
	cout<<"Get PTZ Status Function Called "<<"\n"<<"\n";
}
GetPTZStatus::~GetPTZStatus()
{
}

int GetPTZStatus::GetStatus(OnvifClientDevice &onvifDevice)
{
			OnvifClientDevice *onvifDeviceobj;
			onvifDeviceobj=&onvifDevice;

			OnvifClientPTZ ptz(*onvifDeviceobj);
			_tptz__GetStatusResponse StatusResponse;

			ptz.Initialize();
			OnvifClientMedia media(*onvifDeviceobj);
			_trt__GetProfilesResponse profiles;
			media.GetProfiles(profiles);
			int size = profiles.Profiles.size();


			int test1=0;
			string profileToken;

			tt__Profile *subProfile = profiles.Profiles[0];
			profileToken = subProfile->token;


			test1=ptz.GetStatus(StatusResponse,profileToken);
			cout<<"test1 : "<<test1<<"\n";
			cout<<"POSITION_X : "<<StatusResponse.PTZStatus->Position->PanTilt->x<<"\n";
			cout<<"POSITION_Y : "<<StatusResponse.PTZStatus->Position->PanTilt->y<<"\n";
			cout<<"POSITION_SPACE : "<<*StatusResponse.PTZStatus->Position->PanTilt->space<<"\n";
			cout<<"ZOOM_X : "<<StatusResponse.PTZStatus->Position->Zoom->x<<"\n";
			cout<<"ZOOM_X_SPACE : "<<*StatusResponse.PTZStatus->Position->Zoom->space<<"\n";
			//cout<<"MOVE_STATUS_PANTILT : "<<StatusResponse.PTZStatus->MoveStatus->PanTilt<<"\n";
			//cout<<"MOVE_STATUS_ZOOM : "<<StatusResponse.PTZStatus->MoveStatus->Zoom<<"\n";
			cout<<"PTZ_ERROR : "<<*StatusResponse.PTZStatus->Error<<"\n";
			cout<<"UTC_TIME : "<<StatusResponse.PTZStatus->UtcTime<<"\n";
			cout<<"\n";

	return 0;

}
