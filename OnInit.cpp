#include "Backgammon.h"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 600;

bool Backgammon::OnInit() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fmt::print("SDL could net initialize! SDL_Error: {}\n", SDL_GetError());
		return false;
	}
	
	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	
	if(window == NULL) {
		fmt::print("Window could not be created! SDL_Error: {}\n", SDL_GetError());
		return false;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) {
		fmt::print("Renderer could not be cerated! SDL_Error: {}\n", SDL_GetError());
		return false;
	}
	
	return true;
}
