/*
 * CameraControl.h
 *
 *  Created on: 08-Feb-2016
 *      Author: ubuntu
 */

#ifndef CAMERACONTROL_H_
#define CAMERACONTROL_H_

#include "AbsoluteMove.h"
#include "ContinuousMove.h"
#include "StopContinuous.h"
#include "RelativeMoveEvent.h"
#include "GetPTZUrl.h"
#include "GetRTSPUrl.h"


class CameraControl{

public:

	OnvifClientDevice *onvifDevice;
	AbsoluteMove * absoluteMove;
	ContinuousMove * continuousMove;
	StopContinuous * stopContinuous;
	RelativeMoveEvent * relativeMoveEvent;
	GetPTZUrl * getPTZUrl;
	GetRTSPUrl * getRtspUrl;
	bool isPTZCamera;
	char * cameraIp,* userName, * password, * systemIP, * portNumber;

public:
	CameraControl();
	CameraControl( char * CameraIPInfo , char * userNameInfo , char * passwordInfo, char * systemIpInfo , char * portNumberInfo );
	void StartAbsoluteMove();
	bool StartRelativeMove( int index );
	bool StartContinuousMove();
	void StopContinuousMove();
	void Initialize(char * CameraIPInfo , char * userNameInfo , char * passwordInfo, char * systemIpInfo , char * portNumberInfo);
	bool VerificationCamera( char * ipAddress , char * portNumber , char * userName , char * password ,char * rtsp, char * systemIPInfo);

};





#endif /* CAMERACONTROL_H_ */
