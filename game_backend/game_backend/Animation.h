#ifndef _ANIMATION_
#define _ANIMATION_
#include <string>
#include <vector>
#include "Model.h"
class Animation{
private:
	std::string animation_name;
	std::vector<std::pair<int,int>> frame_ids;
	friend class Model;
	friend class GameObject;
public:
	Animation(std::string,std::vector<std::pair<int,int>>,float);
	float speed;
};
#endif