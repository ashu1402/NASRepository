#ifndef _RELATIVE_MOVE_EVENT_
#define _RELATIVE_MOVE_EVENT_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class RelativeMoveEvent
{


public :

	RelativeMoveEvent(float positionX,float positionY,float speedX,float speedY);
    ~RelativeMoveEvent();
	int Relative(float positionX,float positionY ,float speedX,float speedY,OnvifClientDevice &onvifDevice);
	int RMove(int x,OnvifClientDevice &onvifDevice);
};


#endif
