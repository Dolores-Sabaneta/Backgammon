#include "Client.hpp"

Client::Client() : memory(width * height * 4){
		OnInit();
		OnEvent();
		//board.draw();
}

void Client::run() {
	running = true;
	while(running) {
		display.dispatch();
	}
}


