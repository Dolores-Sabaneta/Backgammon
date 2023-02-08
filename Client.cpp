#include "Client.hpp"

Client::Client() {
		OnInit();
		OnEvent();
		board.draw_position(surface, mem);
}

void Client::run() {
	running = true;
	while(running) {
		display.dispatch();
	}
}


