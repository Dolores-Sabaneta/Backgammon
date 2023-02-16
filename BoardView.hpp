#pragma once

#include <iostream>
#include <png.h>
#include <cstdio>
#include <fmt/core.h>
#include <vector>
#include <algorithm>

#include <wayland-client.hpp>
#include <wayland-client-protocol-extra.hpp>

#include <cairomm/surface.h>
#include <cairomm/context.h>
#include <cairo-ft.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "SharedMemory.hpp"
#include "HoveringChecker.hpp"

using namespace wayland;
class BoardView {
private:
	png_bytep *image; //2D array of the image
	png_uint_32 height, width;
	HoveringChecker *hovering_checker = nullptr;
	Cairo::RefPtr<Cairo::Context> cr;
	surface_t surface;
	void *mem;
	bool hovering{false};
public:
	BoardView();
	void init(surface_t &surface, void *mem);
	void draw(std::vector<int8_t> &position);
	
	void checker_pressed(std::vector<int8_t> &position, double cur_x, double cur_y);
	void checker_hover(double cur_x, double cur_y);
	void checker_off(std::vector<int8_t> &position);
	bool is_hovering();
};
