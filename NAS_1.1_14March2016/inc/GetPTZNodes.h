#ifndef _GET_PTZ_NODES_
#define _GET_PTZ_NODES_

#include "onvifclient.h"
#include "motionxml.h"
#include <iostream>


using namespace std;

class GetPTZNodes
{

public :

	GetPTZNodes();
    ~GetPTZNodes();
	int GetNodes(OnvifClientDevice &onvifDevice);
};


#endif
