#include "GameObjectManager.h"
void GameObjectManager::addObject(GameObject* game_object){
	game_object_list.push_back(game_object);
}
GameObject* GameObjectManager::getObject(std::string object_id){
	return id_object_map[object_id];
}
GameObjectManager* GameObjectManager::s_pInstance=0;