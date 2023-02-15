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
	int point, checker;
	double x, y;
	bool color;
	unsigned char *background;
public:
	HoveringChecker(int point, int checker, bool color);
	~HoveringChecker();
	int get_point();
	int get_checker();
	bool get_color();
	void set_background(void *mem, double movement_x, double movement_y);
	unsigned char *get_background();
	double get_x();
	double get_y();
};
