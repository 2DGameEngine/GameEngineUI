#include <SDL.h>
#include <stdio.h>
#include <Windows.h>
#include "Game.h"
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#include "FileManager.h"
#include <iostream>
const int FPS=60;
const int DELAY_TIME=1000.0f/FPS;

int main(int argc,char* argv[]){
	UINT32 frameStart,frameTime;
	AllocConsole();
	freopen("CON","w",stdout);
	struct retJSON GameJSON, tmpJSON;
	std::string source = "\.\.", file = source;
	file += "\\Game.json";
	GameJSON = FileManager::Instance()->readJSON(file.c_str());
	int width=640, height=480;
	std::string gname = ""; 
	tmpJSON = FileManager::Instance()->returnVALUE(GameJSON, "Width");
	if(tmpJSON.type == "int"){
		width = tmpJSON.retVal.intVal;
	}
	tmpJSON = FileManager::Instance()->returnVALUE(GameJSON, "Height");
	if(tmpJSON.type == "int"){
		height = tmpJSON.retVal.intVal;
	}
	tmpJSON = FileManager::Instance()->returnVALUE(GameJSON, "Name");
	if(tmpJSON.type == "string"){
		gname.assign(tmpJSON.stringVal);
	}
	TheGame::Instance()->init(gname.c_str(),100,100,width,height,false);
	while(TheGame::Instance()->running()){
		frameStart=SDL_GetTicks();
		TheGame::Instance()->update();
		TheGame::Instance()->render();
		frameTime=SDL_GetTicks()-frameStart;
		if(frameTime<DELAY_TIME){
			SDL_Delay((int)(DELAY_TIME-frameTime));
		}
	}
	TheGame::Instance()->clean();
	return 0;
}