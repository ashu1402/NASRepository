#ifndef _DETECT_MOTION_
#define _DETECT_MOTION_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class DetectMotion
{

public :

	DetectMotion();
    ~DetectMotion();
	int GetMotion(string Notify,OnvifClientDevice &onvifDevice);
};


#endif
