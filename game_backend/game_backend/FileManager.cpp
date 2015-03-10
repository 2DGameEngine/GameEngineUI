#include "FileManager.h"
#include "Jzon.h"
#include<iostream>

std::map<int, retJSON> FileManager::initializeObjects(const char* source){
	DIR *dir;
	struct dirent *ent;
	std::string filename, s_full = "";
	std::map<int, retJSON> js;
	int i = 0;
	if ((dir = opendir (source)) != NULL) {
		 /* all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if(strcmp(ent->d_name,".") && strcmp(ent->d_name,"..")){
				filename = ent->d_name;
				if(filename.find(".json") != std::string::npos){
					s_full = source;
					s_full += "\\";
					s_full += filename;
					js[i++] = FileManager::readJSON(s_full.c_str());
				}
			}
		}
		closedir (dir);
		return js;
	}
	closedir (dir);
	return js;
}

struct retJSON FileManager::parseJSON(Jzon::Node &node){
	retJSON ret;
	ret.type = "undefined";
	std::map<std::string, retJSON> mapVal;
	if (node.IsValue()){
		switch (node.AsValue().GetValueType()){
		case Jzon::Value::VT_NULL   : ret.type = "null";return ret;
		case Jzon::Value::VT_STRING : std::cout<<node.ToString().c_str()<<std::endl;ret.type = "string"; ret.stringVal=(char*)node.ToString().c_str();return ret;
			case Jzon::Value::VT_NUMBER : std::cout<<node.ToFloat()<<std::endl;
				if(node.ToFloat() == (float)node.ToInt()){
					ret.type = "int"; ret.retVal.intVal=node.ToInt();
				}
				else{
					ret.type = "float"; ret.retVal.floatVal=node.ToFloat();
				}
				return ret;
			case Jzon::Value::VT_BOOL: std::cout<<node.ToBool()<<std::endl;
				ret.type = "bool"; ret.retVal.boolVal=node.ToBool();return ret;
		}
	}
	else if (node.IsArray()){
		Jzon::Array &stuff = node.AsArray();
		ret.type = "array";
		int i;
		Jzon::Array::iterator it = stuff.begin();
		for (i=0; it != stuff.end(); ++it, i++){
			char st[2];
			ret.mapVal[itoa(i,st,10)] = FileManager::parseJSON((*it));
		}
		return ret;
	}
	else if (node.IsObject()){
		Jzon::Object stuff = node.AsObject();
		ret.type = "object";
		for (Jzon::Object::iterator it = stuff.begin(); it != stuff.end(); ++it){
			ret.mapVal[(*it).first] = FileManager::parseJSON((*it).second);
		}
		return ret;
	}
	return ret;
}

struct retJSON FileManager::readJSON(const char* filename){
	Jzon::Object rootNode;
    Jzon::FileReader::ReadFile(filename, rootNode);
	std::cout<<filename<<std::endl;
	return FileManager::parseJSON(rootNode);
}

struct retJSON FileManager::returnVALUE(retJSON ret, char *index){
	retJSON Tret;
	std::map<std::string, retJSON>::iterator Tret1;
	Tret.type = "undefined";
	if(ret.type == "int" || ret.type == "float" || ret.type == "bool" || ret.type == "string" || ret.type == "undefined")
		return Tret;
	else if(ret.type == "array" || ret.type == "object"){
		Tret1 = ret.mapVal.find(index);
		if(Tret1 == ret.mapVal.end()){
			return Tret;
		}
		return Tret1->second;
	}
	return Tret;
}

FileManager* FileManager::s_pInstance=0;