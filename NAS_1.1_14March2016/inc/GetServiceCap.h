#ifndef _GET_SERVICE_CAP_
#define _GET_SERVICE_CAP_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class GetServiceCap
{

public :

	GetServiceCap();
    ~GetServiceCap();
	int GetService(OnvifClientDevice &onvifDevice);
};


#endif
