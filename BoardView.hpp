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
	void start_hover(std::vector<int8_t> &position, surface_t &surface, void *mem, int point, int checker, bool color, shm_t &shm, surface_t &hovering_surface, subsurface_t &hovering_subsurface);
	void hover(double x, double y, surface_t &surface);
	void stop_hover(surface_t &surface, void *mem);
	
	bool is_hovering();
};
