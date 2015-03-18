#include "FileManager.h"
#include "Jzon.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include<iostream>
#include<stdio.h>

using namespace Awesomium;

bool FileManager::copyFile(const char* source,const char* dest){
	FILE* source_pointer = fopen(source, "rb");
	FILE* dest_pointer = fopen(dest, "wb");
	if(source_pointer==NULL || dest_pointer==NULL)
		return false;
	size_t size;
	char buf[BUFSIZ];
	while (size = fread(buf, 1, BUFSIZ, source_pointer)) {
        fwrite(buf, 1, size, dest_pointer);
    }
	fclose(source_pointer);
    fclose(dest_pointer);
	return true;
}

void FileManager::createDir(const char* dir) {
	#if defined _MSC_VER
		_mkdir(dir);
	#elif defined __GNUC__
		mkdir(dir, 0777);
	#endif
}

void FileManager::removeDir(const char* dir) {
	#if defined _MSC_VER
		_rmdir(dir);
	#elif defined __GNUC__
		rmdir(dir);
	#endif
}


bool FileManager::removeFile(const char* file) {
	int result = remove(file);
	if(result == 0)
		return true;
	return false;
}

bool FileManager::renameDirorFile(const char* oldDir, const char* newDir) {
	int result = rename( oldDir, newDir );
	if( result != 0 )
		return false;
	return true;
}

bool FileManager::renameGame(const char* source, const char* oldName, const char* prName){
	DIR *dir, *tmp;
	bool ret = true;
	struct dirent *ent;
	std::string sfldr, str, tmpStr;
	std::string sfull_path;
	if ((dir = opendir (source)) != NULL) {
		 /* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if(strcmp(ent->d_name,".") && strcmp(ent->d_name,"..")){
				sfldr = source;
				sfldr += "\\";
				str = ent->d_name;
				std::size_t found = str.find(oldName);
				if (found != std::string::npos){
					std::string file, nFile;
					tmpStr = oldName;
					tmpStr += ".sln";

					if(str == tmpStr){
						file = sfldr + ent->d_name;
						std::ifstream in(file);
						file = sfldr + prName + ".sln";
						std::ofstream out(file);
						std::string wordToReplace(oldName), wordToReplaceWith(prName);
						if (!in || !out){
							return false;
						}
						std::string line;
						size_t len = wordToReplace.length();
						while (std::getline(in, line)){
							while (true){
								size_t pos = line.find(wordToReplace);
								if (pos != std::string::npos)
									line.replace(pos, len, wordToReplaceWith);
								else 
									break;
							}
							out<<line<<'\n';
						}
						file = sfldr + ent->d_name;
						in.close();
						out.close();
						std::cout<<"Remove = "<<file<<"\n";
						if(!removeFile(file.c_str())){
							return false;
						}
						file = sfldr + prName + ".sln";
						sfull_path = file;
						std::cout<<"Rsfull = "<<sfull_path<<"\n";
					}
					else{
						size_t len = strlen(oldName);
						file = sfldr + ent->d_name;
						str = ent->d_name;
						str.replace(found, len, prName);
						nFile = sfldr + str;
						if(!renameDirorFile(file.c_str(), nFile.c_str())){
							return false;						
						}
						sfull_path = nFile;
					}
				}
				else{
					sfull_path = sfldr.append(ent->d_name);
				}
				tmp = opendir (sfull_path.c_str());
				if( tmp != NULL){
					closedir(tmp);
					ret = renameGame(sfull_path.c_str(), oldName, prName);
				}
				if(ret == false){
					std::cout<<"Renameret = false\n";
					return false;
				}
			}
		}
		closedir (dir);
		return true;
	} 
	else {
		return false;
	}
}

bool FileManager::copyFolder(const char* source, const char* dest){
	DIR *dir, *tmp;
	bool ret = true;
	struct dirent *ent;
	std::string sfldr;
	std::string dfldr, tmpStr;
	const char* sfull_path;
	const char *dfull_path;
	createDir(dest);
	std::cout<<dest<<"\n";
	if ((dir = opendir (source)) != NULL) {
		 /* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if(strcmp(ent->d_name,".") && strcmp(ent->d_name,"..")){
				sfldr.assign(source);
				dfldr.assign(dest);
				sfldr += "\\";
				dfldr += "\\";
				sfull_path = (sfldr.append(ent->d_name)).c_str();
				dfull_path = (dfldr.append(ent->d_name)).c_str();
				tmp = opendir (sfull_path);
				std::cout<<"sful = "<<sfull_path<<"\n";
				if( tmp != NULL){
					closedir(tmp);
					ret = copyFolder(sfull_path, dfull_path);
				}
				else{
					ret = copyFile(sfull_path, dfull_path);
				}
				if(ret == false){
					//delete the folder dest
					std::cout<<"ret = false\n";
					//removeDir(dest);
					return false;
				}
			}
		}
		closedir (dir);
		return true;
	} 
	else {
		return false;
	}
}

bool FileManager::checkGame(const char* source, const char* gameName){
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (source)) != NULL) {
		 /* all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if(!strcmp(ent->d_name, gameName)){
				closedir (dir);
				return true;
			}
		}
		closedir (dir);
		return false;
	}
	closedir (dir);
	return true;
}

bool FileManager::writeJSON(const char* filename, const char* jsonStr){
	FILE* dest_pointer = fopen(filename, "wb");
	if(dest_pointer==NULL)
		return false;
	fwrite(jsonStr, 1, strlen(jsonStr), dest_pointer);
    fclose(dest_pointer);
	return true;
}

JSValue FileManager::parseJSON(Jzon::Node &node){
	JSObject jsObj;
	JSArray jsAr;
	std::cout<<"Inside"<<std::endl;
	if (node.IsValue()){
		switch (node.AsValue().GetValueType()){
			case Jzon::Value::VT_NULL   : return JSValue::Null();
			case Jzon::Value::VT_STRING : std::cout<<node.ToString().c_str()<<std::endl; return JSValue(WSLit(node.ToString().c_str()));
			case Jzon::Value::VT_NUMBER : std::cout<<node.ToFloat()<<std::endl; return JSValue(node.ToFloat());
			case Jzon::Value::VT_BOOL   : std::cout<<node.ToBool()<<std::endl; return JSValue(node.ToBool());
		}
	}
	else if (node.IsArray()){
		Jzon::Array &stuff = node.AsArray();
		for (Jzon::Array::iterator it = stuff.begin(); it != stuff.end(); ++it){
			jsAr.Push(FileManager::parseJSON((*it)));
		}
		return JSValue(jsAr);
	}
	else if (node.IsObject()){
		Jzon::Object stuff = node.AsObject();
		for (Jzon::Object::iterator it = stuff.begin(); it != stuff.end(); ++it){

			std::string name = (*it).first;
			Jzon::Node &node1 = (*it).second;
			jsObj.SetProperty(WSLit(name.c_str()), FileManager::parseJSON(node1));
		}
		return JSValue(jsObj);
	}
	return JSValue(jsObj);
}

JSValue FileManager::readJSON(const char* filename){
	Jzon::Object rootNode;
    Jzon::FileReader::ReadFile(filename, rootNode);
	std::cout<<filename<<std::endl;
	return FileManager::parseJSON(rootNode);
}

FileManager* FileManager::s_pInstance=0;