#ifndef _GetPTZUrl_
#define _GetPTZUrl_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class GetPTZUrl
{

public :

	GetPTZUrl();
    ~GetPTZUrl();
	int GetPTZ(string str,OnvifClientDevice &onvifDevice);
	bool IsCameraPTZ(OnvifClientDevice &onvifDevice);
};


#endif
