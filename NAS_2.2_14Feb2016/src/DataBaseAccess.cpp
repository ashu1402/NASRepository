#include "DataBaseAccess.h"

void DataBaseAccess::PrintException(string className, string functionName,
		exception e) {
	try {
		FILE* file = fopen("NAS.log", "a");
		char output[SIZEOFCHARARRAY];

		time_t t;
		time(&t);
		strftime(output, 21, "%Y-%m-%d %H:%M:%S", localtime(&t));
		fprintf(file, "time---> %s \n", output);
		fprintf(file, "%s    %s    %s \n", className.c_str(),
				functionName.c_str(), e.what());
		fclose(file);

	} catch (std::exception e) {
		if (  ISDEBUG )  cout<<"\nError in Log Writer Function\n";
	}
}

DataBaseAccess::DataBaseAccess() {
	try {
		mySqlConnection = new MYSQL();
		pthread_mutex_init(&database_mutex, NULL);
		Open();
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "DataBaseAccess", e);
	}
}

DataBaseAccess::~DataBaseAccess() {
	if (serverIp != NULL)
		free(serverIp);

}
bool DataBaseAccess::Open() {

	try {
		while(1){
			serverIp = (char *) malloc(sizeof(char) * 100);
			sprintf(serverIp, "%s", "localhost");
			hostName = "root"; //	area51			| root
			password = "root"; //	asdfgh			| password
			portNumber = 3306;
			schemaName = "NAS_V5";

			unsigned int conn_timeout = 80;
			mySqlConnection = mysql_init(NULL);
			mysql_options(mySqlConnection, MYSQL_OPT_CONNECT_TIMEOUT,
					&conn_timeout);
			if (mysql_real_connect(mySqlConnection, serverIp, hostName.c_str(),
					password.c_str(), schemaName.c_str(), portNumber,
					NULL, 0)) {
				return true;
			}

			if (  ISDEBUG )  cout<<"\n---------------DataBase Server not found----------------\n";
			//      }
			//exit(0);
			char errorMSG[1000];
			sprintf( errorMSG,"%s", mysql_error(mySqlConnection));
			//int errorMSG = mysql_error(mySqlConnection);
			//if (errorNo == MYSQL_ERROR_ACCESS_DENIED || errorNo == MYSQL_ERROR_SERVER_GONE || errorNo == MYSQL_ERROR_UNKNOWN_DB || MYSQL_E$
			if (  ISDEBUG )  cout<<"\nServerIP--->"<<serverIp;
			if (  ISDEBUG )  cout<<"\nHostName--->"<<hostName.c_str();
			if (  ISDEBUG )  cout<<"\nPassword--->"<<password.c_str();
			if (  ISDEBUG )  cout<<"\n---------------DataBase Server not found----------------\n";
			if (  ISDEBUG )  cout<<"\nMysql Error Msg:---->"<<errorMSG;
		}

	} catch (std::exception e) {
		PrintException("dataBaseAccess", "Open", e);
		return false;
	}
}

bool DataBaseAccess::OpenRecursion() {
	try {
		int recursionCount = QUERY_RECURSION_COUNT;
		unsigned int conn_timeout = 800;
		mySqlConnection = mysql_init(NULL);
		for (int i = 0; i < recursionCount; i++) {
			mysql_options(mySqlConnection, MYSQL_OPT_CONNECT_TIMEOUT,
					&conn_timeout);
			if (mysql_real_connect(mySqlConnection, serverName.c_str(),
					hostName.c_str(), password.c_str(), schemaName.c_str(),
					portNumber,
					NULL, 0)) {
				/*if (! mysql_query(mySqlConnection, "SET SESSION SQL_MODE = NO_AUTO_VALUE_ON_ZERO")){
				 if ( !mysql_query(mySqlConnection, "SET GLOBAL SQL_MODE = NO_AUTO_VALUE_ON_ZERO")){
				 return true;
				 }
				 }*/
				return true;
			} else
				continue;
		}
		if (  ISDEBUG )  cout<<"\n\nDataBase Server not found\n\n";
		//exit(0);
		//return false;

	} catch (std::exception e) {
		PrintException("dataBaseAccess", "OpenRecursion", e);
	}
	//exit(0);
	return false;
}

bool DataBaseAccess::ExecuteQueryWithConnectionOpenRecursion(char *query) {
	try {
		int recursionCount = QUERY_RECURSION_COUNT;
		if (OpenRecursion()) {
			for (int i = 0; i < recursionCount; i++) {
				if (!mysql_query(mySqlConnection, query))
					return true;
				else
					continue;
			}
			return false;
		} else
			return false;

	} catch (std::exception e) {
		PrintException("dataBaseAccess",
				"ExecuteQueryWithConnectionOpenRecursion", e);
	}
	return false;

}

bool DataBaseAccess::ExecuteQueryWithOutConnectionOpenRecursion(char *query) {
	char message[SIZEOFCHARARRAYBIGGER];
	try {
		int recursionCount = QUERY_RECURSION_COUNT;
		for (int i = 0; i < recursionCount; i++) {
			if (!mysql_query(mySqlConnection, query))
				return true;
			else
				continue;
		}

	} catch (std::exception e) {
		PrintException("dataBaseAccess",
				"ExecuteQueryWithOutConnectionOpenRecursion", e);
	}
	// sprintf( message, "\n%s\n",query,"Mysql Query Have Some Problem Please look it\n");
	// commClass->PrintLog( message );
	return false;
}

bool DataBaseAccess::StartQueryRecursion(char *query) {
	bool statusResult = false;
	try {

		int errorNo = mysql_errno(mySqlConnection);

		if (errorNo == MYSQL_ERROR_ACCESS_DENIED
				|| errorNo == MYSQL_ERROR_SERVER_GONE
				|| errorNo == MYSQL_ERROR_UNKNOWN_DB
				|| MYSQL_ERROR_CONNECTION_LOST)
			statusResult = ExecuteQueryWithConnectionOpenRecursion(query);
		else {
			statusResult = ExecuteQueryWithOutConnectionOpenRecursion(query);
		}
		return statusResult;

	} catch (std::exception e) {
		PrintException("dataBaseAccess", "StartQueryRecursion", e);
	}
	return statusResult;

}

void DataBaseAccess::Close() {

	try {
		mysql_commit(mySqlConnection);
		mysql_close(mySqlConnection);

	} catch (std::exception e) {
		PrintException("dataBaseAccess", "Close", e);
	}

}

bool DataBaseAccess::AuthenticateUser ( char* username, char* password,int* UID ) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,"select password,UID from userinfo where username = '%s' and status = 1",username);
		cout<<"\nAuthenticateUser"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				if (strcmp(row[0], password) == 0) {
					*UID = atoi(row[1]);
					return true;
				} else
					return false;
				break;
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetUGroupInfoFromUID(int UID, vector<int> *UGID,
		vector<string> *UGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select ugroupinfo.UGID,ugroupinfo.UGName from ugroupinfo inner join userugrouprelation where ugroupinfo.ugid=userugrouprelation.ugid and userugrouprelation.uid =%d ",
				UID);
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UGID->push_back(atoi(row[0]));
				UGName->push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetCGroupInfoFromUID(int UID, vector<int> *CGID,
		vector<string> *CGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		//sprintf(query,"select cgroupinfo.cgid,cgroupinfo.CGName from userugrouprelation inner join userugroupchannelcgrouprelation inner join channelcgrouprelation inner join cgroupinfo where userugrouprelation.uugid = userugroupchannelcgrouprelation.uugid and channelcgrouprelation.ccgid=userugroupchannelcgrouprelation.ccgid and cgroupinfo.cgid=channelcgrouprelation.cgid and userugrouprelation.uid =%d ",UID);
		sprintf(query,
				"select distinct(cgroupinfo.cgid),cgroupinfo.CGName from ugroupcgrouprelation inner join userugrouprelation inner join cgroupinfo where ugroupcgrouprelation.ugid = userugrouprelation.ugid and cgroupinfo.cgid = ugroupcgrouprelation.cgid  and userugrouprelation.uid=%d ",
				UID);

		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CGID->push_back(atoi(row[0]));
				CGName->push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetDeviceDetails(vector<int> &DID,
		vector<string> &macAddress, vector<string> &DeviceIP,
		vector<int> &status, vector<string> &deviceName,vector<int> &backup) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select DID,DEVICENAME,MACADDRESS,DEVICEIP,STATUS,BACKUP from deviceinfo");
		//	if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				DID.push_back(atoi(row[0]));
				deviceName.push_back(row[1]);
				macAddress.push_back(row[2]);
				DeviceIP.push_back(row[3]);
				status.push_back(atoi(row[4]));
				backup.push_back(atoi(row[5]));
			}
			return true;
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::CheckUserUGroupInfoExist(char * UGID, char * UID,
		int DID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		sprintf(query,"select * from userugrouprelation where UGID ='%s' and UID = '%s' and DID = '%d' ",
				UGID, UID, DID);
		if (  ISDEBUG )  cout<<"\nCheckUserUGroupInfoExist = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return row[0];
			}
		}
		return 0;
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return 0;
}

bool DataBaseAccess::CheckChannelCGroupInfoExist(char * CGID, char * CID,
		char * UID, int DID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		sprintf(query,
				"select * from channelcgrouprelation where CGID ='%s' and CID = '%s' and UID = '%s' and DID = '%d'",
				CGID, CID, UID, DID);
		if (  ISDEBUG )  cout<<"query = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return row[0];
			}
		}
		return 0;
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return 0;
}

bool DataBaseAccess::CheckAlreadyExist(char * value, char * tableName,
		char * attributeName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		sprintf(query, "select * from %s where %s ='%s'", tableName,
				attributeName, value);
		if (  ISDEBUG )  cout<<"query = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return true;
			}
		}
		return 0;
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return 0;
}

bool DataBaseAccess::GetDeviceID(int &DID, char *DeviceIP) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select DID from deviceinfo where DEVICEIP ='%s'",
				DeviceIP);
		if (  ISDEBUG )  cout<<"\nGetDeviceID = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				DID = atoi(row[0]);
				return true;
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetDeviceIP(char *deviceIp, int DID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select DEVICEIP from deviceinfo where DID ='%d' ", DID);
		if (  ISDEBUG )  cout<<"query = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				sprintf(deviceIp, "%s", row[0]);
				return true;
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetChannelInfoFromCGIDInteraction(bool isAllGroup,
		int CGID, vector<string> *CID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select CID from channelcgrouprelation where CGID=%d ",
				CGID);
		if (  ISDEBUG )  cout<<"query = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(row[0]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetChannelInfoFromCGID(int CGID, vector<string> *CID,vector<string> *ChannelName, vector<string> *RTSP, vector<string> *DID,vector<string> *status, vector<string> *storagestatus,vector<string> *RetrieverMode, vector<string> *ChannelMode,vector<string> *StoragePath, vector<string> *StorageSpace,vector<string> *StorageSpaceUsed, vector<string> *StorageFileType,vector<string> *ReStream,vector<string>*cameraIP , vector<string>*userName , vector<string>*portNumber , vector<string>*password, vector<string>*ptzControl, vector<string>*motion, vector<string>*description, vector<string>*make, vector<string>*model ) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select distinct(channelinfo.CID),channelinfo.ChannelName,channelinfo.RTSP,channelinfo.DID,channelinfo.status,channelinfo.storagestatus,channelinfo.retrivermode,channelinfo.REPLICAMODE,channelinfo.storagepath,channelinfo.storagespace,channelinfo.storagespaceused,channelinfo.storagefiletype,channelinfo.STREAMMODE,channelinfo.PTZCONTROL,channelinfo.MOTION,channelinfo.DESCRIPTION,channelinfo.IPADDRESS,channelinfo.PORTNUMBER,channelinfo.USERNAME,channelinfo.PASSWORD,channelinfo.MAKE,channelinfo.MODEL from cgroupinfo inner join channelcgrouprelation inner join channelinfo where cgroupinfo.cgid=channelcgrouprelation.cgid and channelcgrouprelation.cid = channelinfo.cid  and cgroupinfo.cgid =%d",
				CGID);
		if (  ISDEBUG )  cout<<"query = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(row[0]);
				ChannelName->push_back(row[1]);
				RTSP->push_back(row[2]);
				DID->push_back(row[3]);
				status->push_back(row[4]);
				storagestatus->push_back(row[5]);
				RetrieverMode->push_back(row[6]);
				ChannelMode->push_back(row[7]);
				StoragePath->push_back(row[8]);
				StorageSpace->push_back(row[9]);
				StorageSpaceUsed->push_back(row[10]);
				StorageFileType->push_back(row[11]);
				ReStream->push_back(row[12]);
				ptzControl->push_back(row[13]);
				motion->push_back(row[14]);
				description->push_back(row[15]);
				cameraIP->push_back(row[16]);
				portNumber->push_back(row[17]);
				userName->push_back(row[18]);
				password->push_back(row[19]);
				make->push_back(row[20]);
				model->push_back(row[21]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetAllChannelOnlyOwner(int sysDID, vector<int> *CID,
		vector<string> *ChannelName, vector<string> *RTSP, vector<int> *DID,
		vector<int> *status, vector<int> *storagestatus,
		vector<string> *RetrieverMode, vector<string> *ChannelMode,
		vector<string> *StoragePath, vector<string> *StorageSpace,
		vector<string> *StorageSpaceUsed, vector<string> *StorageFileType,
		vector<string> *ReStream, vector<string>*deviceIP,vector<string>*ptzControl
		,vector<string>*motion,vector<string>*description,vector<string>*ipAddress,vector<string>*PortNumber,vector<string>*userName,vector<string>*Password,vector<string>*make,vector<string>*model) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,"select T1.CID,T1.ChannelName,T1.RTSP,T1.DID,T1.status,T1.storagestatus,T1.retrivermode,T1.REPLICAMODE,T1.storagepath,T1.storagespace,T1.storagespaceused,T1.storagefiletype,T1.STREAMMODE,T2.DEVICEIP,T1.IPADDRESS,T1.USERNAME,T1.PORTNUMBER,T1.PASSWORD,T1.PTZCONTROL,T1.MOTION,T1.DESCRIPTION,T1.MAKE,T1.MODEL from channelinfo T1 inner join deviceinfo  T2 where T1.DID = T2.DID");
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(atoi(row[0]));
				ChannelName->push_back(row[1]);
				RTSP->push_back(row[2]);
				DID->push_back(atoi(row[3]));
				status->push_back(atoi(row[4]));
				storagestatus->push_back(atoi(row[5]));
				RetrieverMode->push_back(row[6]);
				ChannelMode->push_back(row[7]);
				StoragePath->push_back(row[8]);
				StorageSpace->push_back(row[9]);
				StorageSpaceUsed->push_back(row[10]);
				StorageFileType->push_back(row[11]);
				ReStream->push_back(row[12]);
				deviceIP->push_back(row[13]);
				ipAddress->push_back(row[14]);
				userName->push_back(row[15]);
				PortNumber->push_back(row[16]);
				Password->push_back(row[17]);
				ptzControl->push_back(row[18]);
				motion->push_back(row[19]);
				description->push_back(row[20]);
				make->push_back(row[21]);
				model->push_back(row[22]);


			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetAllChannelUsingDID(vector<int> *CID,vector<string> *ChannelName, vector<string> *RTSP, vector<int> *DID,vector<int> *status, vector<int> *storagestatus,vector<string> *RetrieverMode, vector<string> *ChannelMode,vector<string> *StoragePath, vector<string> *StorageSpace,vector<string> *StorageSpaceUsed, vector<string> *StorageFileType,vector<string> *ReStream, vector<string>*deviceIP, int DIDInfo, vector<string>*cameraIP , vector<string>*userName , vector<string>*portNumber , vector<string>*password, vector<string>*ptzControl, vector<string>*motion, vector<string>*description, vector<string>*make, vector<string>*model) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select T1.CID,T1.ChannelName,T1.RTSP,T1.DID,T1.status,T1.storagestatus,T1.retrivermode,T1.REPLICAMODE,T1.storagepath,T1.storagespace,T1.storagespaceused,T1.storagefiletype,T1.restream,T2.DEVICEIP,T1.IPADDRESS,T1.USERNAME,T1.PORTNUMNER,T1.PASSWORD,T1.PTZCONTROL,T1.MOTION,T1.DESCRIPTION,T1.MAKE,T1.MODEL from channelinfo T1 inner join deviceinfo T2 where T1.DID = T2.DID and T2.DID = %d",DIDInfo);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(atoi(row[0]));
				ChannelName->push_back(row[1]);
				RTSP->push_back(row[2]);
				DID->push_back(atoi(row[3]));
				status->push_back(atoi(row[4]));
				storagestatus->push_back(atoi(row[5]));
				RetrieverMode->push_back(row[6]);
				ChannelMode->push_back(row[7]);
				StoragePath->push_back(row[8]);
				StorageSpace->push_back(row[9]);
				StorageSpaceUsed->push_back(row[10]);
				StorageFileType->push_back(row[11]);
				ReStream->push_back(row[12]);
				deviceIP->push_back(row[13]);
				cameraIP->push_back(row[14]);
				userName->push_back(row[15]);
				portNumber->push_back(row[16]);
				password->push_back(row[17]);
				ptzControl->push_back(row[18]);
				motion->push_back(row[19]);
				description->push_back(row[20]);
				make->push_back(row[21]);
				model->push_back(row[22]);

			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}


bool DataBaseAccess::GetAllChannel(vector<int> *CID,vector<string> *ChannelName, vector<string> *RTSP, vector<int> *DID,vector<int> *status, vector<int> *storagestatus,vector<string> *RetrieverMode, vector<string> *ChannelMode,vector<string> *StoragePath, vector<string> *StorageSpace,vector<string> *StorageSpaceUsed, vector<string> *StorageFileType,vector<string> *ReStream, vector<string>*deviceIP , vector<string>*cameraIP , vector<string>*userName , vector<string>*portNumber , vector<string>*password, vector<string>*ptzControl, vector<string>*motion, vector<string>*description, vector<string>*make, vector<string>*model) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,"select T1.CID,T1.ChannelName,T1.RTSP,T1.DID,T1.status,T1.storagestatus,T1.retrivermode,T1.REPLICAMODE,T1.storagepath,T1.storagespace,T1.storagespaceused,T1.storagefiletype,T1.STREAMMODE,T2.DEVICEIP,T1.IPADDRESS,T1.USERNAME,T1.PORTNUMBER,T1.PASSWORD,T1.PTZCONTROL,T1.MOTION,T1.DESCRIPTION,T1.MAKE,T1.MODEL from channelinfo T1 inner join deviceinfo T2 where T1.DID = T2.DID");
		if (  ISDEBUG )  cout<<"\nGetAllChannel"<<query;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(atoi(row[0]));
				ChannelName->push_back(row[1]);
				RTSP->push_back(row[2]);
				DID->push_back(atoi(row[3]));
				status->push_back(atoi(row[4]));
				storagestatus->push_back(atoi(row[5]));
				RetrieverMode->push_back(row[6]);
				ChannelMode->push_back(row[7]);
				StoragePath->push_back(row[8]);
				StorageSpace->push_back(row[9]);
				StorageSpaceUsed->push_back(row[10]);
				StorageFileType->push_back(row[11]);
				ReStream->push_back(row[12]);
				deviceIP->push_back(row[13]);
				cameraIP->push_back(row[14]);
				userName->push_back(row[15]);
				portNumber->push_back(row[16]);
				password->push_back(row[17]);
				ptzControl->push_back(row[18]);
				motion->push_back(row[19]);
				description->push_back(row[20]);
				make->push_back(row[21]);
				model->push_back(row[22]);

			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetUserInfoFromUID(int UID, vector<string> *UName,
		vector<string> *UserName, vector<string> *Password,
		vector<string> *UserRole, vector<string> *Email, vector<string> *Mobile,
		vector<string>* Status) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select userinfo.Name,userinfo.username,userinfo.password,userinfo.userrole,userinfo.email,userinfo.ContactNo,userinfo.status from userinfo where userinfo.uid =%d ",
				UID);
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UName->push_back(row[0]);
				UserName->push_back(row[1]);
				Password->push_back(row[2]);
				UserRole->push_back(row[3]);
				Email->push_back(row[4]);
				Mobile->push_back(row[5]);
				Status->push_back(row[6]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::IsServerExist(char * deviceIP, char * DIDInfo, int &DID,
		char *getdeviceIP) {
	MYSQL_ROW row;
	int rowCount = 0;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];

	bool statusResult = false;
	try {
		if (  ISDEBUG )  cout<<"\nsa"<<endl;
		if (deviceIP == NULL)
			sprintf(query,
					"SELECT DID,DEVICEIP from deviceinfo where DID = '%s'",
					DIDInfo);
		else if (DIDInfo == NULL)
			sprintf(query,
					"SELECT DID,DEVICEIP from deviceinfo where DEVICEIP = '%s' ",
					deviceIP);
		else
			sprintf(query,
					"SELECT DID,DEVICEIP from deviceinfo where DEVICEIP = '%s' or DID = '%s'",
					deviceIP, DIDInfo);

		if (  ISDEBUG )  cout<<"\nsa"<<endl;
		if (  ISDEBUG )  cout<<"query = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			rowCount = mysql_num_rows(res);
			if (rowCount > 0) {

				while ((row = mysql_fetch_row(res)) != NULL) {
					if (  ISDEBUG )  cout<<"\nResult----->" << row[0] << endl;
					DID = atoi(row[0]);
					sprintf(getdeviceIP, "%s", row[1]);
					return true;
				}
			}
		}
		return false;
		//mysql_commit(mySqlConnection);
		//mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "IsServerExist", e);
	}
	return false;
}

bool DataBaseAccess::GetDeviceInfo(char * deviceName, char * deviceip, int &DID,
		char *macAddress, bool &backUP ) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,"select DID,MACADDRESS,DEVICENAME,BACKUP from deviceinfo where OWERES = 1 and DEVICEIP='%s'",deviceip);
		if (  ISDEBUG )  cout<<"query = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				DID = atoi(row[0]);
				sprintf(macAddress, "%s", row[1]);
				sprintf(deviceName, "%s", row[2]);
				backUP = atoi(row[3]);
				return true;
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
		return false;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

int DataBaseAccess::GetSystemDID() {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select DID from deviceinfo where OWERES = 1");
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return atoi(row[0]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return 0;
}

string DataBaseAccess::GetSystemIPFromDID(int DID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select DeviceIP from deviceinfo where DID = %d", DID);
		if (  ISDEBUG )  cout<<"query = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return row[0];
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return 0;
}

int DataBaseAccess::InsertUserInfo(int UID, char* name, char *userName,
		char *password, int userRole, char* email, char* mobile, int DID,
		bool status) {
	char query[3000];
	bool statusResult;
	//int id =0;
	try {
		sprintf(query,
				"insert into `userinfo` (`UID`,`NAME`,`USERNAME`,`PASSWORD`,`USERROLE`,`EMAIL`,`CONTACTNO`,`DID`,`STATUS`,`CREATETIMESTAMP`,`LASTMODIFIEDTIMESTAMP`) Values ('%d','%s','%s','%s',%d,'%s','%s',%d,%d,now(),now())",
				UID, name, userName, password, userRole, email, mobile, DID,
				status);
		if (  ISDEBUG )  cout<<"\nInsertUserInfo" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		//id = mysql_insert_id(mySqlConnection);
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

/*
 int DataBaseAccess::UpdateUserInfo(char* name, char *userName, char *password,int userRole,char* email,char* mobile,int DID,bool status) {
 char query[3000];
 bool statusResult;
 int id =0;
 try {
 sprintf(query,"update `userinfo` set NAME = '%s' ,USERNAME = '%s',PASSWORD ='%s',USERROLE = '%d',EMAIL= '%d',CONTACTNO = '%s',DID= '%d',STATUS= '%d',CREATETIMESTAMP= NOW(),LASTMODIFIEDTIMESTAMP= NOW())",name,userName,password,userRole,email,mobile,DID,status);
 //	cout<<"q"<<query<<endl;
 statusResult = !(mysql_query(mySqlConnection, query));
 id = mysql_insert_id(mySqlConnection);
 if (!statusResult) {
 statusResult = StartQueryRecursion (query);

 }
 mysql_commit(mySqlConnection);
 return id;
 }
 catch (std::exception e) {
 PrintException("dataBaseAccess","InsertUserInfo",e);
 }
 return 0;
 }
 */


int DataBaseAccess::CheckUserInfo( char *UserName,char *UID){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		sprintf(query, "select * from userinfo where USERNAME ='%s' and UID = '%s'", UserName, UID);
		if (  ISDEBUG )  cout<<"query = " << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return atoi(row[0]);
			}
		}
		return 0;
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return 0;
}

int DataBaseAccess::UpdateUserInfo(char* UID, char* name, char *userName,
		char *password, char* userRole, char* email, char* mobile,
		char* status) {
	char query[3000];
	bool statusResult;
	//int id =0;
	try {
		sprintf(query,
				"update `userinfo` set `NAME`='%s',`PASSWORD`='%s',`USERROLE`=%s,`EMAIL`='%s',`CONTACTNO`='%s',`STATUS`=%s,`LASTMODIFIEDTIMESTAMP`=now() where UID =%s",
				name, password, userRole, email, mobile, status, UID);
		if (  ISDEBUG )  cout<<"\nUpdateUserInfo->>>>>>" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}

		//return statusResult;
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::InsertDeviceInfo(int DID, char* DeviceName, char *DeviceIP,
		int Status, int Owner, char* MacId, bool isBackUp ) {
	char query[3000];
	bool statusResult;
	//int id =0;
	try {
		sprintf(query,
				"insert into `deviceinfo` (`DID`,`DEVICENAME`,`DEVICEIP`,`STATUS`,`OWERES`,`MACADDRESS`,`BACKUP`,`CREATETIMESTAMP`,`LASTMODIFIEDTIMESTAMP`) values ('%d','%s','%s',%d,%d,'%s','%d',now(),now())",
				DID, DeviceName, DeviceIP, Status, Owner, MacId,isBackUp);
		cout<<"\nInsertDeviceInfo---->" << query << endl;
		if (  ISDEBUG )  cout<<"\nInsertDeviceInfo---->" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		//id = mysql_insert_id(mySqlConnection);
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::UpdateDeviceInfo(char* DID, char* DeviceName,
		char *DeviceIP, char* Status,char * BackUP) {
	char query[3000];
	bool statusResult;
	int id = 0;
	try {
		sprintf(query,
				"update `deviceinfo` Set `DEVICENAME`='%s',`DEVICEIP`='%s',`STATUS`=%s, `BACKUP` = %s,`LASTMODIFIEDTIMESTAMP`=now() where DID =%s ",
				DeviceName, DeviceIP, Status, DID,BackUP );
		if (  ISDEBUG )  cout<<"\nUpdateDeviceInfo->>>>>>" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}


bool DataBaseAccess::InactiveChannel(char * CID ){
	char query[3000];
	bool statusResult;

	try {
		sprintf(query,"update channelinfo set STATUS = '0',LASTMODIFIEDTIMESTAMP = 'now()' where CID = '%s'", CID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::NewInsertChannelInfo(int CID, char * CName, int DID, char * MAKE,char * MODEL,char * USERNAME,char * PASSWORD,char *IPADDRESS,char *PORTNUMBER,char *RTSP,char *STORAGEPATH,char *STORAGESPACE,char *STORAGESPACEUSED,char *STORAGEFILETYPE,char *STORAGESTATUS ,char *STREAMMODE ,char *PTZCONTROL ,char *RETRIVERMODE,char *REPLICAMODE ,char *STATUS,char *MOTION, char *DESCRIPTION) {
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"insert into `channelinfo`(`CID`,`CHANNELNAME`,`DID`,`MAKE`,`MODEL`,`USERNAME`,`PASSWORD`,`IPADDRESS`,`PORTNUMBER`,`RTSP`,`STORAGEPATH`,`STORAGESPACE`,`STORAGESPACEUSED`,`STORAGEFILETYPE`,`STORAGESTATUS` ,`STREAMMODE` ,`PTZCONTROL` ,`RETRIVERMODE`,`REPLICAMODE` ,`STATUS`, `MOTION`, `DESCRIPTION` , `CREATETIMESTAMP` ,`LASTMODIFIEDTIMESTAMP`)  values ('%d','%s','%d','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',now(),now())",CID,CName, DID, MAKE, MODEL, USERNAME, PASSWORD,IPADDRESS,PORTNUMBER,RTSP,STORAGEPATH,STORAGESPACE,STORAGESPACEUSED,STORAGEFILETYPE,STORAGESTATUS ,STREAMMODE ,PTZCONTROL ,RETRIVERMODE,REPLICAMODE ,STATUS,MOTION, DESCRIPTION);
		if (  ISDEBUG )  cout<<"\nInsert channel--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}


int DataBaseAccess::InsertChannelInfo(int CID, char* ChannelName, char *RTSP,
		int Status, int DID, int RetrieverModeValue, int ChannelModeValue,
		char* StoragePath, char* StorageSpaceValue, char* StorageSpaceUsedValue,
		char* StorageFileTypeValue, int StorageStatusValue, int ReStreamValue) {
	char query[3000];
	bool statusResult;
	int id = 0;
	try {
		sprintf(query,"insert into `channelinfo` (`CID`,`ChannelName`,`RTSP`,`STATUS`,`DID`,`RETRIVERMODE`,`CHANNELMODE`,`STORAGEPATH`,`STORAGESPACE`,`STORAGESPACEUSED`,	`STORAGEFILETYPE`,`STORAGESTATUS`,`RESTREAM`,`CREATETIMESTAMP`,`LASTMODIFIEDTIMESTAMP`) values ('%d','%s','%s',%d,%d,%d,%d,'%s','%s','%s','%s',%d,%d,now(),now())",CID, ChannelName, RTSP, Status, DID, RetrieverModeValue,ChannelModeValue, StoragePath, StorageSpaceValue,StorageSpaceUsedValue, StorageFileTypeValue, StorageStatusValue,ReStreamValue);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::UpdateChannelInfo( int DID, char * CID, char * CName, char * MAKE,char * MODEL,char * USERNAME,char * PASSWORD,char *IPADDRESS,char *PORTNUMBER,char *RTSP,char *STORAGESPACE,char *STORAGESPACEUSED,char *STORAGEFILETYPE,char *STORAGESTATUS ,char *STREAMMODE ,char *PTZCONTROL ,char *RETRIVERMODE,char *REPLICAMODE ,char *STATUS,char *MOTION, char *DESCRIPTION) {
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"update `channelinfo` set `DID` = '%d' ,`CHANNELNAME` = '%s' ,`MAKE` = '%s' ,`MODEL` = '%s',`USERNAME` = '%s',`PASSWORD` = '%s',`IPADDRESS` = '%s',`PORTNUMBER` = '%s',`RTSP` = '%s',`STORAGESPACE` = '%s',`STORAGEFILETYPE` = '%s',`STORAGESTATUS` = '%s' ,`STREAMMODE` = '%s',`PTZCONTROL` = '%s',`RETRIVERMODE` = '%s',`REPLICAMODE` = '%s' ,`STATUS` = '%s', `MOTION` = '%s', `DESCRIPTION` = '%s', `LASTMODIFIEDTIMESTAMP`=now() where CID =%s ",DID,CName,  MAKE,MODEL,USERNAME,PASSWORD,IPADDRESS,PORTNUMBER,RTSP,STORAGESPACE,STORAGEFILETYPE,STORAGESTATUS ,STREAMMODE ,PTZCONTROL ,RETRIVERMODE,REPLICAMODE ,STATUS,MOTION, DESCRIPTION,CID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::UpdateCGroupInfo(int CGID, char* CGName, int DID) {
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"update `Cgroupinfo` set CGNAME = '%s',DID = '%d',LASTMODIFIEDTIMESTAMP= NOW() where CGID = '%d'",CGName, DID, CGID);
		statusResult = !(mysql_query(mySqlConnection, query));
		cout<<"\nUpdateCGroupInfo--->"<<query;
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		if (statusResult)
			return 1;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::UpdateUGroupInfo(int UGID, char* UGName, int DID) {
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"update `ugroupinfo` set UGNAME = '%s',DID = '%d',CREATETIMESTAMP = NOW(),LASTMODIFIEDTIMESTAMP= NOW() where UGID = '%d'",UGName, DID, UGID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		if (statusResult)
			return 1;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::InsertUGroupInfo(char* UGName, int DID, int UGID) {
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"insert into `ugroupinfo` (`UGID`,`UGNAME`,`DID`,`CREATETIMESTAMP`,`LASTMODIFIEDTIMESTAMP`) Values ('%d','%s',%d,now(),now())",UGID, UGName, DID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::InsertCGroupInfo(int CGID, char* CGName, int DID) {
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"insert into `cgroupinfo` (`CGID`,`CGNAME`,`DID`,`CREATETIMESTAMP`,`LASTMODIFIEDTIMESTAMP`) Values ('%d','%s',%d,now(),now())",CGID, CGName, DID);
		cout<<"\nInsert CGroup--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::InsertUserUGroupRelation(int UUGID, int UID, int UGID,
		int DID) {
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"insert into `userugrouprelation` (`UUGID`,`UID`,`UGID`,`DID`,`CREATETIMESTAMP`,`LASTMODIFIEDTIMESTAMP`) Values (%d,%d,%d,%d,now(),now());",UUGID, UID, UGID, DID);
		cout<<"\nInsertUserUGroupRelation--->"<<query<<endl;
		if (  ISDEBUG )  cout<<"\nInsertUserUGroupRelation--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::UpdateUserUGroupRelation(int UUGID, char* UID, char* UGID,
		int DID) {
	char query[3000];
	bool statusResult;
	//int id =0;
	try {
		sprintf(query,
				"update `userugrouprelation` set UID = '%s',UGID = '%s',DID = '%d',CREATETIMESTAMP = NOW(),LASTMODIFIEDTIMESTAMP = NOW() where UUGID = '%d'",
				UID, UGID, DID, UUGID);
		if (  ISDEBUG )  cout<<"\nUpdateChannelCGroupRelation->>>>" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		//id = mysql_insert_id(mySqlConnection);
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		if (statusResult)
			return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::UpdateChannelCGroupRelation(int CCGID, char* CID,
		char* CGID, int DID, char* UID) {
	char query[3000];
	bool statusResult;
	//int id =0;
	try {
		sprintf(query,
				"update `channelcgrouprelation` set CID = '%s',CGID = '%s',DID = '%d',UID = '%s',LASTMODIFIEDTIMESTAMP = NOW() where CCGID = '%d'",
				CID, CGID, DID, UID, CCGID);
		if (  ISDEBUG )  cout<<"\nUpdateChannelCGroupRelation->>>>" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		//id = mysql_insert_id(mySqlConnection);
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		if (statusResult)
			return 1;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

/*
 int DataBaseAccess::InsertChannelCGroupRelation(int CCGID, int CID,int CGID,int DID,int UID) {
 char query[3000];
 bool statusResult;
 int id =0;
 try {
 sprintf(query,"insert into `channelcgrouprelation` (`CCGID`,`CID`,`CGID`,`DID`,`UID`,`CREATETIMESTAMP`,`LASTMODIFIEDTIMESTAMP`) Values (%d,%d,%d,%d,%d,now(),now());",CCGID,CID,CGID,DID,UID);
 cout<<"\nInsertChannelCGroupRelation---->>>>>"<<query<<endl;
 statusResult = !(mysql_query(mySqlConnection, query));
 id = mysql_insert_id(mySqlConnection);
 if (!statusResult) {
 statusResult = StartQueryRecursion (query);

 }
 mysql_commit(mySqlConnection);
 return id;
 }
 catch (std::exception e) {
 PrintException("dataBaseAccess","InsertUserInfo",e);
 }
 return 0;
 }*/

int DataBaseAccess::DeleteUserUGroupRelation(int UID, int UGID) {
	char query[3000];
	bool statusResult;
	int id = 0;
	try {
		sprintf(query,
				"delete from `userugrouprelation` where UID =%d and UGID =%d",
				UID, UGID);
		if (  ISDEBUG )  cout<<"DeleteUserUGroupRelation" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::DeleteChannelCGroupRelation(int CID, int CGID) {
	char query[3000];
	bool statusResult;
	//int id =0;
	try {
		sprintf(query,
				"delete from `channelcgrouprelation` where CID =%d and CGID =%d",
				CID, CGID);
		if (  ISDEBUG )  cout<<"DeleteChannelCGroupRelation" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

int DataBaseAccess::InsertchannelCGroupRelation(int CCGID, int CID, int CGID,
		int DID, int UID) {
	char query[3000];
	bool statusResult;
	//int id =0;
	try {
		sprintf(query,
				"insert into `channelcgrouprelation` (`CCGID`,`CID`,`CGID`,`DID`,`UID`,`BLOCKING`,`CREATETIMESTAMP`,`LASTMODIFIEDTIMESTAMP`) Values (%d,%d,%d,%d,%d,0,now(),now())",
				CCGID, CID, CGID, DID, UID);
		if (  ISDEBUG )  cout<<"\nInsertchannelCGroupRelation"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		//id = mysql_insert_id(mySqlConnection);
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

//UpdateUGroupCGroupRelation(ret,UGID->value(),CGID->value(),UID,DID)


bool DataBaseAccess::UpdateUGroupCGroupRelation(int UGCGID, char* UGID, char* CGID,int DID, char* UID) {
	char query[3000];
	bool statusResult;
	//int id =0;
	try {
		if (  ISDEBUG )  cout<<"\nUpdate1"<<endl;
		sprintf(query,"update `ugroupcgrouprelation` set`UGID` = '%s',`CGID`= '%s',`DID`= '%d',`UID`= '%s',`LASTMODIFIEDTIMESTAMP` = Now() where UGCGID = '%d'",UGID, CGID, DID, UID,UGCGID);
		if (  ISDEBUG )  cout<<"\nugroupcgrouprelation--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		//id = mysql_insert_id(mySqlConnection);
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return false;
}



int DataBaseAccess::InsertUGroupCGroupRelation(int UGCGID, int UGID, int CGID,
		int DID, int UID) {
	char query[3000];
	bool statusResult;

	try {
		sprintf(query,"insert into ugroupcgrouprelation values(%d,%d,%d,%d,%d,0,now(),now())",UGCGID, UGID, CGID, UID, DID);
		cout<<"\nInsertUGroupCGroupRelation"<<query;
		if (  ISDEBUG )  cout<<"\n"<<query;
		statusResult = !(mysql_query(mySqlConnection, query));

		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

bool DataBaseAccess::GetAllActiveChannelInfo(vector<int> *CID,
		vector<string> *ChannelName, vector<string> *RTSP, vector<int> *DID,
		vector<string> *StoragePath, vector<int> *Retrivermode,
		vector<string> *StorageSpace, vector<string> *StorageSpaceUsed,
		vector<string> *StorageFileType, vector<int> *StorageStatus,
		vector<int> *streamMode, int SysDID ,vector<string> *IpAddress,vector<string> *portNumber , vector<string> *userName,vector<string> * password) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select channelinfo.CID,channelinfo.ChannelName,channelinfo.RTSP,channelinfo.DID,channelinfo.storagepath,channelinfo.RETRIVERMODE,channelinfo.StorageSpace,channelinfo.StorageSpaceUsed,channelinfo.StorageFileType,channelinfo.StorageStatus,channelinfo.StreamMode,channelinfo.IPADDRESS,channelinfo.PORTNUMBER,channelinfo.USERNAME,channelinfo.PASSWORD from channelinfo where channelinfo.status =1 and DID=%d ",
				SysDID);
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(atoi(row[0]));
				ChannelName->push_back(row[1]);
				RTSP->push_back(row[2]);
				DID->push_back(atoi(row[3]));
				StoragePath->push_back(row[4]);
				Retrivermode->push_back(atoi(row[5]));
				StorageSpace->push_back(row[6]);
				StorageSpaceUsed->push_back(row[7]);
				StorageFileType->push_back(row[8]);
				StorageStatus->push_back(atoi(row[9]));
				streamMode->push_back(atoi(row[10]));
				IpAddress->push_back((row[11]));
				portNumber->push_back((row[12]));
				userName->push_back((row[13]));
				password->push_back((row[14]));
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::GetActiveChannelInfoFromCID(int StartCID, vector<int> *CID,
		vector<string> *ChannelName, vector<string> *RTSP, vector<int> *DID,
		vector<string> *StoragePath, vector<int> *Retrivermode,
		vector<string> *StorageSpace, vector<string> *StorageSpaceUsed,
		vector<string> *StorageFileType, vector<int> *StorageStatus,
		vector<int> *ReStream, int SysDID,vector<string> *IpAddress,vector<string> *portNumber , vector<string> *userName,vector<string> * password) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select channelinfo.CID,channelinfo.ChannelName,channelinfo.RTSP,channelinfo.DID,channelinfo.storagepath,channelinfo.RETRIVERMODE,channelinfo.StorageSpace,channelinfo.StorageSpaceUsed,channelinfo.StorageFileType,channelinfo.StorageStatus,channelinfo.StreamMode,channelinfo.IPADDRESS,channelinfo.PORTNUMBER,channelinfo.USERNAME,channelinfo.PASSWORD from channelinfo where channelinfo.status =1 and DID=%d and CID =%d",
				SysDID, StartCID);
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(atoi(row[0]));
				ChannelName->push_back(row[1]);
				RTSP->push_back(row[2]);
				DID->push_back(atoi(row[3]));
				StoragePath->push_back(row[4]);
				Retrivermode->push_back(atoi(row[5]));
				StorageSpace->push_back(row[6]);
				StorageSpaceUsed->push_back(row[7]);
				StorageFileType->push_back(row[8]);
				StorageStatus->push_back(atoi(row[9]));
				ReStream->push_back(atoi(row[10]));
				IpAddress->push_back((row[11]));
				portNumber->push_back((row[12]));
				userName->push_back((row[13]));
				password->push_back((row[14]));
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

void DataBaseAccess::InsertVideoInfo(char *videoName, int CID, int DID,
		char *startDateTime, char *endDateTime, double videoSize) {
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,
				"insert into videoinfo ( VNAME, CID, DID, SIZE, CREATINGTIME, ENDINGTIME,CREATETIMESTAMP,LASTMODIFIEDTIMESTAMP ) values( '%s','%d','%d','%f','%s','%s',NOW(),NOW())",
				videoName, CID, DID, videoSize, startDateTime, endDateTime);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (  ISDEBUG )  cout<<"\nquery"<<query;
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		if (  ISDEBUG )  cout<<"\nStatus " << statusResult;
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertVideoInfo", e);
	}
}

void DataBaseAccess::GetVideoSize(int VID, double & size) {

	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select SIZE from videoinfo where VID = '%d'", VID);

		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		if (res) {
			rowCount = mysql_num_rows(res);
			if (rowCount > 0) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					sprintf(query, "%s", row[0]);
					size = atof(query);
				}
				mysql_free_result(res);
				res = NULL;
			}

		}
		mysql_commit(mySqlConnection);
	}

	catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoSize", e);
	}
}

bool DataBaseAccess::DeleteVideoFromTable(const char * tableName, int VID) {
	try {
		char query[SIZEOFCHARARRAYBIGGER];
		bool statusResult;
		sprintf(query, "DELETE from %s where VID = '%d'", tableName, VID);
		if (  ISDEBUG )  cout<<"Delete Query" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		if (statusResult) {
			if (  ISDEBUG )  cout<<"\nDeleted from " << tableName << "...." << VID << endl;
			return true;
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "DeleteVideoFromTable", e);

	}
	return false;
}

bool DataBaseAccess::DeleteVideoFromAllTable(int VID, double & size) {
	//	DeleteVideoFromTable("videostatus", VID);
	//DeleteVideoFromTable("featureinfo",VID);
	try {
		GetVideoSize(VID, size);
		//	if ( !DeleteVideoFromTable("videotiming",VID) )
		//		return false;
		if (!DeleteVideoFromTable("videoinfo", VID))
			return false;
	}

	catch (std::exception e) {
		PrintException("dataBaseAccess", "DeleteVideoFromAllTable", e);
	}
	//ResetTable("channelinfo");
	return true;
}

void DataBaseAccess::GetStorageSpaceUsed(int CID, int DID,double &storageSpaceUsed){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select STORAGESPACEUSED from channelinfo where CID = '%d' and DID = '%d'", CID,DID);

		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		if (res) {
			rowCount = mysql_num_rows(res);
			if (rowCount > 0) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					storageSpaceUsed =  atof(row[0]);
					break;
				}
				mysql_free_result(res);
				res = NULL;
			}

		}
		mysql_commit(mySqlConnection);
	}

	catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoSize", e);
	}
}

void DataBaseAccess::UpdateChannelStorageSpaceUsed(int CID, int DID,
		double storageSpaceUsed) {
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,
				"update channelinfo SET STORAGESPACEUSED = '%f',LASTMODIFIEDTIMESTAMP = 'now()' where CID = '%d' and DID = '%d'",
				storageSpaceUsed, CID, DID);
		if (  ISDEBUG )  cout<<"\n"<<query;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "UpdateChannelStorageSpaceUsed", e);
	}
}

void DataBaseAccess::GetVideoNameAndVID(vector<string> &videoName,
		vector<int> &vidInfo, int CID, int DID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,
				"select VNAME, VID from videoinfo where CID = '%d' and DID = '%d' order by CREATINGTIME",
				CID, DID);
		//sprintf( query,"select VNAME, VID from videoinfo where CID = %d",CID );
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				sprintf(query, "%s", row[0]);
				videoName.push_back(query);
				sprintf(query, "%s", row[1]);
				vidInfo.push_back(atoi(query));
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

bool DataBaseAccess::GetVIDForChannelAccordingTime(vector<string> &videoName,
		vector<int> &VID, vector<string> &VideoStartTime,
		vector<string> &VideoEndTime, int CID, int DID, char* StartTime,
		char* EndTime) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		if (  ISDEBUG )  cout<<"before query\n";
		sprintf(query,
				"SELECT VNAME,VID,CREATINGTIME,ENDINGTIME from videoinfo  where (( CREATINGTIME  between '%s' and '%s') or ( ENDINGTIME between '%s' and '%s') or ( CREATINGTIME <= '%s' and ENDINGTIME >= '%s')) ",
				StartTime, EndTime, StartTime, EndTime, StartTime, EndTime);
		sprintf(query, "%s and  ( CID = %d and DID = %d )", query, CID, DID);

		if (  ISDEBUG )  cout<<"q" << query << endl << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				videoName.push_back(row[0]);
				VID.push_back(atoi(row[1]));
				VideoStartTime.push_back(row[2]);
				VideoEndTime.push_back(row[3]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return 0;
}

void DataBaseAccess::GetVIDForSelectChannelAccordingTime(
		vector<string> &videoName, vector<int> &VID, vector<int> CID,
		vector<int> DID, char* StartTime, char* EndTime) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"SELECT VNAME,VID from videoinfo  where ( CREATINGTIME  between '%s' and '%s') or ( ENDINGTIME between '%s' and '%s') or ( CREATINGTIME <= '%s' and ENDINGTIME >= '%s') ",
				StartTime, EndTime, StartTime, EndTime, StartTime, EndTime);
		for (int i = 0; i < CID.size(); i++) {
			if (i < 1)
				sprintf(query, "%s and  ( CID = '%s' and DID = '%s')", query,
						CID[i], DID[i]);
			else
				sprintf(query, "%s or ( CID = '%s' and DID = '%s')", query,
						CID[i], DID[i]);
		}
		sprintf(query, "%s )", query);

		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				sprintf(query, "%s", row[0]);
				videoName.push_back(query);
				sprintf(query, "%s", row[1]);
				VID.push_back(atoi(query));
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

void DataBaseAccess::GetAllUserOnlyOwners(vector<string> &DID, vector<string> *UID,
		vector<string> *Name, vector<string> *UName, vector<string> *Password,
		vector<string> *UserRole, vector<string> *Email, vector<string> *Mobile,
		vector<string> *Status) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select userinfo.UID,userinfo.Name,userinfo.username,userinfo.password,userinfo.userrole,userinfo.email,userinfo.ContactNo,userinfo.status,userinfo.DID from userinfo ");
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UID->push_back(row[0]);
				Name->push_back(row[1]);
				UName->push_back(row[2]);
				Password->push_back(row[3]);
				UserRole->push_back(row[4]);
				Email->push_back(row[5]);
				Mobile->push_back(row[6]);
				Status->push_back(row[7]);
				DID.push_back(row[8]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
}

void DataBaseAccess::GetAllUser(vector<string> *UID, vector<string> *Name,
		vector<string> *UName, vector<string> *Password,
		vector<string> *UserRole, vector<string> *Email, vector<string> *Mobile,
		vector<string> *Status) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select userinfo.UID,userinfo.Name,userinfo.username,userinfo.password,userinfo.userrole,userinfo.email,userinfo.ContactNo,userinfo.status from userinfo");
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UID->push_back(row[0]);
				Name->push_back(row[1]);
				UName->push_back(row[2]);
				Password->push_back(row[3]);
				UserRole->push_back(row[4]);
				Email->push_back(row[5]);
				Mobile->push_back(row[6]);
				Status->push_back(row[7]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
}		//GetAllUGroupOnlyUser

void DataBaseAccess::GetAllUGroupOnlyUser(vector<string> &DID, vector<string> *UGID,
		vector<string> *UGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select Ugid,ugname,DID from ugroupinfo");
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UGID->push_back(row[0]);
				UGName->push_back(row[1]);
				DID.push_back(row[2]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
}

void DataBaseAccess::GetAllUGroup(vector<string> *UGID,
		vector<string> *UGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select Ugid,ugname from ugroupinfo");
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UGID->push_back(row[0]);
				UGName->push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
}

void DataBaseAccess::GetAllCGroupOnlyOwner(vector<string> &DID, vector<string> *CGID,
		vector<string> *CGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select cgid,cgname,DID from cgroupinfo ");
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CGID->push_back(row[0]);
				CGName->push_back(row[1]);
				DID.push_back(row[2]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
}
void DataBaseAccess::GetAllCGroup(vector<string> *CGID,
		vector<string> *CGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select cgid,cgname from cgroupinfo");
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CGID->push_back(row[0]);
				CGName->push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
}

void DataBaseAccess::GetAllVideoInfo(vector<int> *VID, vector<string> *VName,vector<int> *CID,vector<int> *DID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query, "select VID,VNAME,CID,DID  from videoinfo");
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				VID->push_back(atoi(row[0]));
				VName->push_back(row[1]);
				CID->push_back(atoi(row[2]));
				DID->push_back(atoi(row[3]));
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

void DataBaseAccess::GetAllDeviceInfo(vector<string> *DID,
		vector<string> *DName, vector<string> *DeviceIP, vector<string> *Status,
		vector<string> *Owner, vector<string> *MacAddress,vector<string> *backup, bool withOwner ) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		if ( withOwner )
			sprintf(query,"select DID,devicename,deviceip,status,oweres,macaddress,BACKUP from deviceinfo");
		else
			sprintf(query,"select DID,devicename,deviceip,status,oweres,macaddress,BACKUP from deviceinfo where oweres != 1");

		cout<<"\nGetAllDeviceInfo--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				DID->push_back(row[0]);
				DName->push_back(row[1]);
				DeviceIP->push_back(row[2]);
				Status->push_back(row[3]);
				Owner->push_back(row[4]);
				MacAddress->push_back(row[5]);
				backup->push_back(row[6]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

void DataBaseAccess::GetAllUserUGroupRelationOnlyOwners(vector<string> &DID,
		vector<string> *UUGID, vector<string> *UID, vector<string> *UGID,
		vector<string> *UGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select T1.UUGID,T1.UID,T1.UGID,T2.UGNAME,T2.DID from userugrouprelation T1 inner join ugroupinfo T2 where T1.UGID = T2.UGID ");
		//sprintf( query,"select VNAME, VID from videoinfo where CID = %d",CID );
		if (  ISDEBUG )  cout<<"\nQuery--->"<<query;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UUGID->push_back(row[0]);
				UID->push_back(row[1]);
				UGID->push_back(row[2]);
				UGName->push_back(row[3]);
				DID.push_back(row[4]);

			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

void DataBaseAccess::GetAllUserUGroupRelation(vector<string> *UID,
		vector<string> *UGID, vector<string> *UGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,
				"select userugrouprelation.UID,userugrouprelation.UGID,(select ugroupinfo.UGName from ugroupinfo where ugroupinfo.ugid=userugrouprelation.ugid) as UGName  from userugrouprelation");
		//sprintf( query,"select VNAME, VID from videoinfo where CID = %d",CID );
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UID->push_back(row[0]);
				if (  ISDEBUG )  cout<<"\nRow[0]---->" << row[0];
				if (  ISDEBUG )  cout<<"\nRow[1]---->" << row[1];
				if (  ISDEBUG )  cout<<"\nRow[2]---->" << row[2];
				UGID->push_back(row[1]);
				UGName->push_back(row[2]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

void DataBaseAccess::GetAllChannelCGroupRelationOnlyOwners(vector<string> &DID,
		vector<string> *CCGID, vector<string> *CID, vector<string> *CGID,
		vector<string> *CGName, vector<int> *UID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,"select T1.CCGID, T1.CID,T1.CGID,T2.CGNAME,T1.UID, T1.DID from channelcgrouprelation T1 inner join cgroupinfo T2 where T1.CGID = T2.CGID ");
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CCGID->push_back(row[0]);
				CID->push_back(row[1]);
				CGID->push_back(row[2]);
				CGName->push_back(row[3]);
				UID->push_back(atoi(row[4]));
				DID.push_back(row[5]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

void DataBaseAccess::GetAllChannelCGroupRelation(vector<string> *CID,
		vector<string> *CGID, vector<string> *CGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,
				"select channelcgrouprelation.CID,channelcgrouprelation.CGID,(select cgroupinfo.CGName from cgroupinfo where cgroupinfo.cgid=channelcgrouprelation.cgid) as CGName  from channelcgrouprelation");
		//sprintf( query,"select VNAME, VID from videoinfo where CID = %d",CID );
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(row[0]);
				CGID->push_back(row[1]);
				CGName->push_back(row[2]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

int DataBaseAccess::UpdateDumpingStatus(char* CID, char* DID, int status) {
	char query[3000];
	bool statusResult;
	int id = 0;
	try {
		sprintf(query,
				"update channelinfo set storagestatus=%d,LASTMODIFIEDTIMESTAMP=now() where CID=%s and DID=%s",
				status, CID, DID);
		if (  ISDEBUG )  cout<<"q" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

//Ashu
bool DataBaseAccess::GetAllAvailableNASDetail(vector<string> & nasIP) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query, "select DEVICEIP from deviceinfo where OWERES = 0");
		if (  ISDEBUG )  cout<<"\nQuery" << query;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				nasIP.push_back(row[0]);
			}
			mysql_free_result(res);
			res = NULL;
			return true;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::isMacAddressExist(char * macAddress, char * serverIP ) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"select DEVICEIP from deviceinfo where OWERES = 1 and MACADDRESS = '%s'",macAddress);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				sprintf( serverIP,"%s", row[0]);
				return true;
			}
			mysql_free_result(res);
			res = NULL;
		}
		return false;
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::InsertOwnerDeviceIp(char * macAddress, int DID,
		char * ip) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,
				"insert into deviceinfo values('%d','%downerNAS','%s','1','1','%s','0',NOW(),NOW())",
				DID, DID, ip, macAddress);
		if (  ISDEBUG )  cout<<"\nInsertOwnerDeviceIp--->" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		if (statusResult)
			return true;
		else
			return false;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

int DataBaseAccess::GetRowCount(const char * tableName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,
				"SELECT COUNT(*) FROM %s T1 inner join deviceinfo T2 where T2.DID = T1.DID and T2.OWERES = 1",
				tableName);
		if (  ISDEBUG )  cout<<"\nGetRowCount" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return atoi(row[0]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
		return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::RemoveChannelFromCGID(int CGID, vector<int> removeCID,
		int UID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	char tempQuery[SIZEOFCHARARRAYBIGGER];

	bool statusResult = false;
	try {

		if (removeCID.size() > 0) {

			for (int i = 0; i < (int)removeCID.size(); i++) {
				if (i == 0)
					sprintf(tempQuery,"delete from channelcgrouprelation where (CGID = '%d' and CID = '%d')",
							CGID, removeCID[i]);
				else
					sprintf(tempQuery, " %s or (CGID = '%d' and CID = '%d' )", query,
							CGID, removeCID[i]);
				sprintf( query , "%s", tempQuery );
				if (  ISDEBUG )  cout<<"\nQuery"<<query;
			}
			if (  ISDEBUG )  cout<<"\nRemoveChannelFromCGID--->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}

			mysql_commit(mySqlConnection);
			return statusResult;
		} else
			return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::AddChannelToCGID(int CGID, vector<int> addCID, int DID,
		int UID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	char temp[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		if (addCID.size() > 0) {
			for (int i = 0; i < addCID.size(); i++) {
				if (i == 0)
					sprintf(temp,
							"insert into channelcgrouprelation values ('','%d','%d','%d','%d','0',NOW(),NOW())",
							addCID[i], CGID, DID, UID);
				else
					sprintf(temp,
							" %s ,('','%d','%d','%d','%d','0',NOW(),NOW())",
							query, addCID[i], CGID, DID, UID);
				sprintf(query, "%s", temp);
			}
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult)
				statusResult = StartQueryRecursion(query);
			mysql_commit(mySqlConnection);
			return statusResult;
		} else
			return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::RemoveUserFromUGID(int UGID, vector<int> removeUID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	char temp[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		if (  ISDEBUG )  cout<<"\nsize is addUID"<<removeUID.size();
		if (removeUID.size() > 0) {
			for (int i = 0; i < (int)removeUID.size(); i++) {
				if (i == 0)
					sprintf(temp, "delete from userugrouprelation where (UGID = '%d' and UID = '%d')",UGID, removeUID[i]);
				else
					sprintf(temp, " %s or (UGID = '%d' and UID = '%d')", query,UGID, removeUID[i]);
				sprintf ( query,"%s", temp);
			}
			if (  ISDEBUG )  cout<<"\nRmoveUserGroup--->"<<query;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}

			mysql_commit(mySqlConnection);
			return statusResult;
		} else{
			if (  ISDEBUG )  cout<<"\ndashjkdhnk RemoveUserFromUGID\n";
			return true;
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::AddUserToUGID(int UGID, vector<int> addUID, int DID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		if (  ISDEBUG )  cout<<"\nsize is addUID"<<addUID.size();
		if (addUID.size() > 0) {
			for (int i = 0; i < addUID.size(); i++) {
				if (i == 0)
					sprintf(query,
							"insert into userugrouprelation values ('','%d','%d','%d',NOW(),NOW())",
							addUID[i], UGID, DID);
				else
					sprintf(query, "%s ,('','%d','%d','%d',NOW(),NOW())", query,
							addUID[i], UGID, DID);
				if (  ISDEBUG )  cout<<"\nQuery"<<query;

			}
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			mysql_commit(mySqlConnection);
			return statusResult;
		}
		else
			return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

//Ashu

int DataBaseAccess::CheckUGroupCGroupInfoExist( char *UGID ,char * CGID, int DID, int UID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,"select UGCGID from ugroupcgrouprelation where UGID = '%s' and CGID = '%s' and DID = '%d' and UID = '%d'",UGID , CGID,DID,UID);
		if (  ISDEBUG )  cout<<"\nQuery"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if ( rowCount > 0 ){
			while ((row = mysql_fetch_row(res)) != NULL) {
				if (  ISDEBUG )  cout<<"\nAvailabel\n"<<endl;
				return atoi(row[0]);
			}
			mysql_free_result(res);
			res = NULL;
		}

		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
	return 0;
}


bool DataBaseAccess::InsertCGroupUGroupRelation(int CCGID, vector<int> addUUGID,int DID, int UID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	char temp[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		if (  ISDEBUG )  cout<<"\nsize is addUUGID"<<addUUGID.size();
		if (addUUGID.size() > 0) {
			//sprintf(query,"insert into from ugroupcgrouprelation where");
			for (int i = 0; i < addUUGID.size(); i++) {
				if (i == 0)
					sprintf(temp,
							"insert into ugroupcgrouprelation values ('','%d','%d','%d','%d','0',NOW(),NOW())",
							addUUGID[i], CCGID, DID, UID);
				else
					sprintf(temp,
							" %s , ('','%d','%d','%d','%d','0',NOW(),NOW())",
							query, addUUGID[i], CCGID, DID, UID);
				sprintf(query, "%s", temp);
			}
			if (  ISDEBUG )  cout<<"\nQuery"<<query;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}

			mysql_commit(mySqlConnection);
			return statusResult;
		} else
			return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::InsertUGroupCGroupRelation(int UGID, vector<int> addCGID,
		int DID, int UID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	char temp[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		if (  ISDEBUG )  cout<<"\nsize is addUGID"<<addCGID.size();
		if (addCGID.size() > 0) {
			//sprintf(query,"insert into from ugroupcgrouprelation where");
			for (int i = 0; i < (int) addCGID.size(); i++) {
				if (i == 0)
					sprintf(temp,
							"insert into ugroupcgrouprelation values ('','%d','%d','%d','%d','0',NOW(),NOW())",
							UGID, addCGID[i], DID, UID);
				else
					sprintf(temp,
							" %s ,('','%d','%d','%d','%d','0',NOW(),NOW())",query, UGID, addCGID[i], DID, UID);
				sprintf(query, "%s", temp);
			}
			cout<<"\nInsertUGroupCGroupRelation---->"<<query<<endl;
			if (  ISDEBUG )  cout<<"\nInsertUGroupCGroupRelation"<<query;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}

			mysql_commit(mySqlConnection);
			return statusResult;
		} else
			return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::RemoveCGroupUGroupRelation(int CGID,
		vector<int> removeUGID, int DID, int UID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	char temp[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		if (  ISDEBUG )  cout<<"\nsize is RemoveUUGID"<<removeUGID.size();
		if (removeUGID.size() > 0) {

			for (int i = 0; i < (int) removeUGID.size(); i++) {
				if (i == 0)
					sprintf(temp,
							"delete from ugroupcgrouprelation where (`UGID` = '%d' and CGID = '%d' and DID = '%d')",
							removeUGID[i], CGID, DID);
				else
					sprintf(temp,
							" %s or (`UGID` = '%d' and CGID = '%d' and DID = '%d')", query,
							removeUGID[i], CGID, DID);
				sprintf(query, "%s", temp);
			}
			if (  ISDEBUG )  cout<<"\nQuery"<<query;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}

			mysql_commit(mySqlConnection);
			return statusResult;
		} else
			return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::RemoveUGroupCGroupRelation(int UGID,
		vector<int> removeCGID, int DID, int UID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	char temp[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		 cout<<"\nsize is RemoveCGID"<<removeCGID.size();
		if (  ISDEBUG )  cout<<"\nsize is RemoveCGID"<<removeCGID.size();
		if (removeCGID.size() > 0) {

			for (int i = 0; i < (int) removeCGID.size(); i++) {
				if (i == 0)
					sprintf(temp,"delete from ugroupcgrouprelation where (`UGID` = '%d' and CGID = '%d' and DID = '%d')",
							UGID, removeCGID[i], DID);
				else
					sprintf(temp,
							" %s or (`UGID` = '%d' and CGID = '%d' and DID = '%d')", query,
							UGID, removeCGID[i], DID);
				sprintf(query, "%s", temp);
				cout<<"\nQuery---->"<<query<<endl;
			}
			cout<<"\nRemoveUGroupCGroupRelation"<<query;
			if (  ISDEBUG )  cout<<"\nQuery"<<query;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}

			mysql_commit(mySqlConnection);
			return statusResult;
		} else
			return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

/*
 bool DataBaseAccess::InsertCCGroupUUGroupRelation(int CGID, vector<int> addUGID,int DID, int UID ){
 MYSQL_ROW row;
 MYSQL_RES *res = NULL;
 int rowCount = 0;
 char query[SIZEOFCHARARRAYBIGGER];
 char temp[SIZEOFCHARARRAYBIGGER];
 bool statusResult = false;
 try {
 cout<<"\nsize is addUGID"<< addUGID.size();
 if ( addUGID.size() > 0 ){
 //sprintf(query,"insert into from userugroupchannelcgrouprelation where");
 for ( int i = 0 ; i < addUGID.size(); i++ ){
 if ( i == 0 )
 sprintf(temp,"insert into userugroupchannelcgrouprelation values ('','%d','%d','%d','%d','0',NOW(),NOW())",addUGID[i],CGID,DID,UID);
 else
 sprintf(temp," %s ('','%d','%d','%d','%d','0',NOW(),NOW())",query, addUGID[i],CGID,DID,UID);
 sprintf( query ,"%s",temp);
 }
 cout<<"\nQuery"<<query;
 statusResult = !(mysql_query(mySqlConnection, query));
 if (!statusResult) {
 statusResult = StartQueryRecursion (query);
 }

 mysql_commit(mySqlConnection);
 return statusResult;
 }
 else
 return true;
 }
 catch (std::exception e) {
 PrintException("dataBaseAccess","GetVideoNameAndVID",e);
 }
 return false;
 }

 bool DataBaseAccess::InsertUUGroupCCGroupRelation(int UUGID, vector<int> addCCGID,int DID, int UID){
 MYSQL_ROW row;
 MYSQL_RES *res = NULL;
 int rowCount = 0;
 char query[SIZEOFCHARARRAYBIGGER];
 char temp[SIZEOFCHARARRAYBIGGER];
 bool statusResult = false;
 try {
 cout<<"\nsize is addUUGID"<<addCCGID.size();
 if ( addCCGID.size() > 0 ){
 //sprintf(query,"insert into from userugroupchannelcgrouprelation where");
 for ( int i = 0 ; i < addCCGID.size(); i++ ){
 if ( i == 0 )
 sprintf(temp,"insert into userugroupchannelcgrouprelation values ('','%d','%d','%d','%d','0',NOW(),NOW())",UUGID,addCCGID[i],DID,UID);
 else
 sprintf(temp," %s ('','%d','%d','%d','%d','0',NOW(),NOW())",query, UUGID,addCCGID[i],DID,UID);
 sprintf( query ,"%s",temp);
 }
 cout<<"\nQuery"<<query;
 statusResult = !(mysql_query(mySqlConnection, query));
 if (!statusResult) {
 statusResult = StartQueryRecursion (query);
 }

 mysql_commit(mySqlConnection);
 return statusResult;
 }
 else
 return true;
 }
 catch (std::exception e) {
 PrintException("dataBaseAccess","GetVideoNameAndVID",e);
 }
 return false;
 }

 bool DataBaseAccess::RemoveCCGroupUUGroupRelation(int CCGID, vector<int> removeUUGID,int DID, int UID){
 MYSQL_ROW row;
 MYSQL_RES *res = NULL;
 int rowCount = 0;
 char query[SIZEOFCHARARRAYBIGGER];
 char temp[SIZEOFCHARARRAYBIGGER];
 bool statusResult = false;
 try {
 cout<<"\nsize is RemoveUUGID"<<removeUUGID.size();
 if ( removeUUGID.size() > 0 ){

 for ( int i = 0 ; i < removeUUGID.size(); i++ ){
 if ( i == 0 )
 sprintf(temp,"delete from userugroupchannelcgrouprelation where (`UUGID` = '%d' and CCGID = '%d' and DID = '%d')",removeUUGID[i],CCGID,DID);
 else
 sprintf(temp," %s or (`UUGID` = '%d' and CCGID = '%d' and DID = '%d')",removeUUGID[i],CCGID,DID);
 sprintf( query ,"%s",temp);
 }
 cout<<"\nQuery"<<query;
 statusResult = !(mysql_query(mySqlConnection, query));
 if (!statusResult) {
 statusResult = StartQueryRecursion (query);
 }

 mysql_commit(mySqlConnection);
 return statusResult;
 }
 else
 return true;
 }
 catch (std::exception e) {
 PrintException("dataBaseAccess","GetVideoNameAndVID",e);
 }
 return false;
 }

 bool DataBaseAccess::RemoveUUGroupCCGroupRelation(int UUGID, vector<int> removeCCGID,int DID, int UID){
 MYSQL_ROW row;
 MYSQL_RES *res = NULL;
 int rowCount = 0;
 char query[SIZEOFCHARARRAYBIGGER];
 char temp[SIZEOFCHARARRAYBIGGER];
 bool statusResult = false;
 try {
 cout<<"\nsize is RemoveUUGID"<<removeCCGID.size();
 if ( removeCCGID.size() > 0 ){

 for ( int i = 0 ; i < removeCCGID.size(); i++ ){
 if ( i == 0 )
 sprintf(temp,"delete from userugroupchannelcgrouprelation where (`UUGID` = '%d' and CCGID = '%d' and DID = '%d')",UUGID,removeCCGID[i],DID);
 else
 sprintf(temp," %s or (`UUGID` = '%d' and CCGID = '%d' and DID = '%d')",UUGID,removeCCGID[i],DID);
 sprintf( query ,"%s",temp);
 }
 cout<<"\nQuery"<<query;
 statusResult = !(mysql_query(mySqlConnection, query));
 if (!statusResult) {
 statusResult = StartQueryRecursion (query);
 }

 mysql_commit(mySqlConnection);
 return statusResult;
 }
 else
 return true;
 }
 catch (std::exception e) {
 PrintException("dataBaseAccess","GetVideoNameAndVID",e);
 }
 return false;
 }*/

bool DataBaseAccess::InsertUGroupRelationWithCGroup(int UGID,
		vector<int> addCGID, vector<int> removeCGID, int UID, int DID) {
	bool result = false;
	if ( addCGID.size() > 0 ){
		if (InsertUGroupCGroupRelation(UGID, addCGID, DID, UID) ){
			result = true;
			cout<<"\nresutl1"<<endl;
		}
	}
	else{
		result = true;
		cout<<"\nresutl2"<<endl;
	}
	cout<<"\n1111"<<endl;
	cout<<"\nresutl3"<<endl;
	if ( removeCGID.size() > 0 )
			if (RemoveUGroupCGroupRelation(UGID, removeCGID, DID, UID)) {
				cout<<"\nresutl4"<<endl;
				if ( result )
					return true;
	}
	else if ( result ){

		return true;
	}

	return false;
}

bool DataBaseAccess::InsertCGroupRelationWithUGroup(int CGID,
		vector<int> addUGID, vector<int> removeUGID, int UID, int DID) {
	if (InsertCGroupUGroupRelation(CGID, addUGID, DID, UID)) {
		if (  ISDEBUG )  cout<<"\n  1  Return True InsertCGroupRelationWithUGroup\n";
		if (RemoveCGroupUGroupRelation(CGID, removeUGID, DID, UID)) {
			if (  ISDEBUG )  cout<<"\n  2  Return True InsertCGroupRelationWithUGroup\n";
			return true;
		}
	}
	return false;
}

/*
 bool DataBaseAccess::InsertUUGroupRelationWithCCGroup(int UUGID,vector<int> addCCGID,vector<int>removeCCGID, int UID, int DID){
 if ( InsertUUGroupCCGroupRelation(UUGID, addCCGID,DID,UID) ){
 if (RemoveUUGroupCCGroupRelation(UUGID,removeCCGID,DID,UID )){
 return true;
 }
 }
 return false;
 }

 bool DataBaseAccess::InsertCCGroupRelationWithUUGroup(int CCGID,vector<int> addUUGID,vector<int>removeUUGID, int UID, int DID){
 if ( InsertCCGroupUUGroupRelation(CCGID, addUUGID,DID,UID) ){
 if (RemoveCCGroupUUGroupRelation(CCGID,removeUUGID,DID,UID )){
 return true;
 }
 }
 return false;
 }*/

bool DataBaseAccess::GetUGroupCGroupRelationOnlyOwners(vector<string> &UGID,
		vector<string> &CGID,vector<string> &UGCGID,vector<string> &UID, vector<string> & DID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT UGID,CGID,UGCGID,UID,DID FROM ugroupcgrouprelation ");
		if (  ISDEBUG )  cout<<"\nQuery--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UGID.push_back(row[0]);
				CGID.push_back(row[1]);
				UGCGID.push_back(row[2]);
				UID.push_back(row[3]);
				DID.push_back(row[4]);
			}
			mysql_free_result(res);
			res = NULL;
			return true;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}



bool DataBaseAccess::GetUGroupCGroupRelation(vector<string> &UGID,
		vector<string> &CGID, vector<string> &CGNAME, vector<string> &UGNAME) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT T1.UGID,T1.CGID,T2.CGName,T3.UGName FROM ugroupcgrouprelation T1 inner join cgroupinfo T2 inner join ugroupinfo T3 where T1.CGID = T2.CGID and T1.UGID = T3.UGID ;");
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UGID.push_back(row[0]);
				CGID.push_back(row[1]);
				CGNAME.push_back(row[2]);
				UGNAME.push_back(row[3]);
			}
			mysql_free_result(res);
			res = NULL;
			return true;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

void DataBaseAccess::GetAllFromChannelRTSP( vector<int> *RESTREAMID,vector<int> *CID,vector<int> *DID,vector<string> *width,vector<string> *height,vector<string> *bitrate,vector<string> *framerate)
{
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,"select restreamid,cid,did,width,height,bitrate,framerate from channelrtsp");
		if (  ISDEBUG )  cout<<"query for the fetch channelrtsp="<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if ( rowCount > 0 ){
			while ((row = mysql_fetch_row(res)) != NULL) {
				RESTREAMID->push_back(atoi(row[0]));
				CID->push_back(atoi(row[1]));
				DID->push_back(atoi(row[2]));
				width->push_back(row[3]);
				height->push_back(row[4]);
				bitrate->push_back(row[5]);
				framerate->push_back(row[6]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
}


int DataBaseAccess::InsertReStream( char *CID, char *DID, char *width,char * height, char * frameRate,char * bitRate ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0, id = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,"insert into channelrtsp values('','%s','%s','%s','%s','%s','%s',NOW(),NOW()) ",CID, DID, width, height,bitRate, frameRate );
		if (  ISDEBUG )  cout<<"\nQuery--->"<<query;
		statusResult = !(mysql_query(mySqlConnection, query));
		id = mysql_insert_id(mySqlConnection);
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		mysql_free_result(res);
		return id;
		mysql_commit(mySqlConnection);
	}
	catch (Exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
	return id;
}

void DataBaseAccess::InsertIntoProcessMonitor(double ProcessCpuUtilisation,double OverallCpuUtilisation,int ProcessVirtualMemoryUtilisation,int ProcessPhysicalMemoryUtilisation, unsigned long long int DataSent,unsigned long long int  DataReceived){
	char query[4000];
	bool statusResult;
	try {
		sprintf(query ,"insert into ProcessMonitor ( ProcessCpuUtilisation, OverallCpuUtilisation , ProcessVirtualMemoryUtilisation, ProcessPhysicalMemoryUtilisation, DataSent, DataReceived, EntryDateTime) values( %f, %f, %d, %d, %u, %u,  NOW() )", ProcessCpuUtilisation, OverallCpuUtilisation , ProcessVirtualMemoryUtilisation, ProcessPhysicalMemoryUtilisation, DataSent, DataReceived);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		printf("\nDataBase Error\n");
	}
}



bool DataBaseAccess::GetProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive,vector<string> &entryDate,vector<string> &entryTime, const char * fillterValue , const char * startDate , const char * startTime , const char * endDate , const char * endTime){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		//SELECT * FROM ProcessMonitor where EntryDateTime <= '2016-01-20 17:16:46' and EntryDateTime >= '2015-01-20 17:16:46'ORDER BY RAND() LIMIT 10
		sprintf(query,"select `ProcessCpuUtilisation` ,`OverallCpuUtilisation` ,`ProcessVirtualMemoryUtilisation` ,`ProcessPhysicalMemoryUtilisation`,`DataSent` ,`DataReceived`,`EntryDate`,`EntryTime` from ProcessMonitor");
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if ( rowCount > 0 ){
			while ((row = mysql_fetch_row(res)) != NULL) {
				processCPU.push_back(row[0]);
				totalCPU.push_back(row[1]);
				virtualMemory.push_back(row[2]);
				physicalMemory.push_back(row[3]);
				dataSend.push_back(row[4]);
				dataReceive.push_back(row[5]);
				entryDate.push_back(row[6]);
				entryTime.push_back(row[7]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
}




bool DataBaseAccess::GetOffLinePerformanceMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive, const char * startDate , const char * startTime , const char * endDate , const char * endTime, int DID, int count ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,"select `ProcessCpuUtilisation` ,`OverallCpuUtilisation` ,`ProcessVirtualMemoryUtilisation` ,`ProcessPhysicalMemoryUtilisation`,`DataSent` ,`DataReceived` from ProcessMonitor where EntryDateTime >= '%s %s' and EntryDateTime <= '%s %s' ORDER BY RAND() LIMIT %d", startDate , startTime , endDate, endTime, count );
		if (  ISDEBUG )  cout<<"\nquery---->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if ( rowCount > 0 ){
			while ((row = mysql_fetch_row(res)) != NULL) {
				processCPU.push_back(row[0]);
				totalCPU.push_back(row[1]);
				virtualMemory.push_back(row[2]);
				physicalMemory.push_back(row[3]);
				dataSend.push_back(row[4]);
				dataReceive.push_back(row[5]);

			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
}



bool DataBaseAccess::GetMaxProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive, const char * startDate , const char * startTime , const char * endDate , const char * endTime){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,"select max(ProcessCpuUtilisation) ,max(OverallCpuUtilisation) ,max(ProcessVirtualMemoryUtilisation) ,max(ProcessPhysicalMemoryUtilisation),max(DataSent) ,max(DataReceived) from ProcessMonitor where EntryDateTime between '%s %s' and '%s %s';",  startDate, startTime,endDate, endTime);
		if (  ISDEBUG )  cout<<"\nMax Query--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if ( rowCount > 0 ){
			while ((row = mysql_fetch_row(res)) != NULL) {
				processCPU.push_back(row[0]);
				totalCPU.push_back(row[1]);
				virtualMemory.push_back(row[2]);
				physicalMemory.push_back(row[3]);
				dataSend.push_back(row[4]);
				dataReceive.push_back(row[5]);

			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
}


bool DataBaseAccess::GetLiveProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"select ProcessCpuUtilisation ,OverallCpuUtilisation ,ProcessVirtualMemoryUtilisation ,ProcessPhysicalMemoryUtilisation,DataSent ,DataReceived from ProcessMonitor ORDER BY `EntryID` DESC limit 1 ");
		if (  ISDEBUG )  cout<<"\nMax Query--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if ( rowCount > 0 ){
			while ((row = mysql_fetch_row(res)) != NULL) {
				processCPU.push_back(row[0]);
				totalCPU.push_back(row[1]);
				virtualMemory.push_back(row[2]);
				physicalMemory.push_back(row[3]);
				dataSend.push_back(row[4]);
				dataReceive.push_back(row[5]);
			}
			mysql_free_result(res);
			res = NULL;
			return true;
		}
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
	return false;
}



bool DataBaseAccess::GetMinProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive, const char * startDate , const char * startTime , const char * endDate , const char * endTime){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,"select min(ProcessCpuUtilisation) ,min(OverallCpuUtilisation) ,min(ProcessVirtualMemoryUtilisation) ,min(ProcessPhysicalMemoryUtilisation),min(DataSent) ,min(DataReceived) from ProcessMonitor  where EntryDateTime between '%s %s' and '%s %s';",  startDate, startTime,endDate, endTime);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if ( rowCount > 0 ){
			while ((row = mysql_fetch_row(res)) != NULL) {
				processCPU.push_back(row[0]);
				totalCPU.push_back(row[1]);
				virtualMemory.push_back(row[2]);
				physicalMemory.push_back(row[3]);
				dataSend.push_back(row[4]);
				dataReceive.push_back(row[5]);

			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
}





bool DataBaseAccess::GetAvgProcessMonitoring(vector<string> &processCPU,vector<string> &totalCPU,vector<string> &virtualMemory,vector<string> &physicalMemory,vector<string> &dataSend, vector<string> &dataReceive, const char * startDate , const char * startTime , const char * endDate , const char * endTime){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,"select avg(ProcessCpuUtilisation) ,avg(OverallCpuUtilisation) ,avg(ProcessVirtualMemoryUtilisation) ,avg(ProcessPhysicalMemoryUtilisation),avg(DataSent) ,avg(DataReceived) from ProcessMonitor where EntryDateTime between '%s %s' and '%s %s';",  startDate, startTime,endDate, endTime);
		if (  ISDEBUG )  cout<<"\nAvg Query--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		if ( res != NULL ){
			rowCount = mysql_num_rows(res);
			if ( rowCount > 0 ){
				while ((row = mysql_fetch_row(res)) != NULL) {

					processCPU.push_back(row[0]);
					totalCPU.push_back(row[1]);
					virtualMemory.push_back(row[2]);
					physicalMemory.push_back(row[3]);
					dataSend.push_back(row[4]);
					dataReceive.push_back(row[5]);
				}
				mysql_free_result(res);
				res = NULL;
				return true;
			}
		}
		return false;
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
	return false;
}


bool DataBaseAccess::GetBooleanValue( char * value ){

	if ( strcmp(value,"1") == 0 )
		return true;
	else
		return false;
}

bool DataBaseAccess::GetAllChannelDetailsForDID( int DID , vector<std::string > & cName, vector<bool> & dumpingStatus ,vector<bool> & activeStatus ,vector<bool> & resendStatus ,vector<bool> & retrievelStatus,vector<bool> & channelModeStatus  ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,"select  `CHANNELNAME`,`STORAGESTATUS`,`STATUS` ,`STREAMMODE`,`RETRIVERMODE` ,`REPLICAMODE`  from channelinfo where DID ='%d' limit 500;",DID);
		if (  ISDEBUG )  cout<<"\nAvg Query--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		if ( res != NULL ){
			rowCount = mysql_num_rows(res);
			if ( rowCount > 0 ){
				while ((row = mysql_fetch_row(res)) != NULL) {
					cName.push_back(row[0]);
					dumpingStatus.push_back(GetBooleanValue(row[1]));
					activeStatus.push_back(GetBooleanValue(row[2]));
					resendStatus.push_back(GetBooleanValue(row[3]));
					retrievelStatus.push_back(GetBooleanValue(row[4]));
					channelModeStatus.push_back(GetBooleanValue(row[5]));
				}
				mysql_free_result(res);
				res = NULL;
				return true;
			}
		}
		return false;
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
	return false;
}


int DataBaseAccess::CheckActiveUser(int UID,int DID){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		sprintf(query,"select AUID from activeUser where UID ='%d' and DID = '%d'",UID, DID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return atoi(row[0]);
			}
		}
		return -1;
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return -1;
}

int DataBaseAccess::InsertActiveUserInfo(int UID, int DID,const char *clientIP,int status){

	try {
		char query[SIZEOFCHARARRAYBIGGER];
		bool statusResult = false;
		sprintf(query, "insert into activeUser values ('','%d','%d','%s','%d',NOW(),NOW())", UID,DID,clientIP,status);
		if (  ISDEBUG )  cout<<"\nInsert activeUser---> "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		mysql_commit(mySqlConnection);
		return mysql_insert_id(mySqlConnection);;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return 0;
}

bool DataBaseAccess::UpdateActiveUserInfo(int AUID,const char *clientIP,int status){
	try {
		char query[SIZEOFCHARARRAYBIGGER];
		bool statusResult = false;
		sprintf(query, "update activeUser set `CLIENTMACHINEIP` = '%s', STATUS = '%d'  where AUID = '%d' ",clientIP,status,AUID );
		if (  ISDEBUG )  cout<<"\nupdate activeUser---> "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::GetActiveUserInfo( int DID, vector<string> &Uname, vector<int> &Urole, vector<string> &lastLoginTime ,vector<string> &ip  ){
	try {
		MYSQL_ROW row;
		MYSQL_RES *res = NULL;
		int rowCount = 0;
		char query[SIZEOFCHARARRAYBIGGER];
		bool statusResult = false;
		sprintf(query, "select T1.USERName,T1.USERROLE, T2.LASTMODIFIEDTIMESTAMP,T2.CLIENTMACHINEIP from userinfo T1 inner join (select * from activeUser where DID = '%d' and STATUS = '1' ) T2 where T1.UID = T2.UID",DID );
		if (  ISDEBUG )  cout<<"\n GetActiveUserInfo ---> "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion (query);
		}
		res = mysql_store_result(mySqlConnection);
		if ( res != NULL ){
			rowCount = mysql_num_rows(res);
			if ( rowCount > 0 ){
				while ((row = mysql_fetch_row(res)) != NULL) {
					Uname.push_back(row[0]);
					Urole.push_back(atoi(row[1]));
					lastLoginTime.push_back(row[2]);
					ip.push_back(row[3]);
				}
				mysql_free_result(res);
				res = NULL;
				return true;
			}
		}
		return false;
		mysql_commit(mySqlConnection);
	}
	catch (std::exception e) {
		PrintException("dataBaseAccess","GetVideoNameAndVID",e);
	}
	return false;
}

bool DataBaseAccess::UpdateActiveUserStatus(int AUID,int status){
	try {
		char query[SIZEOFCHARARRAYBIGGER];
		bool statusResult = false;
		sprintf(query, "update activeUser set STATUS = '%d' where AUID = '%d' ",status,AUID );
		if (  ISDEBUG )  cout<<"\nUpdate activeUser---> "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

bool DataBaseAccess::CheckUserLoginStatus( int AUID){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		sprintf(query,"select STATUS from activeUser where AUID = '%d'",AUID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return GetBooleanValue(row[0]);
			}
		}
		return false;
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}

bool DataBaseAccess::UpdateActiveUserStatusFalse(){
	try {
		char query[SIZEOFCHARARRAYBIGGER];
		bool statusResult = false;
		sprintf(query, "update activeUser set STATUS = '0'");
		if (  ISDEBUG )  cout<<"\n UpdateActiveUserStatusFalse---> "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

/************************************29 jan 2016 *****************************************/
void DataBaseAccess::GetAllChannelWithoutDetails(  vector < std::string >&cID , vector < std::string >&cName, const char * searchText){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;
	char temp[5];
			sprintf ( temp,"%s","%");
			cout<<"\ntemp--->"<<temp<<endl;
	try {
		if ( strcmp(searchText ,"") == 0 )
			sprintf(query,"select CID, CHANNELNAME from channelinfo");
		else
			sprintf(query,"select CID, CHANNELNAME from channelinfo where CHANNELNAME like '%s%s%s'",temp,searchText,temp);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				cID.push_back(row[0]);
				cName.push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
		//return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllChannelWithoutDetails", e);
	}
	//return false;
}
void DataBaseAccess::GetAllChannelGroupWithoutDetails(  vector < std::string >&CGID , vector < std::string >&CGName, const char * searchText){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;
	char temp[5];
			sprintf ( temp,"%s","%");
			cout<<"\ntemp--->"<<temp<<endl;
	try {
		if ( strcmp(searchText ,"") == 0 )
			sprintf(query,"select CGID, CGNAME from cgroupinfo");
		else
			sprintf(query,"select CGID, CGNAME from cgroupinfo where CGNAME like '%s%s%s'",temp,searchText,temp);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CGID.push_back(row[0]);
				CGName.push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
		//	return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllChannelGroupWithoutDetails", e);
	}
	//return false;
}
void DataBaseAccess::GetAllUserWithoutDetails(  vector < std::string >&UID , vector < std::string >&UName, const char * searchText){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;
	char temp[5];
			sprintf ( temp,"%s","%");
			cout<<"\ntemp--->"<<temp<<endl;
	try {
		if ( strcmp(searchText ,"") == 0 )
			sprintf(query,"select UID, NAME from userinfo");
		else
			sprintf(query,"select UID, NAME from userinfo where NAME like '%s%s%s'",temp,searchText,temp);
		if (  ISDEBUG )  cout<<"\nquery"<<query;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UID.push_back(row[0]);
				UName.push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
		//return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllUserWithoutDetails", e);
	}
	//return false;
}
void DataBaseAccess::GetAllUserGroupWithoutDetails(  vector < std::string >&UGID , vector < std::string >&UGName, const char * searchText){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;
	char temp[5];
			sprintf ( temp,"%s","%");
			cout<<"\ntemp--->"<<temp<<endl;
	try {
		if ( strcmp(searchText ,"") == 0 )
			sprintf(query,"select UGID, UGNAME from ugroupinfo");
		else
			sprintf(query,"select UGID, UGNAME from ugroupinfo where UGNAME like '%s%s%s'",temp,searchText,temp);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UGID.push_back(row[0]);
				UGName.push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
		//return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllUserGroupWithoutDetails", e);
	}
	//return false;
}
void DataBaseAccess::GetAllDeviceWithoutDetails(  vector < std::string >&DID , vector < std::string >&DName, const char * searchText ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;
	char temp[5];
		sprintf ( temp,"%s","%");
		cout<<"\ntemp--->"<<temp<<endl;
	try {
		if ( strcmp(searchText ,"") == 0 )
			sprintf(query,"select DID, DEVICENAME from deviceinfo");
		else
			sprintf(query,"select DID, DEVICENAME from deviceinfo where DEVICENAME like '%s%s%s'",temp,searchText,temp);
		statusResult = !(mysql_query(mySqlConnection, query));

		cout<<"\nGetAllDeviceWithoutDetails--->"<<query<<endl;
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				DID.push_back(row[0]);
				DName.push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
		//return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAlldeviceWithoutDetails", e);
	}
	//return false;
}


void DataBaseAccess::GetUGIDRelatedToUID( vector< int > &UGID , vector< int >UID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( UID.size() >  0 ){
			sprintf(query,"select UGID from userugrouprelation where (");
			for ( int i = 0 ; i < (int)UID.size() ; i++ ){
				if ( i == 0)
					sprintf(query,"%s UID = '%d'",query,UID[i]);
				else
					sprintf(query,"%s or UID = '%d'",query,UID[i]);
			}
			sprintf(query,"%s )",query);

			if ( UGID.size() > 0 ){
				for ( int i = 0 ; i < (int)UGID.size() ; i++ ){
					if ( i == 0 )
						sprintf(query,"%s and ( UGID != '%d'",query,UGID[i]);
					else
						sprintf(query,"%s and UGID != '%d'",query,UGID[i]);
				}
				sprintf(query,"%s )",query);
			}
			if (  ISDEBUG )  cout<<"\n GetUGIDRelatedToUID ---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					UGID.push_back(atoi(row[0]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAlldeviceWithoutDetails", e);
	}

}

void DataBaseAccess::GetUIDRelatedToUGID( vector< int > &UID , vector< int >UGID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( UGID.size() >  0 ){
			sprintf(query,"select UID from userugrouprelation where (");
			for ( int i = 0 ; i < (int)UGID.size() ; i++ ){
				if ( i == 0)
					sprintf(query,"%s UGID = '%d'",query,UGID[i]);
				else
					sprintf(query,"%s or UGID = '%d'",query,UGID[i]);
			}
			sprintf(query,"%s )",query);

			if ( UID.size() > 0 ){
				for ( int i = 0 ; i < (int)UID.size() ; i++ ){
					if ( i == 0 )
						sprintf(query,"%s and ( UID != '%d'",query,UID[i]);
					else
						sprintf(query,"%s and UID != '%d'",query,UID[i]);
				}
				sprintf(query,"%s )",query);
			}
			if (  ISDEBUG )  cout<<"\n GetUIDRelatedToUGID ---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					UID.push_back(atoi(row[0]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAlldeviceWithoutDetails", e);
	}
}

void DataBaseAccess::GetCGIDRelatedToUGID( vector< int > &CGID , vector< int >UGID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( UGID.size() >  0 ){
			sprintf(query,"select CGID from ugroupcgrouprelation where (");
			for ( int i = 0 ; i < (int)UGID.size() ; i++ ){
				if ( i == 0)
					sprintf(query,"%s UGID = '%d'",query,UGID[i]);
				else
					sprintf(query,"%s or UGID = '%d'",query,UGID[i]);
			}
			sprintf(query,"%s )",query);

			if ( CGID.size() > 0 ){
				for ( int i = 0 ; i < (int)CGID.size() ; i++ ){
					if ( i == 0 )
						sprintf(query,"%s and ( CGID != '%d'",query,CGID[i]);
					else
						sprintf(query,"%s and CGID != '%d'",query,CGID[i]);
				}
				sprintf(query,"%s )",query);
			}
			if (  ISDEBUG )  cout<<"\n GetCGIDRelatedToUGID ---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					CGID.push_back(atoi(row[0]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAlldeviceWithoutDetails", e);
	}
}

void DataBaseAccess::GetUGIDRelatedToCGID( vector< int > &UGID , vector< int >CGID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( CGID.size() >  0 ){
			sprintf(query,"select UGID from ugroupcgrouprelation where (");
			for ( int i = 0 ; i < (int)CGID.size() ; i++ ){
				if ( i == 0)
					sprintf(query,"%s CGID = '%d'",query,CGID[i]);
				else
					sprintf(query,"%s or CGID = '%d'",query,CGID[i]);
			}
			sprintf(query,"%s )",query);

			if ( UGID.size() > 0 ){
				for ( int i = 0 ; i < UGID.size() ; i++ ){
					if ( i == 0 )
						sprintf(query,"%s and ( UGID != '%d'",query,UGID[i]);
					else
						sprintf(query,"%s and UGID != '%d'",query,UGID[i]);
				}
				sprintf(query,"%s )",query);
			}
			if (  ISDEBUG )  cout<<"\n GetUGIDRelatedToCGID---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					UGID.push_back(atoi(row[0]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAlldeviceWithoutDetails", e);
	}
}
void DataBaseAccess::GetCIDRelatedToCGID( vector< int > &CID , vector< int >CGID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( CGID.size() >  0 ){
			sprintf(query,"select CID from channelcgrouprelation where (");
			for ( int i = 0 ; i < (int)CGID.size() ; i++ ){
				if ( i == 0)
					sprintf(query,"%s CGID = '%d'",query,CGID[i]);
				else
					sprintf(query,"%s or CGID = '%d'",query,CGID[i]);
			}
			sprintf(query,"%s )",query);

			if ( CID.size() > 0 ){
				for ( int i = 0 ; i < (int)CID.size() ; i++ ){
					if ( i == 0 )
						sprintf(query,"%s and ( CID != '%d'",query,CID[i]);
					else
						sprintf(query,"%s and CID != '%d'",query,CID[i]);
				}
				sprintf(query,"%s )",query);
			}
			if (  ISDEBUG )  cout<<"\n GetCIDRelatedToCGID---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					CID.push_back(atoi(row[0]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAlldeviceWithoutDetails", e);
	}
}

void DataBaseAccess::GetCGIDRelatedToCID( vector< int > &CGID , vector< int > CID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( CID.size() >  0 ){
			sprintf(query,"select CGID from channelcgrouprelation where (");
			for ( int i = 0 ; i < (int)CID.size() ; i++ ){
				if ( i == 0)
					sprintf(query,"%s CID = '%d'",query,CID[i]);
				else
					sprintf(query,"%s or CID = '%d'",query,CID[i]);
			}

			sprintf(query,"%s )",query);

			if ( CGID.size() > 0 ){
				for ( int i = 0 ; i < (int)CGID.size() ; i++ ){
					if ( i == 0 )
						sprintf(query,"%s and ( CGID != '%d'",query,CGID[i]);
					else
						sprintf(query,"%s and CGID != '%d'",query,CGID[i]);
				}
				sprintf(query,"%s )",query);
			}
			if (  ISDEBUG )  cout<<"\n GetCGIDRelatedToCID---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					CGID.push_back(atoi(row[0]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetCGIDRelatedToCID", e);
	}
}

void DataBaseAccess::GetDIDRelatedToCID( vector< int > &DID , vector< int >CID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( CID.size() >  0 ){
			sprintf(query,"select distinct DID from channelinfo where ( ");
			for ( int i = 0 ; i < (int)CID.size() ; i++ ){
				if ( i == 0)
					sprintf(query,"%s CID = '%d'",query,CID[i]);
				else
					sprintf(query,"%s or CID = '%d'",query,CID[i]);
			}
			sprintf(query,"%s )",query);

			if ( DID.size() > 0 ){
				for ( int i = 0 ; i < (int)DID.size() ; i++ ){
					if ( i == 0 )
						sprintf(query,"%s and ( DID != '%d'",query,DID[i]);
					else
						sprintf(query,"%s and DID != '%d'",query,DID[i]);
				}
				sprintf(query,"%s )",query);
			}
			if (  ISDEBUG )  cout<<"\nGetDIDRelatedToCID---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					DID.push_back(atoi(row[0]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetCGIDRelatedToCID", e);
	}
}

void DataBaseAccess::GetCIDRelatedToDID( vector< int > &CID , vector< int > DID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( DID.size() >  0 ){
			sprintf(query,"select CID from channelinfo where (");
			for ( int i = 0 ; i < (int)DID.size() ; i++ ){
				if ( i == 0 )
					sprintf(query,"%s DID = '%d'",query,DID[i]);
				else
					sprintf(query,"%s or DID = '%d'",query,DID[i]);
			}

			sprintf(query,"%s )",query);

			if ( CID.size() > 0 ){
				for ( int i = 0 ; i < (int)CID.size() ; i++ ){
					if ( i == 0 )
						sprintf(query,"%s and ( CID != '%d'",query,CID[i]);
					else
						sprintf(query,"%s and CID != '%d'",query,CID[i]);
				}
				sprintf(query,"%s )",query);
			}
			if (  ISDEBUG )  cout<<"\nGetCIDRelatedToDID---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					CID.push_back(atoi(row[0]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetCGIDRelatedToCID", e);
	}
}

void DataBaseAccess::GetNameFromTable(vector< int > &UID,vector< string > &UName ,const char * tableName,const char * getColumnName, const char * conditionColumnName ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( UID.size() >  0 ){
			sprintf(query,"select %s,%s from %s where ", getColumnName,conditionColumnName, tableName);
			for ( int i = 0 ; i < (int)UID.size() ; i++ ){
				if ( i == 0)
					sprintf(query,"%s %s = '%d'",query,conditionColumnName,UID[i]);
				else
					sprintf(query,"%s or %s = '%d'",query,conditionColumnName,UID[i]);
			}
			if (  ISDEBUG )  cout<<"\n GetNameFromTable ---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			UID.clear();
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {

					UName.push_back(row[0]);
					UID.push_back(atoi(row[1]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetCGIDRelatedToCID", e);
	}
}

void DataBaseAccess::GetName(vector< int >&UID,vector< int >&UGID,vector< int >&CGID,vector< int >&CID,vector< int >&DID,vector< string >& UName,vector< string >&UGName,vector< string >&CGName,vector< string >&CName,vector< string >&DName){

	GetNameFromTable(UID,UName ,"userinfo", "NAME", "UID" );
	GetNameFromTable(UGID,UGName ,"ugroupinfo", "UGNAME", "UGID" );
	GetNameFromTable(CGID,CGName ,"cgroupinfo", "CGNAME", "CGID" );
	GetNameFromTable(CID,CName ,"channelinfo", "CHANNELNAME", "CID" );
	GetNameFromTable(DID,DName ,"deviceinfo", "DEVICENAME", "DID" );
}

void DataBaseAccess::GetRelation( vector < int > relationTo,vector < int > relationFrom, vector < int > &IDLinkFrom, vector < int > &IDLinkTo, const char *tagLinkFrom, const char *tagLinkTo, const char *tableName ){

	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {
		if ( relationTo.size() >  0 ){
			sprintf(query,"select %s, %s from %s where (", tagLinkFrom, tagLinkTo, tableName);
			for ( int i = 0 ; i < (int)relationTo.size() ; i++ ){
				if ( i == 0)
					sprintf(query,"%s %s = '%d'",query,tagLinkFrom,relationTo[i]);
				else
					sprintf(query,"%s or %s = '%d'",query,tagLinkFrom,relationTo[i]);
			}
			sprintf( query, "%s )",query);
			if ( (int)relationFrom.size() > 0 ){
				for ( int i = 0 ; i < (int)relationFrom.size() ; i++ ){
					if ( i == 0)
						sprintf(query,"%s and ( %s = '%d'",query,tagLinkTo,relationFrom[i]);
					else
						sprintf(query,"%s or %s = '%d'",query,tagLinkTo,relationFrom[i]);
				}
				sprintf( query, "%s )",query);
			}
			if (  ISDEBUG )  cout<<"\n GetRelation---->"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}
			res = mysql_store_result(mySqlConnection);
			if (res) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					IDLinkFrom.push_back(atoi(row[0]));
					IDLinkTo.push_back(atoi(row[1]));
				}
			}
			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetCGIDRelatedToCID", e);
	}
}

void DataBaseAccess::GetChannelInfo(char * channelName , int &CID , int &DID){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {

		sprintf(query,"select CID, DID from channelinfo where CHANNELNAME = '%s'", channelName);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID = atoi(row[0]);
				DID = atoi(row[1]);
				break;
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);

	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetCGIDRelatedToCID", e);
	}
}

/********************************************************************************************/


void DataBaseAccess::GetAllCIDANDDID( vector<int> &tempCID,int tempDID , vector<string> &outputPath){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {

		sprintf(query,"select CID, STORAGEPATH from channelinfo where DID = '%d'", tempDID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				tempCID.push_back(atoi(row[0]));
				outputPath.push_back((row[1]));
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);

	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetCGIDRelatedToCID", e);
	}
}

void DataBaseAccess::GetChannelUsedSpace(int tempCID,int sysDID,double &channelUsedSize){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = 0;

	try {

		sprintf(query,"select sum(SIZE) from videoinfo where DID = '%d' and CID = '%d'", sysDID, tempCID);
		if (  ISDEBUG )  cout<<"\nquery--->"<<query;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				channelUsedSize = atof(row[0]);
			}

			mysql_commit(mySqlConnection);
			mysql_free_result(res);
		}
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetChannelUsedSpace", e);
	}
}

bool DataBaseAccess::CheckChannelExists(const char * CID, int sysDID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT * FROM channelinfo where CID = '%s' and DID = '%d' ",CID ,sysDID);
		if (  ISDEBUG )  cout<<"\n CheckChannelExists" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return true;
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
		return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "CheckChannelExists", e);
	}
	return 0;
}

int DataBaseAccess::GetRowCountInVideoInfo(int CID , int DID) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT COUNT(*) FROM videoinfo where CID = '%d' and DID = '%d'",CID,DID);
		if (  ISDEBUG )  cout<<"\nGetRowCountInVideoInfo" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return atoi(row[0]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
		return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetRowCountInVideoInfo", e);
	}
	return false;
}

void DataBaseAccess::GetDIDFromCID( int &mainDID,char * CID){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT DID FROM channelinfo where CID = '%d' ",CID);
		if (  ISDEBUG )  cout<<"\n GetDIDFromCID" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				mainDID =  atoi(row[0]);
				return;
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
		//return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetDIDFromCID", e);
	}
	//return false;
}
void DataBaseAccess::GetOutputFolder(char* outputPath,int CID, int sysDID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT STORAGEPATH FROM channelinfo where CID = '%d' and DID = '%d' ",CID,sysDID);
		if (  ISDEBUG )  cout<<"\n GetDIDFromCID" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				sprintf(outputPath,"%s", row[0]);
				return;
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
		//return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetOutputFolder", e);
	}
}

void DataBaseAccess::GetAllVideosForParticularCID( int tempCID, vector<string> &vName ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT VNAME FROM videoinfo where CID = '%d' ",tempCID);
		if (  ISDEBUG )  cout<<"\n GetDIDFromCID" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				vName.push_back(row[0]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
		//return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetOutputFolder", e);
	}
}

bool DataBaseAccess::FileExist(const char *fileName ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT * FROM videoinfo where VNAME = '%s' ",fileName);
		if (  ISDEBUG )  cout<<"\n--FileExist---->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return true;
			}
			mysql_free_result(res);
			res = NULL;
		}
		return false;
		mysql_commit(mySqlConnection);
		//return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetOutputFolder", e);
	}
	return false;
}

bool DataBaseAccess::GetChannelForCGID( std::string CGID,vector<std::string> &CNAME){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query," select T2.CHANNELNAME from ( SELECT CID FROM channelcgrouprelation where CGID = '%s') T1 inner join channelinfo T2 where T1.CID = T2.CID",CGID.c_str());
		cout<<"\n--GetChannelForCGID---->"<<query<<endl;
		if (  ISDEBUG )  cout<<"\n--GetChannelForCGID---->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CNAME.push_back(row[0]);
			}
			mysql_free_result(res);
			res = NULL;
			return true;
		}
		return false;
		mysql_commit(mySqlConnection);
		//return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetChannelForCGID", e);
	}
	return false;
}

bool DataBaseAccess::GetAllChannelGroupForUID( int UID, vector <std::string> &CGID,vector <std::string> &CGNAME){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"select T2.CGID,T2.CGNAME from ( SELECT * FROM userugrouprelation where UID = '%d' ) T1 inner join (select sT1.UGID,sT2.CGID,sT2.CGNAME from ugroupcgrouprelation sT1 inner join cgroupinfo sT2 where sT1.CGID = sT2.CGID  ) T2 where T1.UGID = T2.UGID",UID);
		cout<<"\nGetAllChannelGroupForUID--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CGID.push_back(row[0]);
				CGNAME.push_back(row[1]);
			}

			mysql_free_result(res);
			res = NULL;
			return true;
		}
		return false;
		mysql_commit(mySqlConnection);
		//return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllChannelGroupForUID", e);
	}
	return false;
}


bool DataBaseAccess::GetChannelID( char *CGName,char * CName, int &CID,int &DID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"select T1.CID,T2.DID from ( SELECT CID FROM channelcgrouprelation where CGID = (select CGID from cgroupinfo where CGNAME = '%s') ) T1 inner join ( select CID,DID from channelinfo where CHANNELNAME = '%s' ) T2 where T1.CID = T2.CID",CGName,CName);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID = atoi(row[0]);
				DID = atoi(row[1]);
				return true;
			}

			mysql_free_result(res);
			res = NULL;
			return true;
		}
		return false;
		mysql_commit(mySqlConnection);
		//return 0;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllChannelGroupForUID", e);
	}
	return false;
}

void DataBaseAccess::GetUserID( const char * UserName, int &UID){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"select UID from userinfo where UserName like '%s'",UserName);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UID = atoi(row[0]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllChannelGroupForUID", e);
	}
}

bool DataBaseAccess::InsertImageInfo(char *imageName,char * imagePath,int imageRelatedCID,int imageRelatedDID,int imageRelatedUID){
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"insert into `imageinfo` (`IID`,`INAME`,`CID`,`DID`,`UID`,`DirectoryPath`,`CREATINGTIME`,`CREATETIMESTAMP`,`LASTMODIFIEDTIMESTAMP`) Values ('','%s','%d','%d',%d,%s,now(),now(),now())",imageName, imageRelatedCID, imageRelatedDID, imageRelatedUID, imagePath );
		if (  ISDEBUG )  cout<<"\nInsertImageInfo" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);

		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

void DataBaseAccess::GetCreatedRtsp( const char *CID, char *createdRTSP){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"select RTSP from rtspinfo where CID = '%s' and DID = (select DID from channelinfo where CID = '%s')",CID,CID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				sprintf ( createdRTSP ,"%s", row[0]);
				cout<<"\ncreatedRtsp\n"<<createdRTSP<<endl;
				break;
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllChannelGroupForUID", e);
	}
}

bool DataBaseAccess::InsertCreatedRtsp(int CID, int DID , char * createdRtsp){
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"insert into `rtspinfo` (`ID`,`CID`,`DID`,`RTSP`) Values ('','%d','%d','%s')",CID,DID,createdRtsp );
		if ( ! ISDEBUG )  cout<<"\nInsertCreatedRtsp" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return 0;
}

bool DataBaseAccess::ClearRtspInfo( int CID ){
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"delete from `rtspinfo` where CID ='%d'",CID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return false;

}

bool DataBaseAccess::UpdateRtspInfo(char * CID, char * DID , char * createdRtsp){
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"update `rtspinfo` set `RTSP`= '%s' where CID = '%s' and DID ='%s')",createdRtsp,CID,DID );
		if ( ! ISDEBUG )  cout<<"\nInsertCreatedRtsp" << query << endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return statusResult;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return false;
}

//dBA->UpdateDeviceBackupStatus();
void DataBaseAccess::UpdateDeviceBackupStatus(int sysDID , int status ){
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"update deviceinfo set BACKUP = '%d' where DID = '%d'", status , sysDID );
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "UpdateChannelInfoStatus", e);
	}
}


void DataBaseAccess::UpdateChannelInfoStatus(int sysDID){
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"update channelinfo set STATUS = '0' where DID = '%d'", sysDID);
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "UpdateChannelInfoStatus", e);
	}
}

void DataBaseAccess::ClearRtspTable(){
	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"delete from `rtspinfo`");
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}

}

void DataBaseAccess::GetAllRtspInfo( vector<string> &CID,vector<string> &DID,vector<string> & rtsp ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"select CID,DID,RTSP from rtspinfo");
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID.push_back(row[0]);
				DID.push_back(row[1]);
				rtsp.push_back(row[2]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllChannelGroupForUID", e);
	}
}

int DataBaseAccess::IsChannelsExist( int DID ){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT COUNT(*) FROM channelinfo where DID = '%d' and STATUS = '1'",DID);
		cout<<"\nIsChannelsExist--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				return atoi(row[0]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetAllChannelGroupForUID", e);
	}
	return 0;
}

bool DataBaseAccess::ReplaceDID(const char * tableName, const char * oldDID, int newDID){

	char query[3000];
	bool statusResult;
	try {
		sprintf(query,"update `%s` set DID = '%d' where DID = '%s'", tableName, newDID, oldDID );
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		mysql_commit(mySqlConnection);
		return true;
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "InsertUserInfo", e);
	}
	return false;
}


bool DataBaseAccess::GetAllChannelForDID(const char *DIDInfo,vector<int> *CID,vector<string> *ChannelName, vector<string> *RTSP, vector<int> *DID,vector<int> *status, vector<int> *storagestatus,vector<string> *RetrieverMode, vector<string> *ChannelMode,vector<string> *StoragePath, vector<string> *StorageSpace,vector<string> *StorageSpaceUsed, vector<string> *StorageFileType,vector<string> *ReStream, vector<string>*deviceIP , vector<string>*cameraIP , vector<string>*userName , vector<string>*portNumber , vector<string>*password, vector<string>*ptzControl, vector<string>*motion, vector<string>*description, vector<string>*make, vector<string>*model) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,"select T1.CID,T1.ChannelName,T1.RTSP,T1.DID,T1.status,T1.storagestatus,T1.retrivermode,T1.REPLICAMODE,T1.storagepath,T1.storagespace,T1.storagespaceused,T1.storagefiletype,T1.STREAMMODE,T2.DEVICEIP,T1.IPADDRESS,T1.USERNAME,T1.PORTNUMBER,T1.PASSWORD,T1.PTZCONTROL,T1.MOTION,T1.DESCRIPTION,T1.MAKE,T1.MODEL from channelinfo T1 inner join deviceinfo T2 where T1.DID = T2.DID and T1.DID = '%s'",DIDInfo);
		cout<<"\nGetAllChannel--->"<<query;
		if (  ISDEBUG )  cout<<"\nGetAllChannel"<<query;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(atoi(row[0]));
				ChannelName->push_back(row[1]);
				RTSP->push_back(row[2]);
				DID->push_back(atoi(row[3]));
				status->push_back(atoi(row[4]));
				storagestatus->push_back(atoi(row[5]));
				RetrieverMode->push_back(row[6]);
				ChannelMode->push_back(row[7]);
				StoragePath->push_back(row[8]);
				StorageSpace->push_back(row[9]);
				StorageSpaceUsed->push_back(row[10]);
				StorageFileType->push_back(row[11]);
				ReStream->push_back(row[12]);
				deviceIP->push_back(row[13]);
				cameraIP->push_back(row[14]);
				userName->push_back(row[15]);
				portNumber->push_back(row[16]);
				password->push_back(row[17]);
				ptzControl->push_back(row[18]);
				motion->push_back(row[19]);
				description->push_back(row[20]);
				make->push_back(row[21]);
				model->push_back(row[22]);

			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
	return false;
}


void DataBaseAccess::GetAllUserForDID(const char *DIDInfo,vector<string> *UID, vector<string> *Name,
		vector<string> *UName, vector<string> *Password,
		vector<string> *UserRole, vector<string> *Email, vector<string> *Mobile,
		vector<string> *Status) {

	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query,
				"select userinfo.UID,userinfo.Name,userinfo.username,userinfo.password,userinfo.userrole,userinfo.email,userinfo.ContactNo,userinfo.status from userinfo where DID = '%s'",DIDInfo);
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UID->push_back(row[0]);
				Name->push_back(row[1]);
				UName->push_back(row[2]);
				Password->push_back(row[3]);
				UserRole->push_back(row[4]);
				Email->push_back(row[5]);
				Mobile->push_back(row[6]);
				Status->push_back(row[7]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
}		//GetAllUGroupOnlyUser


void DataBaseAccess::GetAllCGroupForDID(const char * DIDInfo, vector<string> *CGID,
		vector<string> *CGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select cgid,cgname from cgroupinfo where DID = '%s'",DIDInfo);
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CGID->push_back(row[0]);
				CGName->push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
}

void DataBaseAccess::GetAllUGroupForDID(const char * DIDInfo,vector<string> *UGID,vector<string> *UGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {
		sprintf(query, "select Ugid,ugname from ugroupinfo where DID = '%s'",DIDInfo);
		if (  ISDEBUG )  cout<<"query = "<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}
		res = mysql_store_result(mySqlConnection);
		if (res) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UGID->push_back(row[0]);
				UGName->push_back(row[1]);
			}
		}
		mysql_commit(mySqlConnection);
		mysql_free_result(res);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetMethodName", e);
	}
}

void DataBaseAccess::GetAllChannelCGroupRelationForDID(const char * DIDInfo, vector<string> *CID,
		vector<string> *CGID, vector<string> *CGName, vector<string> *UID ) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,"select channelcgrouprelation.CID,channelcgrouprelation.CGID,channelcgrouprelation.UID,(select cgroupinfo.CGName from cgroupinfo where cgroupinfo.cgid=channelcgrouprelation.cgid) as CGName from channelcgrouprelation where DID = '%s'",DIDInfo);
		cout<<"\nGetAllChannelCGroupRelationForDID--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				CID->push_back(row[0]);
				CGID->push_back(row[1]);
				UID->push_back(row[2]);
				CGName->push_back(row[3]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

void DataBaseAccess::GetAllUserUGroupRelationForDID(const char * DID,vector<string> *UID,
	vector<string> *UGID, vector<string> *UGName) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;

	try {

		sprintf(query,"select userugrouprelation.UID,userugrouprelation.UGID,(select ugroupinfo.UGName from ugroupinfo where ugroupinfo.ugid=userugrouprelation.ugid) as UGName  from userugrouprelation where DID = '%s'",DID);
		cout<<"\nGetAllUserUGroupRelationForDID--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UID->push_back(row[0]);
				if (  ISDEBUG )  cout<<"\nRow[0]---->" << row[0];
				if (  ISDEBUG )  cout<<"\nRow[1]---->" << row[1];
				if (  ISDEBUG )  cout<<"\nRow[2]---->" << row[2];
				UGID->push_back(row[1]);
				UGName->push_back(row[2]);
			}
			mysql_free_result(res);
			res = NULL;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
}

bool DataBaseAccess::GetUGroupCGroupRelationForDID(const char * DID,vector<string> &UGID,vector<string> &CGID, vector<string> &CGNAME, vector<string> &UGNAME) {
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT T1.UGID,T1.CGID,T2.CGName,T3.UGName FROM ugroupcgrouprelation T1 inner join cgroupinfo T2 inner join ugroupinfo T3 where T1.CGID = T2.CGID and T1.UGID = T3.UGID and T1.DID = '%s' ;",DID);
		cout<<"\nGetUGroupCGroupRelationForDID--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UGID.push_back(row[0]);
				CGID.push_back(row[1]);
				CGNAME.push_back(row[2]);
				UGNAME.push_back(row[3]);
			}
			mysql_free_result(res);
			res = NULL;
			return true;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	return false;
}

void DataBaseAccess::GetActiveUID(vector<string> &UID, int DID){
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int rowCount = 0;
	char query[SIZEOFCHARARRAYBIGGER];
	bool statusResult = false;
	try {
		sprintf(query,"SELECT UID FROM userinfo where DID = '%d' ;",DID);
		cout<<"\nGetActiveUID--->"<<query<<endl;
		statusResult = !(mysql_query(mySqlConnection, query));
		if (!statusResult) {
			statusResult = StartQueryRecursion(query);
		}

		res = mysql_store_result(mySqlConnection);
		rowCount = mysql_num_rows(res);
		if (rowCount > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				UID.push_back(row[0]);
			}
			mysql_free_result(res);
			res = NULL;
			//return true;
		}
		mysql_commit(mySqlConnection);
	} catch (std::exception e) {
		PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
	}
	//return false;
}

void DataBaseAccess::DeleteTableEntryForDID( const char * tableName ,const char *DIDInfo ){

			char query[3000];
				bool statusResult;
				int id = 0;
				try {
					sprintf(query, "delete from `%s` where DID = '%s'",tableName , DIDInfo);
					cout<<"\nDeleteTableEntryForDID" << query << endl;
					if (  ISDEBUG )  cout<<"DeleteUserUGroupRelation" << query << endl;
					statusResult = !(mysql_query(mySqlConnection, query));
					if (!statusResult) {
						statusResult = StartQueryRecursion(query);

					}
					mysql_commit(mySqlConnection);
					//return statusResult;
				} catch (std::exception e) {
					PrintException("dataBaseAccess", "InsertUserInfo", e);
				}
				//return 0;

}


void DataBaseAccess::DeleteTableEntry( const char * tableName ,const char * columnName, int columnValue ){

			char query[3000];
				bool statusResult;
				int id = 0;
				try {
					sprintf(query, "delete from `%s` where %s = '%d'",tableName ,columnName, columnValue);
					cout<<"\nDeleteTableEntryForDID" << query << endl;
					if (  ISDEBUG )  cout<<"DeleteUserUGroupRelation" << query << endl;
					statusResult = !(mysql_query(mySqlConnection, query));
					if (!statusResult) {
						statusResult = StartQueryRecursion(query);

					}
					mysql_commit(mySqlConnection);
					//return statusResult;
				} catch (std::exception e) {
					PrintException("dataBaseAccess", "InsertUserInfo", e);
				}
				//return 0;

}

bool DataBaseAccess::CheckIDExistInTable(const char * tableName , const char * columnName, int value ){
	MYSQL_ROW row;
		MYSQL_RES *res = NULL;
		int rowCount = 0;
		char query[SIZEOFCHARARRAYBIGGER];
		bool statusResult = false;
		try {
			sprintf(query,"SELECT * FROM %s where `%s` = '%d'",tableName,columnName,value);
			cout<<"\nCheckIDExistInTable"<<query<<endl;
			statusResult = !(mysql_query(mySqlConnection, query));
			if (!statusResult) {
				statusResult = StartQueryRecursion(query);
			}

			res = mysql_store_result(mySqlConnection);
			rowCount = mysql_num_rows(res);
			if (rowCount > 0) {
				while ((row = mysql_fetch_row(res)) != NULL) {
					return true;
				}
				mysql_free_result(res);
				res = NULL;
				//return true;
			}
			mysql_commit(mySqlConnection);
		} catch (std::exception e) {
			PrintException("dataBaseAccess", "GetVideoNameAndVID", e);
		}
		return false;
}

