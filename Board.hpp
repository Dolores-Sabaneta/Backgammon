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
	std::vector<int8_t> position {2, 0, 0, 0, 0, -5, 0, -3, 0, 0, 0, 6, -7, 0, 0, 0, 3, 0, 7, 0, 0, 0, 0, -2, 0, 0, 0, 0};

public:
	Board();
	std::vector<int8_t> &get_position();
};
