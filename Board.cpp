#include "Board.hpp"

Board::Board() {
	
	//open image file
	FILE* file = fopen("board.png", "rb");
	
	//initialize png structure
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	
	//read png header
	png_init_io(png_ptr, file);
	png_read_info(png_ptr, info_ptr);
	
	//image info
	
	int bit_depth, color_type, interlace_type;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

	
	int size = width * height * 4;
	int stride = width * 4;
	image = new png_bytep[height];
	for(int i = 0; i < height; i++) {
		image[i] = new png_byte[stride];
	}
	png_read_image(png_ptr, image);
	
}

png_bytep *Board::get_image() {
	return image;
}

std::vector<int8_t> &Board::get_position() {
	return position;
}

void Board::draw_position(surface_t &surface, void *mem) {
	int stride = width * 4;
	for(int i = 0; i < height; i++) {
		memcpy(static_cast<uint32_t*>(mem) + i * width, image[i] , stride);
	}

	Cairo::RefPtr<Cairo::ImageSurface> imageSurface = Cairo::ImageSurface::create((unsigned char *)mem, 			Cairo::FORMAT_ARGB32, width, height, stride);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(imageSurface);
	for(int i = 0; i < position.size(); i++) {
		if(position[i] != 0) {
			if(position[i] < 0) cr->set_source_rgb(1,0,0);
			else if(position[i] > 0) cr->set_source_rgb(0,0,1);
			for(int j = 0; j < abs(position[i]); j++) {
				if(i < 6) {
					cr->arc(1010 - i * 80, 1010 - j * 80, 40, 0, 2 * M_PI);
				}else if(i < 12) {
					cr->arc(950 - i * 80, 1010 - j * 80, 40, 0, 2 * M_PI);
				}else if(i < 18) {
					cr->arc(70 + (i - 12) * 80, 70 + j * 80, 40, 0, 2 * M_PI);
				}else if(i < 24) {
					cr->arc(130 + (i - 12) * 80, 70 + j * 80, 40, 0, 2 * M_PI);
				}
				cr->fill();
			}		
		}
	}
	surface.damage(0, 0, width, height);
	surface.commit();
}

void Board::hover_checker(int x, int y, int last_hover_x, int last_hover_y, surface_t &surface, void *mem) {

	fmt::print("x: {}, y: {}\n", x, y);
	for(int i = 0; i < 80; i++) {
		memcpy(static_cast<uint32_t*>(mem) + width * (y + i) + x, image[y + i] + x * 4, 80 * 4);
	}
	surface.damage(x, y, 80, 80);
	surface.commit();
	
}
