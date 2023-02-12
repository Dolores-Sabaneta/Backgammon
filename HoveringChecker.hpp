#pragma once

#include <stdexcept>
#include <iostream>
#include <array>
#include <memory>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <random>
 
#include <wayland-client.hpp>
#include <wayland-client-protocol.hpp>
#include <wayland-client-protocol-extra.hpp>
#include <linux/input.h>
#include <wayland-cursor.hpp>
 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fmt/core.h>
#include <png.h>
#include <cairomm/surface.h>
#include <cairomm/context.h>

#include "SharedMemory.hpp"

using namespace wayland;

class HoveringChecker{
private:
	shm_pool_t pool;
	buffer_t buffer;
	surface_t surface;
	subsurface_t subsurface;
	SharedMemory memory;
	double x, y;
	int point, checker;
	//0 is white, 1 is black
	bool color;
public:
	HoveringChecker(shm_t &shm, surface_t &surface, subsurface_t &subsurface, int point, int checker, bool color);
	~HoveringChecker();
	surface_t &get_surface();
	subsurface_t &get_subsurface();
	
	int get_point();
	int get_checker();
	bool get_color();
	
	//distance from center of the checker to cursor
	double get_x();
	double get_y();
	void set_x(double x);
	void set_y(double y);
	
	void *get_mem();
};
