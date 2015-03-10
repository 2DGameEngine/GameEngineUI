#ifndef _EVENT_
#define _EVENT_
#include <SDL.h>
#include "Action.h"
#include "InputHandler.h"
class Action;
enum event_types{
		BUTTON_CLICK,
		MOUSE_MOVE,
		VARIABLE_VALUE
	};
class Event{
public:
	void setEvent(event_types,SDL_Scancode);
	std::vector<Action*> action_list;
	void addAction(Action*);
	bool checkEvent();
	void handleEvent();
private:
	event_types event_type;
	SDL_Scancode scancode;

};
#endif