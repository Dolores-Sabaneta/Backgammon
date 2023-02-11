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
	SharedMemory memory;
	double x, y;
public:
	HoveringChecker(shm_t &shm, surface_t &surface);
	~HoveringChecker();
	surface_t &get_hovering_surface();
};
