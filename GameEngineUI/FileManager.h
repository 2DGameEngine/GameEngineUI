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
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif

class FileManager{
public:
	bool copyFile(const char* source,const char* dest);
	bool copyFolder(const char* source,const char* dest);
	void FileManager::createDir(const char* dir);
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