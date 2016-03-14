#include "GetServiceCap.h".h"


GetServiceCap::GetServiceCap()
{
	cout<<"Get Service Capabilities Function Called "<<"\n"<<"\n";
}
GetServiceCap::~GetServiceCap()
{
}

int GetServiceCap::GetService(OnvifClientDevice &onvifDevice)
{
		OnvifClientDevice *onvifDeviceobj;
		onvifDeviceobj=&onvifDevice;

		OnvifClientPTZ ptz(*onvifDeviceobj);
		_tptz__GetServiceCapabilitiesResponse ServiceCapResponse;
		int test1=-1;
		ptz.Initialize();

		test1=ptz.GetServiceCapabilities(ServiceCapResponse);
		bool x1;
		x1=ServiceCapResponse.Capabilities->EFlip;	// EFlip Support means camera will adjust itself after 90' tilt to display image normally else it will be shown upside down
		cout<<"EFLIP_SUPPORT : "<<x1<<"\n";
		bool x2;
		x2=ServiceCapResponse.Capabilities->Reverse;	//	Indicates whether or not reversing of PT control direction is supported
		cout<<"REVERSE_SUPPORT : "<<x2<<"\n";
		bool x3;
		x3=ServiceCapResponse.Capabilities->GetCompatibleConfigurations;	//	Indicates support for the GetCompatibleConfigurations command
		cout<<"GETCOMPATIBLECONFIGURATIONS_COMMAND_SUPPORT : "<<x3<<"\n";
		bool x4;
		x4=ServiceCapResponse.Capabilities->MoveStatus;		//	Indicates that the PTZVector includes MoveStatus information
		cout<<"PTZVECTOR_MOVESTATUS_INFORMATION : "<<x4<<"\n";
		bool x5;
		x5=ServiceCapResponse.Capabilities->StatusPosition;		//	Indicates that the PTZVector includes Position information
		cout<<"PTZVECTOR_POSITION_INFORMATION : "<<x5<<"\n";
		cout<<"\n";
	return 0;

}
