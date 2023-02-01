#ifndef _BACKGAMMON_h_
	#define _BACKGAMMON_h_
	
#include <SDL2/SDL.h>
#include <fmt/core.h>

using namespace std;

class Backgammon {
	private: 
		bool Running {true};
		SDL_Window *window{NULL};
		SDL_Renderer *renderer {NULL};
		
	public:
		int OnExecute();
		bool OnInit();
		void OnEvent(SDL_Event *event);
		void OnLoop();
		void OnRender();
		void OnCleanup();
};

#endif
