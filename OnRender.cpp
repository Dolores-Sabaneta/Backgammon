#include "Backgammon.h"

void Backgammon::OnRender() {
	int width;
	int height;
	
	SDL_GetRendererOutputSize(renderer, &width, &height);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}
