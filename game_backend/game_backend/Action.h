#ifndef _ACTION_
#define _ACTION_
#include <string>
#include "ActionTypes.h"
class Action{
public:
	void runAction();
	
	Action(std::string action_type,GameObject* game_object,Vector2D vector){
		Action::action_type=action_type;
		Action::vector=vector;
		Action::game_object=game_object;
	}
	Action(std::string action_type,GameObject* game_object,std::string string){
		Action::action_type=action_type;
		Action::string=string;
		Action::game_object=game_object;
	}
private:
	std::string action_type;
	GameObject* game_object;
	Vector2D vector;
	std::string string;
};
#endif