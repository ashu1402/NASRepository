#ifndef _ABSOLUTE_MOVE_
#define _ABSOLUTE_MOVE_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class AbsoluteMove
{


public :

	AbsoluteMove(float positionX,float positionY,float speedX,float speedY);
    ~AbsoluteMove();
	int Absolute(float positionX,float positionY ,float speedX,float speedY,OnvifClientDevice &onvifDevice);
};


#endif
