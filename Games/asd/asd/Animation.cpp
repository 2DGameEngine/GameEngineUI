#include "Animation.h"
Animation::Animation(std::string animation_name,std::vector<std::pair<int,int>> frame_ids,float speed){
	Animation::animation_name=animation_name;
	Animation::frame_ids=frame_ids;
	Animation::speed=speed;
}