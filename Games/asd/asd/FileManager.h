#ifndef _FileManager_H_
#define _FileManager_H_

#include <iostream>
#include <cstdio>
#include "dirent.h"
#include <string.h>
#include <Windows.h>
#include "Jzon.h"
#if defined _MSC_VER
#include <direct.h>
#include <map>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif

union retType
{
    int intVal;
	float floatVal;
	bool boolVal;
};

struct retJSON{
	std::string type;
	std::string stringVal;
	retType retVal;
	std::map<std::string, retJSON> mapVal;
};

class FileManager{
public:
	std::map<int, retJSON> initializeObjects(const char* source);
	struct retJSON readJSON(const char* filename);
	struct retJSON parseJSON(Jzon::Node &node);
	struct retJSON returnVALUE(retJSON ret, char* index);
	static FileManager* Instance(){
		if(s_pInstance==0){
			s_pInstance=new FileManager();
		}
		return s_pInstance;
	}
private:
	FileManager(){};
	static FileManager* s_pInstance;
};
typedef FileManager TheFileManager;
#endif // _FileManager_H_