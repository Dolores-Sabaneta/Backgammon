#include "BoardView.hpp"

BoardView::BoardView() {
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

void BoardView::draw(std::vector<int8_t> &position, surface_t &surface, void *mem) {
	int stride = width * 4;
	for(int i = 0; i < height; i++) {
		memcpy(static_cast<uint32_t*>(mem) + i * width, image[i] , stride);
	}
	
	Cairo::RefPtr<Cairo::ImageSurface> imageSurface = Cairo::ImageSurface::create((unsigned char *)mem, Cairo::FORMAT_ARGB32, width, height, stride);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(imageSurface);
	
	int x = 1010;
	int y = 1010;
	int gravity = -80;
	for(int i = 0; i < 24; i++) {
		if(position[i] != 0) {
			position[i] < 0 ? cr->set_source_rgb(0, 0, 1) : cr->set_source_rgb(1, 0, 0);		
			if(i < 12) {
				i < 6 ? x = 1010 - i * 80 : x = 950 - i * 80;
				y = 1010;
			}else if(i < 24) {
				i < 18 ? x = 70 + (i - 12) * 80 : x = 130 + (i - 12) * 80;
				y = 70;
				gravity = 80;
			}
			for(int j = 0; j < abs(position[i]); j++){
				cr->arc(x, y, 40, 0, 2 * M_PI);
				y += gravity;
				cr->fill();
			}
		}
	}
	surface.damage(0, 0, 1080, 1080);
	surface.commit();
}

void BoardView::start_hover(std::vector<int8_t> &position, surface_t &surface, void *mem, int point, int checker, shm_t &shm, surface_t &hovering_surface) {
	hovering = true;
	fmt::print("a\n");
	hovering_checker = new HoveringChecker(shm, hovering_surface);
	fmt::print("b\n");
	int x = 0;
	int y = 0;
	if(point < 13) {
		y = 650;
		point < 7 ? x = 1050 - point * 80 : x = 990 - point * 80;
	}else {
		y = 30;
		point < 19 ? x = 30 + (point - 12) * 80 : x = 90 + (point - 12) * 80;
	}
	fmt::print("point: {}, checker: {}, x: {}, y: {}\n", point, checker, x, y);
	/*
	for(int i = 0; i < position[point - 1] * 80; i++) {
		memcpy(static_cast<uint32_t *>(mem) + width * (i + y) + x, image[i + y] + x * 4, 80 * 4);
	}
	surface.damage(0,0,80,80);
	surface.commit();
	*/
}

void BoardView::stop_hover() {
	hovering = false;
	delete hovering_checker;
}

bool BoardView::is_hovering() {
	return hovering;
}
