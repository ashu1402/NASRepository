#ifndef _GetRTSPUrl_
#define _GetRTSPUrl_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class GetRTSPUrl
{


public :

	GetRTSPUrl();
    ~GetRTSPUrl();
	bool GetRTSP(OnvifClientDevice &onvifDevice, char * rtsp);
};


#endif
