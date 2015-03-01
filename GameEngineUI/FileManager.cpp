#include "FileManager.h"

bool FileManager::copyFile(const char* source,const char* dest){
	FILE* source_pointer = fopen(source, "rb");
	FILE* dest_pointer = fopen(dest, "wb");
	if(source_pointer==NULL||dest_pointer==NULL)
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
bool FileManager::copyFolder(const char* source, const char* dest){
	DIR *dir, *tmp;
	struct dirent *ent;
	std::string sfldr;
	std::string dfldr;
	const char* sfull_path;
	const char* dfull_path;
	createDir(dest);
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
				if( tmp != NULL){
					closedir(tmp);
					copyFolder(sfull_path, dfull_path);
				}
				else{
					copyFile(sfull_path, dfull_path);
				}
			}
		}
		closedir (dir);
	} 
	else {
		return false;
	}
}
FileManager* FileManager::s_pInstance=0;