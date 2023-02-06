#include "Client.hpp"

Client::Client() {
		fmt::print("B");
		OnInit();
		fmt::print("E");
		OnEvent();
		fmt::print("D");
		draw();
}

void Client::run() {
	running = true;
	while(running) {
		display.dispatch();
	}
}

void Client::draw() {
	//std::fill_n(static_cast<uint32_t*>(mem), 1920 * 1080, 0xff2b76c3);
	//memcpy(mem, board.get_image(), 1920 * 1080 * 4);
	fmt::print("A");
	int stride = width * 4;
	/*
	for(int i = 0; i < height; i++) {
		fmt::print("C");
		memcpy(mem, board.get_image(), stride);
	}
	*/
	surface.attach(buffer, 0, 0);
	surface.damage(0, 0, 1920, 1080);
	surface.commit();	
}


