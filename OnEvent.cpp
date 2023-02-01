#include "Backgammon.h"

void Backgammon::OnEvent(SDL_Event *event) {
	if(event->type == SDL_QUIT) {
		Running = false;
	}else if(event->type == SDL_KEYDOWN) {
		switch(event->key.keysym.sym) {
			case SDLK_COMMA:
			fmt::print("up\n");
			break;
			case SDLK_o:
			fmt::print("down\n");
			break;
			case SDLK_a:
			fmt::print("left\n");
			break;
			case SDLK_e:
			fmt::print("right\n");
			break;
		}
	}
}
