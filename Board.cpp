#include "Board.h"
#include <stdexcept>
#include <cstdlib>
#include <ctime>


void Board::move(int8_t source, int8_t destination) {
//client doesn't need to worry about how we implement source and destination internally.
//for both black and white, 1 is the first point of home, 24 is the last one.
//for both black and white, 25 is the bar, 26 is off

uint8_t dice_used;
if(!active_player) { //white to play
	if(source > 0 && source < 25) { //source is on the board
		if(position[[24] == 0) { // bar is not empty
			if(position[source - 1] < 0) { //source has white checkers
				if(destination > 1 && destination < 25) { //destination is on the board
					if(position[destination - 1] < 2) { //either we hit, or destination is good
						bool available = false;
						distance = source - destination; //distance from source to destination
						for(int i = 0; i < available_dice.size; i++) { //check if we have available dice
							if(available_dice[i] == distance) { //dice available
								available_dice.erase(i);
								position[source - 1]++; //remove a white checker from source
								if(position[destination - 1] == 1) { //we hit
									destination[25]++;	//we put black checker on bar
									position[destination - 1] = -1; //destination has 1 white checker
								}else { //we don't hit
									position[destination - 1]--; //add a white checker to destination
								}
								available = true;
								break;
							}
						}
						if(!available) throw invalid_argument{"No dice available"};					
					}else {
						throw invalid_argument("Illegal move, more than 2 black checkers at destination");
					}
				}else if(destination == 26) { //destination is off
					if(source < 7) { //source is inside of home
						for(int i = 23; i > 5; i--) { //check if there are white checkers outside of home
							if(position[i] < 0) {
								throw invalid_argument{"Can't bear off checkers still outside of home"};
							}
						}
						//we continue, there are no white checkers outside of home
						int distance = source;
						bool available = false;
						//check we have specific dice to bear off
						for(int i = 0; i < available_dice.size; i++) { 
							if(available_dice[i] == distance) { //we have the specific dice to bear off
								available_dice.erase(i);
								position[source - 1]++; //we remove white checker from source
								position[26]++; //we add one checker to white off
								available = true;
								break;
							}
						}
						//we don't have specific dice to bear off
						if(!available) {
							for(int i = 5; i > source - 1; i--) { // check if source is the highest point
								if(position[i] < 0) {
									throw invalid_argument{"Can't bear off, no dice 	available"};
								}
							}
							//we continue, source is the highest point
							bool available = false;
							//check we have bigger available dice than source 
							for(int i = 0; i < available_dice.size; i++) { 
								if(available_dice[i] > source) {
									available_dice.erase(i);
									position[source - 1]++; //remove a white checker at source
									position[26]++; //add a white checker at off
									available = true;
									break;
								}
							}
							if(!available) throw invalid_argument{"No dice available to bear off"};
						}			
					}else {
						throw invalid_argument{"Can't bear off checkers outside of home"};
					}
				}else {
					throw invalid_argument("Invalid destination, out of bounds");
				}
			}else if(position[source - 1] == 0) {
				throw invalid_argument("Invalid source, no checkers to move");
			}else {
				throw invalid_argument("Can't move black checkers");
			}
		}else {
			throw invalid_argument("Can't move from board, there are white checkers on bar");
		}
	}else if(source == 25) { //source is bar
		if(source < 0) { //bar has at least a white checker
			if(destination > 18 && destination < 25) { //destination is inside black home
				if(position[destination - 1] < 2) { //we either enter and hit or just enter
					bool available = false;
					int distance = 25 - destination; //distance we need a dice for
					for(int i = 0; i < available_dice.size; i++) { //check we have dice to enter
						if(x == distance) { //we have dice
							available_dice.erase(i);
							position[24]--; //we remove a white checker from bar
							if(position[destination - 1] == 1) { //we hit
								position[destination - 1] = -1; //put one white checker at destination
								position[25]++; //we move black to bar
							}else { //we don't hit
								position[destination - 1]--; //we add one white checker to destination
							}
							available = true;
							break;
						}
					}
					if(!available) throw invalid_argument{"No dice available to enter"};	
				}else {
					throw invalid_argument{"Can't enter, destination has more than 2 black checkers"};
				}
			}else {
				throw invalid_argument{"can't enter outside your black's home"};
			}
		}else {
			throw invalid_argument{"bar is empty, invalid source"};
		}
	}else {
		throw invalid_argument("Invalid source, out of bounds");
	}
}else { //black to play
	if(source > 0 && source < 25) { //source is on the board
		if(position[25] == 0) {
			if(position[24 - source] > 0) { //source has black checkers
				if(destination > 1 && destination < 25) { //destination is on the board
					if(position[24 - destination] > -2) { //either we hit, or destination is good
						bool available = false;
						distance = source - destination; //distance from source to destination
						for(int i = 0; i < available_dice.size; i++) { //check if we have available dice
							if(available_dice[i] == distance) { //dice available
								available_dice.erase(i);
								position[24 - source]--; //remove a black checker from source
								if(position[24 - destination] == -1) { //we hit
									destination[24]++;	//we put white checker on bar
									position[24 - destination] = 1; //destination has 1 black checker
								}else { //we don't hit
									position[24 - destination]++; //add a white checker to destination
								}
								available = true;
								break;
							}
						}
						if(!available) throw invalid_argument{"No dice available"};					
					}else {
						throw invalid_argument("Illegal move, more than 2 white checkers at destination");
					}
				}else if(destination == 26) { //destination is off
					if(source < 7) { //source is inside of home
						for(int i = 0; i < 18; i++) { //check if there are black checkers outside of home
							if(position[i] > 0) {
								throw invalid_argument{"Can't bear off checkers outside of home"};
							}
						}
						//we continue, there are no white checkers outside of home
						int distance = source;
						bool available = false;
						//check we have specific dice to bear off
						for(int i = 0; i < available_dice.size; i++) { 
							if(available_dice[i] == distance) { //we have the specific dice to bear off
								available_dice.erase(i);
								position[24 - source]++; //we remove black checker from source
								position[27]++; //we add one checker to black off
								available = true;
								break;
							}
						}
						//we don't have specific dice to bear off
						if(!available) {
							for(int i = 18; i < 24 - source; i++) { // check if source is the highest point
								if(position[i] > 0) {
									throw invalid_argument{"Can't bear off checkers, no dice available"};
								}
							}
							//we continue, source is the highest point
							bool available = false;
							//check we have bigger available dice than source
							for(int i = 0; i < available_dice.size; i++) {
								if(available_dice[i] > source) {
									available_dice.erase(i);
									position[24 - source]--; //remove a black checker at source
									position[27]++; //add a black checker at off
									available = true;
									break;
								}
							}
							if(!available) throw invalid_argument{"No dice available to bear off"};
						}			
					}else {
						throw invalid_argument{"Can't bear off checkers outside of home"};
					}
				}else {
					throw invalid_argument("Invalid destination, out of bounds");
				}
			}else if(position[24 - source] == 0) {
				throw invalid_argument("Invalid source, no checkers to move");
			}else {
				throw invalid_argument("Can't move black checkers");
			}
		}
	}else if(source == 25) { //source is bar
		if(source < 0) { //bar has at least a white checker
			if(destination > 18 && destination < 25) { //destination is inside black home
				if(position[24 - destination] > -2) { //we either enter and hit or just enter
					bool available = false;
					int distance = 25 - destination; //distance we need a dice for
					for(int i = 0; i < available_dice.size; i++) { //check we have dice to enter
						if(x == distance) { //we have dice
							available_dice.erase(i);
							position[25]--; //we remove a black checker from bar
							if(position[24 - destination] == -1) { //we hit
								position[24 - destination] = 1; //put one black checker at destination
								position[24]++; //we move white checker to bar
							}else { //we don't hit
								position[24 - destination]++; //we add one black checker to destination
							}
							available = true;
							break;
						}
					}
					if(!available) throw invalid_argument{"No dice available to enter"};	
				}else {
					throw invalid_argument{"Can't enter, destination has more than 2 black checkers"};
				}
			}else {
				throw invalid_argument{"can't enter outside your black's home"};
			}
		}else {
			throw invalid_argument{"bar is empty, invalid source"};
		}
	}else {
		throw invalid_argument("Invalid source, out of bounds");
	}
}
//TO DO check If opponent has at least one legal move and change active player if it is the case

	if(active_player) { //white has played so we check black has at least one legal move
		
	}else { //black has palyed so we check white has at least one legal move
		if(position[24] == 0) { //no white checkers on bar
				if(position[24 - dice.first] < 2 || position[24 - dice.second] < 2) { //we can enter
					active_player = 1; //active player becomes white
					return;
				}else { //we can't enter
					active_player = 0; //active player remains black
					return;
				}
		}else{ //there are white checkers on bar
			for(int i = 23; i > 5; i++) { //check if there are white checkers outside of home
				if(position[i] < 0) { //there are white checkers outside of home
					for(int j = 23; j >= 0; j--) { //we check if there is at least one movable white checker
						//we check that the point has a white checker and the dice doesn't go out of bounds
						if(position[j] < 0 && j - dice.first >= 0 && j - dice.second >= 0) {
							if(position[j - dice.first] < 2 || position[j - dice.second] < 2) { //we can move
								active_player = 1; //active player becomes white
								return;
							}
						}
					}
					//we continue, there are no movable white checkers.
					active_player = 0; //active player remains black
					return;
				}
			}
			//we continue, there are no white checkers outside of home
			int i = 5
			for(; i <= 0; i--) { //serch the highest point
				if(position[i] < 0) { //we found the highest point
					//check we have dice bigger than highest point
					if(i <= dice.first - 1 || i <= dice.second - 1) {
						active_player = 1; //active player becomes black
						return;
					}
					i--;
					break;
				}
			}
			for(; i <= 0; i--) { //check we have specific dice to bear off
				if(position[i] < 0) {
					if(i == dice.first - 1 || i == dice.second - 1) {
						active_player = 1; //active player becomes black
						return
					}
				}
			}
			active_player = 0; // active player becomes white
			return
		}
	}
}


void Board::roll_dice() {
	srand(clock());
	dice.first = rand() % 5 + 1;
	dice.second = rand() % 5 + 1;
	
	if(dice.first == dice.second) {
		available_moves = {dice.first, dice.first, dice.first, dice.first};
	}else {
		available_moves = {dice.first, dice.second};
	}

