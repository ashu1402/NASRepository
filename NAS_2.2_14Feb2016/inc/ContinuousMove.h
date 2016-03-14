#ifndef _CONTINUOUS_MOVE_
#define _CONTINUOUS_MOVE_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class ContinuousMove
{


public :

	ContinuousMove(float speedX,float speedY);
    ~ContinuousMove();
	int Continuous(float speedX,float speedY, LONG64 Timeout,OnvifClientDevice &onvifDevice);
};


#endif
