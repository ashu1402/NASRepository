#include "GetPTZNodes.h"


GetPTZNodes::GetPTZNodes()
{
	cout<<"Get PTZ Nodes Function Called "<<"\n"<<"\n";
}
GetPTZNodes::~GetPTZNodes()
{
}

int GetPTZNodes::GetNodes(OnvifClientDevice &onvifDevice)
{
		OnvifClientDevice *onvifDeviceobj;
		onvifDeviceobj=&onvifDevice;

		OnvifClientPTZ ptz(*onvifDeviceobj);

			ptz.Initialize();

			_tptz__GetNodesResponse GetNodesResponse;

			int test1=-1;
			test1=ptz.GetNodes(GetNodesResponse);

			int size=GetNodesResponse.PTZNode.size();
			cout<<"number of nodes : "<<size<<"\n";
			for(int i=0;i<size;i++)
			{
				tt__PTZNode *node=GetNodesResponse.PTZNode[i];
				cout<<"NODE->NAME : "<<*node->Name<<"\n";									// To get name of the node
				cout<<"NODE->TOKEN : "<<node->token<<"\n";									// To get token of node
				cout<<"NODE->SPACE : "<<node->SupportedPTZSpaces<<"\n";						// To get PTZ space of node		............
				cout<<"NODE->NO_OF_PRESETS : "<<node->MaximumNumberOfPresets<<"\n";			// To check availability of home position operation
				bool x1;
				x1=node->HomeSupported;
				cout<<"HOME_POSITION_AVAILABILITY : "<<x1<<"\n";							// To check home position support is available or not
			//	node->AuxiliaryCommands;													//	To check the auxiliary commands for given ptz node ...................
			}

	return 0;

}
