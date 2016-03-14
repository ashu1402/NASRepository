#ifndef _OnvifMain_
#define _OnvifMain_

#include "../include/onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class OnvifMain
{
public :
	OnvifClientDevice *onvifDevice;
	//OnvifClientDevice onvifDeviceobj;
	//OnvifClientDevice onvifDevice;


public :

	OnvifMain();
	~OnvifMain();
	//void test();
	void Initialize(string ip,string uname,string pwd); // to initialise
	int GetCapability();	// to get capabilities of camera
	void GetProfiles();		// to get url of profiles of camera
	void GetptzUrl(string strUrl);	// to get url of the ptz
	bool DetectMotion();	//to detect motion in the camera
	void MovePTZA();	// to configure the ptz camera
	void MovePTZC();
	void MovePTZR();
	void ChangePtzCameraPositionA(float positionX , float positionY, float speedX , float speedY );
	void ChangePtzCameraPositionC(float speedX , float speedY );
	void ChangePtzCameraPositionR(float positionX , float positionY, float speedX , float speedY );
	void GetPtzStatus();
	void GetServiceCap();
	void GNodes();
	void GNode();
	void Stop();


	/*OnvifClientDevice onvifDevice(url, user, pass);
	_tds__GetCapabilitiesResponse capabilitiesResponse;
	/*int GetCapability(_tds__GetCapabilitiesResponse capabilitiesResponse);
	string GetPTZUrl(string strUrl);
	void GetProfiles();
	string GetCameraUrl();
	void DetectMotion();*/


};


#endif
