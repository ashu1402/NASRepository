#include "XMLParser.h"
#include "XMLCreator.h"
#include "TCPConnection.h"
#include "Retrieve.h"
#include "BootLoader.h"
#include "VOD.h"
#include "Interaction.h"
#include "Monitoring.h"
#include "../onvifgen/include/DeviceBinding.nsmap"
#include "CameraControl.h"
#include "Failover.h"

#define STARTPROCESSMONITORING true
#define STARTFAILOVER true

void CreateUniqueDID(char * macAddress, int &sum){


	//long long sum=0;
	int size = strlen((const char *)macAddress);
	for(int i=size -5;i<size;i++)
	{
		switch(macAddress[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			sum=(sum<<4)+(macAddress[i]-'0');
			break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			sum=(sum<<4)+(macAddress[i]-'a'+10);
			break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			sum=(sum<<4)+(macAddress[i]-'A'+10);
			break;
		}
	}
	sum = sum >> 2;
}

bool GetMacAddress( char * macAddress ){
	struct ifreq s;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	char ethIndex [SIZEOFCHARARRAY];
	int count = 0;
	while(1){
		sprintf( ethIndex,"eth%d", count);
		strcpy(s.ifr_name, ethIndex);
		if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
			int i;
			for (i = 0; i < 6; ++i){
				if ( i == 0 )
					sprintf( macAddress,"%02x:", (unsigned char) s.ifr_addr.sa_data[i]);
				else if ( i < 5 )
					sprintf( macAddress,"%s%02x:", macAddress,(unsigned char) s.ifr_addr.sa_data[i]);
				else
					sprintf( macAddress,"%s%02x",macAddress, (unsigned char) s.ifr_addr.sa_data[i]);

			}
			return true;
		}
		count ++;
		if ( count > 5 )
			count = 0;
	}
	//cout<<"\nUnable to Fetch Mac Address\n";
	return false;
}


int CountToken( std::string str , std::string token , vector< int > &locations ){
	int size = str.size(), index = 0 , count = 0;
	int value = 0;
	char valueChar[2];
	while(1){
		if ( index <= size - 1 ){
			if ( str[index] == token[0] ){
				locations.push_back(index);
				count++;
			}
			else{
				sprintf(valueChar,"%c",str[index]);
				value = atoi(valueChar);
				if ( value == 0 ){
					if ( str[index] != '0' )
						return false;
				}

			}
			index ++;
		}
		else
			return count;
	}
}


bool IsIPAddress(std::string  ipaddr)
{
	int countTokens = -1 ;
	vector< int > locations ;
	countTokens = CountToken(ipaddr, ".", locations );

	if ( countTokens != 3 )
		return false;

	string substr;
	for ( int i = 0 ; i <= countTokens ; i++ ){
		if ( i == 0 )
			substr = ipaddr.substr(0,locations[i]);
		else
			substr = ipaddr.substr(locations[i - 1] + 1,locations[i] - locations[i - 1] - 1);
		int quad = atoi(substr.c_str());
		if (( quad < 0) || (quad > 255) )
			return false;
	}
	return true;
}


bool CheckValidation(DataBaseAccess *dBA, char * serverIP){
	char macAddress[SIZEOFCHARARRAY];
	int DID = 0;

	if ( GetMacAddress(macAddress) ){
		cout<<"\nMacAddress--->"<<macAddress<<endl;
		CreateUniqueDID(macAddress,DID);
		if ( !dBA->isMacAddressExist(macAddress, serverIP ) ){
			while(1){
				cout<<"\nPlease Enter DeviceIp---->";
				gets(serverIP);
				if ( !IsIPAddress(serverIP) ){
					cout<<"\nInvalid IPAddress Please try again\n";
				}
				else
					break;
			}
			if ( !dBA->InsertOwnerDeviceIp( macAddress,DID,serverIP ) ){
				cout<<"\n Unable to Insert \n"<<endl;
				return false;
			}
			else{
				cout<<"\nnot Exists inserted\n";
				return true;
			}
		}else{
			cout<<"\nServerIP--->"<<serverIP;
			return true;
		}
	}
	else
		return false;
}

int main(int argc, char* argv[]){

	gst_init(&argc,&argv);
	char serverIP[SIZEOFCHARARRAY];
	DataBaseAccess *dBA = new DataBaseAccess();
	cout<<"\nCreate Main Function DBA\n";
	dBA->ClearRtspTable();

	if ( CheckValidation( dBA, serverIP ) ){
		int channelCount = dBA->IsChannelsExist( dBA->GetSystemDID() );
		Notification * notification = new Notification();
		Retrieve * RetrieveObj = new Retrieve( serverIP, BroadCastStreamPortNumber, LocalStreamPortNumber );
		RetrieveObj->StartThreadForVideoEntry(RetrieveObj);

		VOD* VODObj = new VOD(serverIP, VODPortNumber );

		BootLoader* BootLoaderObj = new BootLoader( RetrieveObj );
		BootLoaderObj->StartBoot();
		Failover * failover = new Failover(BootLoaderObj);

		if  ( channelCount == 0 ){
			failover->StartThread(failover);
		}
		else
			failover->isBreak = true;


		Interaction * interaction = new Interaction(failover);
		interaction->InitializeThread(interaction);

		TCPConnection* TCPConnectionDesktop = new TCPConnection( tcpSocketPortNumber, RetrieveObj, VODObj, notification, false,failover,interaction );
		if( !TCPConnectionDesktop->StartInternalThread() )
			cout<<"Failed to create thread\n";

		TCPConnection* TCPConnectionAndroid = new TCPConnection( AndroidTcpSocketPortNumber, RetrieveObj, VODObj, notification, true,failover,interaction  );
		if( !TCPConnectionAndroid->StartInternalThread() )
			cout<<"Failed to create thread\n";
		else
			cout<<"\nTcp Connection Created\n";
		usleep(5);

		//Monitoring *monitoring = new Monitoring(dBA);

#if STARTPROCESSMONITORING
		cout<<"\nStarting Monitoring\n";
		Monitoring *monitoring;
		monitoring = new Monitoring();
		monitoring->StartInternalThread();
#endif


		dBA->Close();
		delete dBA;
		cout<<"\n Close Main Function DBA\n";
		TCPConnectionDesktop->WaitForInternalThreadToExit();
		TCPConnectionAndroid->WaitForInternalThreadToExit();


		delete VODObj;
		delete TCPConnectionDesktop;
		delete TCPConnectionAndroid;
		delete interaction;
		delete RetrieveObj;
		delete failover;
		delete BootLoaderObj;
		delete notification;
	}
	else{
		dBA->Close();
		delete dBA;
	}
	return 0;
}
