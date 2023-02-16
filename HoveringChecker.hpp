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
	double offset_x, offset_y;
	bool color;
	unsigned char *background;
public:
	HoveringChecker(int point, int checker, bool color, double offset_x, double offset_y);
	~HoveringChecker();
	int get_point();
	int get_checker();
	bool get_color();
	void set_background(void *mem, double cur_x, double cur_y);
	unsigned char *get_background();
	double get_x();
	double get_y();
	double get_x_offset();
	double get_y_offset();
	bool is_drawable(double cur_x, double cur_y);
};
