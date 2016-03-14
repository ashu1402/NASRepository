#include "motionxml.h"
#include <iostream>
using namespace std;
using namespace rapidxml;


xml_node<>* MotionXML::getNodeByName(const string& nodeName, xml_node<>* parentNode)
	{
		try{
			if(parentNode == NULL)
			{
				return NULL;
			}

			xml_node<>* node = parentNode->first_node();

			while(node != NULL)
			{
				if(node->name() == nodeName)
				{
					return node;
				}

				node = node->next_sibling();
			}
		}catch (exception &e){
			cout<<"Exception caught in MotionXML::getNodeByName Function:"<<e.what()<<endl;
		}
		return NULL;
	}
