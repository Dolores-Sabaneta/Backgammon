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
	FT_Library library;
	FT_Face face;
	FT_Init_FreeType(&library);
	FT_New_Face(library, "Roboto-Black.ttf", 0, &face);
	FT_Set_Pixel_Sizes(face, 0, 32);
	cr->select_font_face("Roboto-Black", Cairo::FONT_SLANT_NORMAL,
	Cairo::FONT_WEIGHT_BOLD);
	cr->set_font_size(64);

	int x = 1010;
	int y = 1010;
	int gravity = -80;
	for(int i = 0; i < 24; i++) {
		if(position[i] != 0) {
			position[i] < 0 ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);		
			if(i < 12) {		
				i < 6 ? x = 1010 - i * 80 : x = 950 - i * 80;
				y = 1010;
			}else if(i < 24) {
				i < 18 ? x = 70 + (i - 12) * 80 : x = 130 + (i - 12) * 80;
				y = 70;
				gravity = 80;
			}
			for(int j = 0; j < std::min(abs(position[i]), 5); j++){
				cr->arc(x, y, 40, 0, 2 * M_PI);
				y += gravity;
				cr->fill();
			}
			if(abs(position[i]) > 5) {
				cr->set_source_rgb(1, 1, 1);
				i < 12 ? cr->move_to(x - 22, y + 104) : cr->move_to(x - 22, y - 56);
				cr->show_text(fmt::format("{}", abs(position[i])));
			}
		}
	}
	surface.damage(0, 0, 1080, 1080);
	surface.commit();
}

void BoardView::start_hover(std::vector<int8_t> &position, surface_t &surface, void *mem, int point, int checker, bool color, shm_t &shm, surface_t &hovering_surface, subsurface_t &hovering_subsurface) {
	hovering = true;
	hovering_checker = new HoveringChecker(shm, hovering_surface, hovering_subsurface, point, checker, color);
	int x = 0;
	int y = 0;
	if(point < 13) {
		y = 1050 - checker * 80;
		point < 7 ? x = 1050 - point * 80 : x = 990 - point * 80;
	}else {
		y = checker * 80 - 50;
		point < 19 ? x = 30 + (point - 13) * 80 : x = 90 + (point - 13) * 80;
	}
	fmt::print("point: {}, checker: {}, x: {}, y: {}\n", point, checker, x, y);
	
	Cairo::RefPtr<Cairo::ImageSurface> imageSurface = Cairo::ImageSurface::create((unsigned char *)hovering_checker->get_mem(), Cairo::FORMAT_ARGB32, 80, 80, 80 * 4);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(imageSurface);
	color == 0 ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
	
	if(abs(position[point - 1]) < 6) {
		for(int i = 0; i < 80; i++) {
			memcpy(static_cast<uint32_t *>(mem) + width * (i + y) + x, image[i + y] + x * 4, 80 * 4);
		}
	}else {
		cr->arc(40, 40, 40, 0, 2 * M_PI);
		cr->fill();
		if(abs(position[point - 1]) > 6) {
			FT_Library library;
			FT_Face face;
			FT_Init_FreeType(&library);
			FT_New_Face(library, "Roboto-Black.ttf", 0, &face);
			FT_Set_Pixel_Sizes(face, 0, 32);
			cr->select_font_face("Roboto-Black", Cairo::FONT_SLANT_NORMAL,
			Cairo::FONT_WEIGHT_BOLD);
			cr->set_font_size(64);
			cr->set_source_rgb(1, 1, 1);
			cr->show_text(fmt::format("{}", abs(position[point - 1]) - 1));
		}
	}
		
	hovering_checker->set_x(x);
	hovering_checker->set_y(y);
	
	
	cr->arc(40, 40, 40, 0, 2 * M_PI);
	cr->fill();
	
	//memset(hovering_checker->get_mem(), 0xFFFFFFFF, 80 * 80 * 4);
	hovering_subsurface.set_position(x, y);	
	hovering_surface.damage(0, 0, 80, 80);
	hovering_surface.commit();
	
	surface.damage(x, y, 80, 80);
	surface.commit();
}
void BoardView::hover(double x, double y, surface_t &surface) {
	hovering_checker->set_x(hovering_checker->get_x() + x);
	hovering_checker->set_y(hovering_checker->get_y() + y);
	hovering_checker->get_subsurface().set_position(hovering_checker->get_x(), hovering_checker->get_y());
	surface.commit();
}
void BoardView::stop_hover(surface_t &surface, void *mem) {
	
	int x = 0;
	int y = 0;
	int point = hovering_checker->get_point();
	int checker = hovering_checker->get_checker();
	if(point < 13) {
		y = 1050 - checker * 80;
		point < 7 ? x = 1050 - point * 80 : x = 990 - point * 80;
	}else {
		y = checker * 80 - 50;
		point < 19 ? x = 30 + (point - 13) * 80 : x = 90 + (point - 13) * 80;
	}
	int stride = width * 4;
	Cairo::RefPtr<Cairo::ImageSurface> imageSurface = Cairo::ImageSurface::create((unsigned char *)mem, Cairo::FORMAT_ARGB32, width, height, stride);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(imageSurface);
	hovering_checker->get_color() == 0 ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
	cr->arc(x + 40, y + 40, 40, 0, 2 * M_PI);
	cr->fill();
 
	surface.damage(x, y, 80, 80);
	surface.commit();
	
	hovering = false;
	delete hovering_checker;
}

bool BoardView::is_hovering() {
	return hovering;
}
