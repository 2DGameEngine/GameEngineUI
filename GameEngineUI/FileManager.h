#ifndef _FileManager_H_
#define _FileManager_H_

#include <iostream>
#include <cstdio>
#include "dirent.h"
#include <string.h>
#include <Windows.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include "Jzon.h"
#if defined _MSC_VER
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif

using namespace Awesomium;

class FileManager{
public:
	bool copyFile(const char* source,const char* dest);
	bool renameGame(const char* source, const char* oldName, const char* prName);
	bool copyFolder(const char* source,const char* dest);
	bool checkGame(const char* source,const char* gameName);
	bool writeJSON(const char* filename,const char* jsonStr);
	JSValue readJSON(const char* filename);
	JSValue parseJSON(Jzon::Node &node);
	void FileManager::createDir(const char* dir);
	void FileManager::removeDir(const char* dir);
	bool FileManager::removeFile(const char* file);
	bool FileManager::renameDirorFile(const char* oldDir, const char* newDir);
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