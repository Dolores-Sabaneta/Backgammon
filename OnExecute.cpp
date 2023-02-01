#include "Backgammon.h"

int Backgammon::OnExecute() {
	if(OnInit() == false) return -1;
	
	SDL_Event event;
	const int frameDelay {1000 / 60};
	int frameStart;
	int frameTime;
	while(Running) {
		frameStart = SDL_GetTicks();
		while(SDL_PollEvent(&event)) {
			OnEvent(&event);
		}
		OnLoop();
		OnRender();
		
		frameTime = SDL_GetTicks() - frameStart;
		if(frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	OnCleanup();
	return 0;
}
