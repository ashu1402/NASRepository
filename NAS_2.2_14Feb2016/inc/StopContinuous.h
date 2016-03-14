#ifndef _STOP_
#define _STOP_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class StopContinuous
{


public :

	StopContinuous();
    ~StopContinuous();
	int Stop(OnvifClientDevice &onvifDevice);
};


#endif
