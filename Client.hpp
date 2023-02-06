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
#include <wayland-client-protocol-extra.hpp>
#include <linux/input.h>
#include <wayland-cursor.hpp>
 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fmt/core.h>

#include "Board.hpp"
#include <png.h>

using namespace wayland;

class Client {
private:
	//global objects
	display_t display;
	registry_t registry;
	compositor_t compositor;
	xdg_wm_base_t xdg_wm_base;
	shm_t shm;
	
	//local objects
	surface_t surface;
	xdg_surface_t xdg_surface;
	xdg_toplevel_t xdg_toplevel;
	buffer_t buffer;
	bool running;
	int fd;
	void *mem = nullptr;
	
	void draw();
	
	void OnInit();
	void OnEvent();
	
	int width = 1920;
	int height = 1080;
	
	Board board;
	
public:
	Client();
	void run();
};


