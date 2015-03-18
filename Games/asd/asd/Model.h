#ifndef _MODEL_
#define _MODEL_
#include <SDL.h>
#include <string>
#include "Animation.h"
#include <vector>
#include "Game.h"
#include <SDL_image.h>
class Animation;
class Model{
private:
	SDL_Texture* texture;
	int width_of_frame;
	int height_of_frame;
	std::string file_name;
	std::vector<Animation*> animations;
	std::string active_animation;
	friend class GameObject;
public:
	//void drawFrame();
	void set_width_and_height_frame(int width,int height){
		Model::width_of_frame=width;
		Model::height_of_frame=height;
	}
	bool load_texture(std::string file_name);
	Animation* getAnimation(std::string);
	void addAnimation(Animation* animation){
		animations.push_back(animation);
	}
};
#endif