#ifndef _GAMEOBJECTMANAGER_
#define _GAMEOBJECTMANAGER_
#include "GameObject.h"
#include <vector>
#include <map>
class GameObject;
class GameObjectManager{
public:
	static GameObjectManager* Instance(){
		if(s_pInstance==0){
			s_pInstance=new GameObjectManager();
		}
		return s_pInstance;
	}
	void addObject(GameObject*);
	std::vector<GameObject*> getObjectList(){
		return game_object_list;
	}
	GameObject* getObject(std::string);
private:
	std::vector<GameObject*> game_object_list;
	static GameObjectManager* s_pInstance;
	std::map <std::string,GameObject*> id_object_map;
};
#endif