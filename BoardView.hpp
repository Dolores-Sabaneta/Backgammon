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
#include "Board.hpp"

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
	
	Board board;
	//std::vector<int8_t> state; //memory of what has been drawn, when position changes we only draw the changes
	void draw_move(int source, int destination);
public:
	BoardView();
	void init(surface_t &surface, void *mem);
	void draw();
	
	void checker_pressed(double cur_x, double cur_y);
	void checker_hover(double cur_x, double cur_y);
	void checker_off(double cur_x, double cur_y);
	bool is_hovering();
};
