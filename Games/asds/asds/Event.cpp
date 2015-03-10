#include "Event.h"
void Event::addAction(Action* action){
	action_list.push_back(action);
}
void Event::setEvent(event_types e,SDL_Scancode scancode){
	event_type=e;
	Event::scancode=scancode;
}
bool Event::checkEvent(){
	if(event_type==BUTTON_CLICK){
		return InputHandler::Instance()->isKeyDown(scancode);
	}
}
void Event::handleEvent(){
	if(checkEvent()){
		for(std::vector<int>::size_type i = 0; i != action_list.size(); i++){ 
			action_list[i]->runAction();
		}
	}
}