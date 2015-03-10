#ifndef _ACTIONTYPES_
#define _ACTIONTYPES_
#include "GameObject.h"
#include "Vector2D.h"
class GameObject;
class ActionTypes{
public:
	void setVelocity(GameObject*,Vector2D);
	void setPosition(GameObject*,Vector2D);
	void setState(GameObject*,std::string);
	static ActionTypes* Instance(){
		if(s_pInstance==0){
			s_pInstance=new ActionTypes();
		}
		return s_pInstance;
	}
private:
	static ActionTypes* s_pInstance;
};
#endif