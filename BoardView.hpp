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
#include "SharedMemory.hpp"
#include "HoveringChecker.hpp"

using namespace wayland;
class BoardView {
private:
	png_bytep *image; //2D array of the image
	png_uint_32 height, width;
	HoveringChecker *hovering_checker = nullptr;
	bool hovering {false};
public:
	BoardView();
	void draw(std::vector<int8_t> &position, surface_t &surface, void *mem);
	void start_hover(std::vector<int8_t> &position, surface_t &surface, void *mem, int point, int checker, shm_t &shm, surface_t &hovering_surface);
	void hover(int x, int y);
	void stop_hover();
	
	bool is_hovering();
};
