#include "Model.h"
Animation* Model::getAnimation(std::string animation_name){
	for(std::vector<int>::size_type i = 0; i != animations.size(); i++) {
		if(!animations[i]->animation_name.compare(animation_name)){
			return animations[i];
		}
	}
}
bool Model::load_texture(std::string file_name){
	SDL_Surface* pTempSurface=IMG_Load(file_name.c_str());
	if(pTempSurface)
		std::cout<<"image found\n";
	else{
		std::cout<<IMG_GetError();
		return false;
	}
	texture=SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(),pTempSurface);
	SDL_FreeSurface(pTempSurface);
	return true;
}