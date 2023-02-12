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

#include "Board.hpp"
#include "BoardView.hpp"
#include "SharedMemory.hpp"

using namespace wayland;

class Client {
private:
	bool running;
	
	int width = 1080;
	int height = 1080;
	//global objects
	display_t display;
	registry_t registry;
	compositor_t compositor;
	subcompositor_t subcompositor;
	xdg_wm_base_t xdg_wm_base;
	shm_t shm;
	output_t output;
	seat_t seat;
	
	//local objects
	surface_t surface;
	xdg_surface_t xdg_surface;
	xdg_toplevel_t xdg_toplevel;
	shm_pool_t pool;
	buffer_t buffer;
	pointer_t pointer;
	cursor_image_t cursor_image;
	buffer_t cursor_buffer;
	surface_t cursor_surface;	
	SharedMemory memory;
	
	void OnInit();
	void OnEvent();
	
	BoardView board_view;
	Board board;
	
	double cur_x, cur_y;
	bool pressed;
	
public:
	Client();
	void run();
};


