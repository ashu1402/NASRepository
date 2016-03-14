#ifndef _RELATIVE_MOVE_
#define _RELATIVE_MOVE_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class RelativeMove
{


public :

	RelativeMove();
    ~RelativeMove();
	int Relative(float positionX,float positionY ,float speedX,float speedY,OnvifClientDevice &onvifDevice);
};


#endif
