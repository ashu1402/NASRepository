/*
 * CameraControl.cpp
 *
 *  Created on: 08-Feb-2016
 *      Author: ubuntu
 */

#include "CameraControl.h"


CameraControl::CameraControl(char * CameraIPInfo , char * userNameInfo , char * passwordInfo, char * systemIpInfo , char * portNumberInfo)
{

	//Store
	cameraIp = (char *)malloc(sizeof(char)*100);
	userName = (char *)malloc(sizeof(char)*100);
	password = (char *)malloc(sizeof(char)*100);
	systemIP = (char *)malloc(sizeof(char)*100);
	portNumber = (char *)malloc(sizeof(char)*100);

	sprintf ( cameraIp,"%s",CameraIPInfo);
	sprintf ( userName,"%s",userNameInfo);
	sprintf ( password,"%s",passwordInfo);
	sprintf ( systemIP,"%s",systemIpInfo);
	sprintf ( portNumber,"%s",portNumberInfo);


	float positionX, positionY, speedX, speedY;

	Initialize(cameraIp,userName,password,systemIP,portNumber);

	absoluteMove = new AbsoluteMove( positionX, positionY, speedX, speedY );
	continuousMove= new ContinuousMove(speedX, speedY);
	stopContinuous= new StopContinuous();
	relativeMoveEvent= new RelativeMoveEvent(positionX,positionY, speedX, speedY);
	getPTZUrl = new GetPTZUrl();
	isPTZCamera = getPTZUrl->IsCameraPTZ(*onvifDevice);
}

CameraControl::CameraControl(){
	getRtspUrl = new GetRTSPUrl();

}

bool CameraControl::VerificationCamera( char * ipAddress , char * portNumberInfo , char * userNameInfo , char * passwordInfo ,char * rtsp, char * systemIPInfo)
{
	Initialize(ipAddress,userNameInfo,passwordInfo,systemIPInfo,portNumberInfo);
	return getRtspUrl->GetRTSP(*onvifDevice, rtsp );
	//return 0;
}

void CameraControl::StartAbsoluteMove()
{
	if ( isPTZCamera )
		absoluteMove->Absolute(0.5,0.5,0.5,0.5,*onvifDevice);
}
bool CameraControl::StartRelativeMove( int index )
{
	if ( isPTZCamera ){
		relativeMoveEvent->RMove(index,*onvifDevice);
		return true;
		}
	else
		return false;
}

bool CameraControl::StartContinuousMove()
{
	if ( isPTZCamera ){
		continuousMove->Continuous(0.3,0.3,100,*onvifDevice);
	return true;
			}
		else
			return false;
}

void CameraControl::StopContinuousMove()
{
	if ( isPTZCamera )
		stopContinuous->Stop(*onvifDevice);

		//
}
void CameraControl::Initialize(char * CameraIPInfo , char * userNameInfo , char * passwordInfo, char * systemIpInfo , char * portNumberInfo)
{

	char c_uri[1000];
	string curl;
	string str_ptz;
	string uName, cIP, pass, sIP, portNO;
	uName = userNameInfo;
	pass = passwordInfo;

	sprintf(c_uri, "http://%s/onvif/device_service",CameraIPInfo);
	curl = c_uri;
	cout<<"\n";
	cout << "Camera Url Generated : " <<curl<<"\n\n";
	onvifDevice = new OnvifClientDevice(curl,uName,pass);

	char s_uri[1000];
	string surl;
	sprintf(s_uri,"http://%s:9090/subscription-2",systemIpInfo);
	surl = s_uri;
	cout<<"\n";
	cout << "System Url Generated : " <<surl<<"\n\n";


}

