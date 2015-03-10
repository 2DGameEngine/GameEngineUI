#ifndef _GAME_
#define _GAME_
#include "SDL.h"
#include <iostream>
#include <vector>
#include "InputHandler.h"
#include <utility>
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Event.h"
#include "Action.h"
class GameObject;
class Game{
public:
	bool init(const char* title,int xpos,int ypos,int width,int height, bool fullscreen);
	void render();
	void update();
	void clean();
	bool running(){ return m_bRunning;}
	void quit(){m_bRunning=false;} 
	static Game* Instance(){
		if(s_pInstance==0){
			s_pInstance=new Game();
		}
		return s_pInstance;
	}
	SDL_Renderer* getRenderer() const { return m_pRenderer; }
private:
	Game(){};
	static Game* s_pInstance;
	bool m_bRunning;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
};
typedef Game TheGame;
#endif