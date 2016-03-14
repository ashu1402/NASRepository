#ifndef _XMLPARSER_
#define _XMLPARSER_

#include "motionxml.h"
#include "../rapidxml/rapidxml.hpp"
#include <string>

using namespace std;
using namespace rapidxml;

class MotionXML
{
private :

public :

	xml_node<>* getNodeByName(const string& nodeName, xml_node<>* parentNode);


};


#endif
