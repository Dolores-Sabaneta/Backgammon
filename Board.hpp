#pragma once

#include <iostream>
#include <png.h>
#include <cstdio>
#include <fmt/core.h>
#include <vector>

#include <wayland-client.hpp>
#include <wayland-client-protocol-extra.hpp>

#include <cairomm/surface.h>
#include <cairomm/context.h>

using namespace wayland;

class Board {
private:	
	std::vector<int8_t> position {2, 0, 0, 0, 0, -8, 0, -3, 0, 0, 0, 8, -5, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0, -2, 0, 0, 0, 0};
	std::pair<uint8_t, uint8_t> dice;
	std::vector<uint8_t> available;
	//0 is white, 1 is black
	bool active{0};
	
	
public:
	Board();
	std::vector<int8_t> &get_position();
	void move(int source, int destination);
};
