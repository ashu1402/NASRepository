#ifndef _GETPTZ_STATUS_
#define _GETPTZ_STATUS_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class GetPTZStatus
{

public :

	GetPTZStatus();
    ~GetPTZStatus();
	int GetStatus(OnvifClientDevice &onvifDevice);
};


#endif
