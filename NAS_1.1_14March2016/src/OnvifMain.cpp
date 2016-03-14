//#include "OnvifMain.h"
//#include "DeviceBinding.nsmap"
//
//OnvifMain::OnvifMain() {
////		///cout<<"!! provide ip address and other details !!";
////		///cout<<"\n";
//
//}
//
//OnvifMain::~OnvifMain() {
//	//free();
//}
//
//void OnvifMain::Initialize(string ip, string uname, string pwd)	// to initialise the ip , user name , and pwd of the system
//		{
//	int ret;
//	char uri[1000];
//	string url;
//	sprintf(uri, "http://%s/onvif/device_service", ip.c_str());
//	//strcpy( (const char *)url,uri);
//	url = uri;
//	cout << "Device Url Generated : " << url << endl;
//	//string eventNotify = "http://192.168.1.17:8989/subscription-2";
//	onvifDevice = new OnvifClientDevice(url, uname, pwd);
//	//new OnvifClientDevice()
//}
//
//int OnvifMain::GetCapability()// to check camera is onvif complient or  not and if yes other detials about that
//{
//
//	_tds__GetCapabilitiesResponse capabilitiesResponse;
//	int test1 = onvifDevice->GetCapabilities(capabilitiesResponse);
//	//capabilitiesResponse.Capabilities->Analytics->AnalyticsModuleSupport;
//	if (test1 < 0) {
//		cout << "Camera is not Onvif Compliant or Password is incorrect"
//				<< endl;
//		return 0;
//	}
//
//	_tds__GetDeviceInformationResponse DeviceInformationResponse;
//	int test2 = onvifDevice->GetDeviceInformation(DeviceInformationResponse);
//	if (test2 < 0) {
//		cout << "device information not received " << endl;
//		return 0;
//	}
//	cout << DeviceInformationResponse.FirmwareVersion << "\n";
//
//	_tds__GetHostnameResponse GetHostnameResponse;
//	int test3 = onvifDevice->GetHostname(GetHostnameResponse);
//	if (test3 < 0) {
//		cout << "device information not received " << endl;
//		return 0;
//	}
//	cout << "Device Manufacturer : "
//			<< *GetHostnameResponse.HostnameInformation->Name << endl;
//
//	//	cout<<Devi
//	//cout<<capabilitiesResponse.Capabilities->Events->;
//	//capabilitiesResponse.Capabilities->PTZ->__any;
//	//cout<<"Test Capabilities "<<capabilitiesResponse.Capabilities->Events->WSPullPointSupport<<endl;
//}
//
//void OnvifMain::GetProfiles()// to check URL of the profiles of the camera , main stream and sub stream
//{
//	OnvifClientMedia media(*onvifDevice);
//	_trt__GetProfilesResponse profiles;
//	media.GetProfiles(profiles);
//
//	int size = profiles.Profiles.size();
//	for (int i = 0; i < size; i++) {
//		tt__Profile *subProfile = profiles.Profiles[i];
//		cout << "subProfile->Name" << subProfile->token << endl << endl;
//		_trt__GetStreamUriResponse StreamUriResponse;
//		tt__StreamSetup StreamSetup;
//		media.GetStreamUri(StreamUriResponse, StreamSetup, subProfile->token);
//		cout << "RTSP URL=" << StreamUriResponse.MediaUri->Uri << endl; //to get the url of the camera
//	}
//}
//void OnvifMain::GetptzUrl(string strUrl)// to get url of the camera if its PTZ
//		{
//	if (onvifDevice->GetPTZUrl(strUrl))
//		cout << "Camera has Ptz with URl " << strUrl << endl;
//
//	else
//		cout << "string does not have a PTZ camera " << endl;
//
//}
//
//bool OnvifMain::DetectMotion()
//{
//	int ret;
//		string eventNotify = "http://192.168.1.103:7878/subscription-2";
//		OnvifClientReceiver onvifReceiver(*onvifDevice);
//		_trv__GetReceiversResponse receivers;
//		onvifReceiver.GetReceivers(receivers);
//		for (vector<tt__Receiver * >::iterator iter =
//				receivers.Receivers.begin();
//				iter < receivers.Receivers.end(); ++iter)
//		{
//			tt__Receiver * pReceivers = *iter;
//			printf("pReceivers %s\n", pReceivers->Token.c_str());
//		}
//
//		OnvifClientEvent onvifEvent(*onvifDevice);
//		string str;
//		onvifDevice->GetMediaUrl(str);
//		onvifEvent.Subscribe(eventNotify);
//
//		struct soap* soapObj = soap_new();
//		OnvifClientEventNotify notify(soapObj);
//
//	    if ((ret = soap_bind(&notify, NULL, 7878, 100) == SOAP_INVALID_SOCKET ) )
//		{
//	        printf("OnvifClientEventNotify::soap_bind Binding on %d port failed\n\n",9090);
//	        return 0;
//	    }
//		int timeStart = time(NULL);
//		int currentTime = 0;
//
//		//Loop to recevie the event
//		while(1)
//		{
//			currentTime = time(NULL);
//			if (currentTime - timeStart > 30)
//			{
//				onvifEvent.Renew();
//			}
//			//printf("soap_accept accepting\n");
//			if( (ret = soap_accept(&notify)) == SOAP_INVALID_SOCKET)
//			{
//				printf("soap_accept accepting timeout\n");
//				return 0;
//			}
//
//	        if ( (soap_begin_serve(&notify)) != SOAP_OK)
//	        {
//	            printf("soap_begin_serve serve %d failed\n\n", ret);
//	            return 0;
//	        }
//
//	        ret = notify.dispatch();
//			//soap_wchar msg = soap_get(soapObj);
//		}
//}
//
//
//
//void OnvifMain::ChangePtzCameraPositionA( float positionX , float positionY, float speedX , float speedY )
//{
//		OnvifClientPTZ ptz(*onvifDevice);
//
//		OnvifClientMedia media(*onvifDevice);
//		_trt__GetProfilesResponse profiles;
//		media.GetProfiles(profiles);
//		int size = profiles.Profiles.size();
//
//		_tptz__ContinuousMoveResponse ContMoveResponse;
//		_tptz__AbsoluteMoveResponse AbsoluteMoveResponse;
//		_tptz__RelativeMoveResponse relMoveResponse;
//
//		tt__PTZSpeed Velocity;
//		tt__PTZVector position;
//		int test1=-1;
//
//		string profileToken;
//		tt__Profile *subProfile = profiles.Profiles[0];
//		ptz.Initialize();
//
//		profileToken = subProfile->token;
//		position.PanTilt = new tt__Vector2D();
//		Velocity.PanTilt = new tt__Vector2D();
//
//		position.PanTilt->x=positionX;
//		position.PanTilt->y=positionY;
//
//		Velocity.PanTilt->x=speedX;
//		Velocity.PanTilt->y=speedY;
//
//		cout<<"x1"<<"\n";
//		test1=ptz.AbsoluteMove(AbsoluteMoveResponse,Velocity,position,profileToken);	// Absolute Movement of Camera
//		cout<<"x2"<<"\n\n";
//		sleep(2);
//		//ptz.ContinuousMove(ContMoveResponse,Velocity,Timeout,profileToken);			// Continuous Movement of Camera
//		//cout<<"dsw"<<"\n";
//
//}
//void OnvifMain::MovePTZA()
//{
//
//	ChangePtzCameraPositionA (0.0,0.0,0.2,0.2);		// to set position and speed of PTZ camera
//}
//void OnvifMain::ChangePtzCameraPositionC(float speedX , float speedY )
//{
//		OnvifClientPTZ ptz(*onvifDevice);
//
//		OnvifClientMedia media(*onvifDevice);
//		_trt__GetProfilesResponse profiles;
//		media.GetProfiles(profiles);
//		int size = profiles.Profiles.size();
//
//		ptz.Initialize();
//		_tptz__ContinuousMoveResponse ContMoveResponse;
//		int test1=-1;
//
//		tt__PTZSpeed Velocity;
//		tt__PTZVector position;
//		LONG64 Timeout=1000000;
//
//		string profileToken;
//		tt__Profile *subProfile = profiles.Profiles[0];
//
//		profileToken = subProfile->token;
//		Velocity.PanTilt = new tt__Vector2D();
//		position.PanTilt = new tt__Vector2D();
//
//		position.PanTilt->x=0.0;
//		position.PanTilt->y=0.0;
//
//		Velocity.PanTilt->x=speedX;
//		Velocity.PanTilt->y=speedY;
//
//	//	Velocity.PanTilt->space=(string*)"http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace";
//
//		cout<<"x1"<<"\n";
//		cout<<"POSITION_X : "<<position.PanTilt->x<<"\n";
//		cout<<"POSITION_Y : "<<position.PanTilt->y<<"\n";
//		cout<<"\n";
//		test1=ptz.ContinuousMove(ContMoveResponse,Velocity,Timeout,position,profileToken);	// Continuous Movement of Camera
//
//		cout<<"x2"<<"\n";
//		//cout<<"dsw"<<"\n";
//}
//void OnvifMain::MovePTZC()
//{
//	ChangePtzCameraPositionC (0.1,0.06555555758451);		// to set position and speed of PTZ camera
//}
//void OnvifMain::ChangePtzCameraPositionR(float positionX , float positionY, float speedX , float speedY)
//{
//	OnvifClientPTZ ptz(*onvifDevice);
//
//	OnvifClientMedia media(*onvifDevice);
//	_trt__GetProfilesResponse profiles;
//	media.GetProfiles(profiles);
//	int size = profiles.Profiles.size();
//
//	int test1=-1;
//
//	_tptz__RelativeMoveResponse relMoveResponse;
//
//	tt__PTZSpeed Speed;
//	tt__PTZVector Translation;
//
//	string profileToken;
//	tt__Profile *subProfile = profiles.Profiles[0];
//	ptz.Initialize();
//
//	profileToken = subProfile->token;
//	Translation.PanTilt = new tt__Vector2D();
//	Speed.PanTilt = new tt__Vector2D();
//
//	Translation.PanTilt->x=positionX;
//	Translation.PanTilt->y=positionY;
//
//	Speed.PanTilt->x=speedX;
//	Speed.PanTilt->y=speedY;
//	cout<<"x1"<<"\n";
//	test1=ptz.RelativeMove(relMoveResponse,Translation,Speed,profileToken);
//	cout<<"x2"<<"\n";
//
//	//cout<<"dsw"<<"\n";
//
//}
//void OnvifMain::MovePTZR()
//{
//
//	ChangePtzCameraPositionR (0.5,0.5,0.5,0.5);		// to set position and speed of PTZ camera
//}
//
//
//void OnvifMain::GetPtzStatus()
//{
//	OnvifClientPTZ ptz(*onvifDevice);
//	_tptz__GetStatusResponse StatusResponse;
//
//	ptz.Initialize();
//
//	OnvifClientMedia media(*onvifDevice);
//	_trt__GetProfilesResponse profiles;
//	media.GetProfiles(profiles);
//	int size = profiles.Profiles.size();
//
//
//	int test1=0;
//	string profileToken;
//
//	tt__Profile *subProfile = profiles.Profiles[0];
//	profileToken = subProfile->token;
//
//
//	test1=ptz.GetStatus(StatusResponse,profileToken);
//	cout<<"test1 : "<<test1<<"\n";
//	cout<<"POSITION_X : "<<StatusResponse.PTZStatus->Position->PanTilt->x<<"\n";
//	cout<<"POSITION_Y : "<<StatusResponse.PTZStatus->Position->PanTilt->y<<"\n";
//	cout<<"POSITION_SPACE : "<<*StatusResponse.PTZStatus->Position->PanTilt->space<<"\n";
//	cout<<"ZOOM_X : "<<StatusResponse.PTZStatus->Position->Zoom->x<<"\n";
//	cout<<"ZOOM_X_SPACE : "<<*StatusResponse.PTZStatus->Position->Zoom->space<<"\n";
//	//cout<<"MOVE_STATUS_PANTILT : "<<StatusResponse.PTZStatus->MoveStatus->PanTilt<<"\n";
//	//cout<<"MOVE_STATUS_ZOOM : "<<StatusResponse.PTZStatus->MoveStatus->Zoom<<"\n";
//	cout<<"PTZ_ERROR : "<<*StatusResponse.PTZStatus->Error<<"\n";
//	cout<<"UTC_TIME : "<<StatusResponse.PTZStatus->UtcTime<<"\n";
//	cout<<"\n";
//}
//void OnvifMain::GetServiceCap()
//{
//		OnvifClientPTZ ptz(*onvifDevice);
//
//		_tptz__GetServiceCapabilitiesResponse ServiceCapResponse;
//		int test1=-1;
//		ptz.Initialize();
//
//		test1=ptz.GetServiceCapabilities(ServiceCapResponse);
//		bool x1;
//		x1=ServiceCapResponse.Capabilities->EFlip;	// EFlip Support means camera will adjust itself after 90' tilt to display image normally else it will be shown upside down
//		cout<<"EFLIP_SUPPORT : "<<x1<<"\n";
//		bool x2;
//		x2=ServiceCapResponse.Capabilities->Reverse;	//	Indicates whether or not reversing of PT control direction is supported
//		cout<<"REVERSE_SUPPORT : "<<x2<<"\n";
//		bool x3;
//		x3=ServiceCapResponse.Capabilities->GetCompatibleConfigurations;	//	Indicates support for the GetCompatibleConfigurations command
//		cout<<"GETCOMPATIBLECONFIGURATIONS_COMMAND_SUPPORT : "<<x3<<"\n";
//		bool x4;
//		x4=ServiceCapResponse.Capabilities->MoveStatus;		//	Indicates that the PTZVector includes MoveStatus information
//		cout<<"PTZVECTOR_MOVESTATUS_INFORMATION : "<<x4<<"\n";
//		bool x5;
//		x5=ServiceCapResponse.Capabilities->StatusPosition;		//	Indicates that the PTZVector includes Position information
//		cout<<"PTZVECTOR_POSITION_INFORMATION : "<<x5<<"\n";
//		cout<<"\n";
//
//}
//void OnvifMain::GNodes()
//{
//	OnvifClientPTZ ptz(*onvifDevice);
//
//	ptz.Initialize();
//
//	_tptz__GetNodesResponse GetNodesResponse;
//
//	int test1=-1;
//	test1=ptz.GetNodes(GetNodesResponse);
//
//	int size=GetNodesResponse.PTZNode.size();
//	cout<<"number of nodes : "<<size<<"\n";
//	for(int i=0;i<size;i++)
//	{
//		tt__PTZNode *node=GetNodesResponse.PTZNode[i];
//		cout<<"NODE->NAME : "<<*node->Name<<"\n";									// To get name of the node
//		cout<<"NODE->TOKEN : "<<node->token<<"\n";									// To get token of node
//		cout<<"NODE->SPACE : "<<node->SupportedPTZSpaces<<"\n";						// To get PTZ space of node		............
//		cout<<"NODE->NO_OF_PRESETS : "<<node->MaximumNumberOfPresets<<"\n";			// To check availability of home position operation
//		bool x1;
//		x1=node->HomeSupported;
//		cout<<"HOME_POSITION_AVAILABILITY : "<<x1<<"\n";							// To check home position support is available or not
//		node->AuxiliaryCommands;													//	To check the auxiliary commands for given ptz node ...................
//	}
//}
//void OnvifMain::GNode()		// To get the properties of PTZ node
//{
//	OnvifClientPTZ ptz(*onvifDevice);
//
//	ptz.Initialize();
//	_tptz__GetNodeResponse GetNodeResponse;
//	_tptz__GetNodesResponse GetNodesResponse;
//
//	int test1=-1;
//	int test2=-1;
//	string nodeToken;
//
//	test1=ptz.GetNodes(GetNodesResponse);
//	tt__PTZNode *node=GetNodesResponse.PTZNode[0];
//	nodeToken=node->token;
//
//	test2=ptz.GetNode(GetNodeResponse,nodeToken);
//
//}
//
//void OnvifMain::Stop()		// To stop ongoing continuous movement or any other type of movement like pan tilt
//{
//	OnvifClientPTZ ptz(*onvifDevice);
//
//	ptz.Initialize();
//
//	OnvifClientMedia media(*onvifDevice);
//	_trt__GetProfilesResponse profiles;
//	media.GetProfiles(profiles);
//	int size = profiles.Profiles.size();
//
//	_tptz__StopResponse StopResponse;
//	bool PanTilt;
//	bool Zoom;
//	string profileToken;
//	int test1=-1;
//
//	tt__Profile *subProfile = profiles.Profiles[0];
//	profileToken = subProfile->token;
//
//	test1=ptz.Stop(StopResponse,PanTilt,Zoom,profileToken);
//	cout<<"stop function called"<<"\n";
//
//}
//
