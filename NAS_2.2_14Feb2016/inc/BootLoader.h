#ifndef _BootLoader_H_
#define _BootLoader_H_

#include "Retrieve.h"
#include "DataBaseAccess.h"
#include "CommonClass.h"
#include <dirent.h>

using namespace std;

class BootLoader
{
private:
	DataBaseAccess* DataBaseAccessObj;
	Retrieve * RetrieveObj ;
	CommonClass* CommonClassObj;


public:
	BootLoader(Retrieve* Obj);
	~BootLoader();

	bool CheckDirectoryExistance(char * directoryPath);
	void CreateDirectory(char *directoryPath);

	void StartBoot();
	void checkConsistency();
	void GetAllFileForDirectory(char * directoryPath, vector<string> &fileName);
	void CheckVideoExistance(char *directory, int tempCID,int sysDID);
};

#endif
