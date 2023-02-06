#pragma once

#include <iostream>
#include <png.h>
#include <cstdio>
#include <fmt/core.h>
class Board {
private:
	png_bytep *image; //2D array of the image

public:
	Board();
	png_bytep *get_image();
};
