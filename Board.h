#ifndef _BOARD_h_
	#define _BOARD_h_

#include <vector>
#include <utility>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>



using namespace std;

class Board {
	public:
		Board(uint8_t match_points) : match_points{match_points}{}
		void move(uint8_t source_checker, uint8_t destination_checker);
		void roll_dice();
		void double_stake();
		void reset_board();
		void render(SDL_Renderer *renderer);
		
		vector<int8_t> &get_position();
		vector<uint8_t> &get_available_dice();
		pair<uint8_t, uint8_t> &get_dice();
		pair<uint8_t, uint8_t> &get_score();
		uint8_t get_match_points();
		uint8_t get_stake();
		bool get_doubling_dice_owner();
		bool get_active_player();
		 
	private:
		//first 24 items are the points
		//negative number of white checkers
		// 0 for no checkers
		//positive number of black checkers
		// last four are  number of white bar[24], black bar[25], white off[26], black off[27]
		vector<int8_t> position
		{2, 0, 0, 0, 0, -5, 0, -3, 0, 0, 0, 5, -5, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, -2, 0, 0, 0, 0};
		pair<uint8_t, uint8_t> dice {0, 0};
		vector<uint8_t> available_dice {1, 1};
		pair<uint8_t, uint8_t> score {0, 0};
		
		uint8_t match_points{0};
		uint8_t stake{0};
		
		bool doubling_dice_owner{0};
		// 0 is white, 1 is black
		bool active_player{0};
};
#endif
