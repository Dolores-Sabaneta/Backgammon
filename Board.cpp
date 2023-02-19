#include "Board.hpp"

Board::Board() {
	
}

std::vector<int8_t> &Board::get_position() {
	return position;
}
void Board::move(int source, int destination) {
	//check source is on board
	if(source < 25 && source > 0) {
	
	}else throw std::invalid_argument("source is off board");
	
	//check destination is on board
	if(destination < 25 && source > 0) {
	
	}else throw std::invalid_argument("destination is off board");
	
	//check source is not empty
	if(position[source - 1] != 0) {
	
	}else throw std::invalid_argument("source is empty");
	
	//check destination and source are same color
	if((position[source-1]<0&&position[destination-1]<0)||(position[source-1]>0&&position[destination-1]>0) || position[destination - 1] == 0) {
	
	}else throw std::invalid_argument("source and destination are not same color");
	
	//check source and destination are not the same
	if(source != destination) {
	
	}else throw std::invalid_argument("source and destination are the same");
	
	if(position[source - 1] < 0) {
		position[source - 1]++;
		position[destination - 1]--;
	}else {
		position[source - 1]--;
		position[destination - 1]++;
	}
	
}
