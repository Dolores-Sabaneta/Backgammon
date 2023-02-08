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
	png_bytep *image; //2D array of the image
	png_uint_32 height, width;
	
	std::vector<int8_t> position {2, 0, 0, 0, 0, -5, 0, -3, 0, 0, 0, 5, -5, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, -2, 0, 0, 0, 0};

public:
	Board();
	png_bytep *get_image();
	std::vector<int8_t> &get_position();
	void draw_position(surface_t &surface, void *mem);
	void hover_checker(int x, int y, int last_hover_x, int last_hover_y, surface_t &surface, void *mem);
};
