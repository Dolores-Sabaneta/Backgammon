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

	//retrieve image size
	int size = width * height * 4;
	int stride = width * 4;
	image = new png_bytep[height];
	for(int i = 0; i < height; i++) {
		image[i] = new png_byte[stride];
	}
	//store png data in image
	png_read_image(png_ptr, image);
	
}

void BoardView::init(surface_t &surface, void *mem){
	//create a cairo context
	
	int stride = width * 4;
	this->surface = surface;
	this->mem = mem;
	Cairo::RefPtr<Cairo::ImageSurface> imageSurface = Cairo::ImageSurface::create((unsigned char *)mem, Cairo::FORMAT_ARGB32, width, height, stride);
	cr = Cairo::Context::create(imageSurface);
	//load font library
	FT_Library library;
	FT_Face face;
	FT_Init_FreeType(&library);
	FT_New_Face(library, "Roboto-Black.ttf", 0, &face);
	FT_Set_Pixel_Sizes(face, 0, 32);
	cr->select_font_face("Roboto-Black", Cairo::FONT_SLANT_NORMAL,
	Cairo::FONT_WEIGHT_BOLD);
	cr->set_font_size(96);
}
void BoardView::draw(std::vector<int8_t> &position) {
	int stride = width * 4;
	//draw the board image
	for(int i = 0; i < height; i++) {
		memcpy(static_cast<uint32_t*>(mem) + i * width, image[i] , stride);
	}
	//convert point and checker number from position to drawing coordinates x, y
	
	int x {0};
	int y {0};
	//access all the points that contain checkers
	for(int point{1}; point < 25; point++) {
		if(position[point - 1] != 0) {
			position[point - 1] < 0 ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
			//(x,y) is the top corner of the first checker in a point		
			if(point < 13) {		
				point < 7 ? x = 1740 - point * 100 : x = 1580 - point * 100;
				y = 980;
			}else if(point < 25) {
				point < 19 ? x = 380 + (point - 13) * 100 : x = 540 + (point - 13) * 100;
				y = 0;
			}
			int gravity = point < 13 ? (-100) : 100;
			for(int j {0}; j < std::min(abs(position[point - 1]), 5); ++j){
				fmt::print("x: {}, y: {}\n", x, y);
				cr->arc(x + 50, y + 50, 50, 0, 2 * M_PI);
				cr->fill();
				y += gravity;
			}
			//we need to remove or add 80 to y because of the previous loop
			y += point < 13 ? 100 : (-100);
			if(abs(position[point - 1]) > 5) {
				cr->set_source_rgb(1, 1, 1);
				// we add so much to y because of the gravity variable
				cr->move_to(x + 18, y + 86);
				cr->show_text(fmt::format("{}", abs(position[point - 1])));
			}
		}
	}
	surface.damage(0, 0, 1920, 1920);
	surface.commit();
}

void BoardView::checker_pressed(std::vector<int8_t> &position, double cur_x, double cur_y) {
	double x{0};
	double y{0};
	//convert cur_x and cur_y to continuous line of points to detect a press more easly
	if(cur_y < 1070 && cur_y > 570) {
		y = -cur_y + 1070;
		if(cur_x < 1730 && cur_x > 1130) {
			x = -cur_x + 1730;
		}else if(cur_x < 970 && cur_x > 370) {
			x = -cur_x + 1570;
		}
	}else if(cur_y < 500 && cur_y > 0) {
		y = cur_y;
		if(cur_x < 1730 && cur_x > 1130) {
			x = cur_x + 670;
		}else if(cur_x < 970 && cur_x > 370) {
			x = cur_x + 830;
		}
	}
	fmt::print("x: {}, y: {}\n", x, y);
	
	//check checker slot contains a checker
	int point = x / 100 + 1;
	int checker = y / 100 + 1;
	if(abs(position[point - 1]) >= checker) {
		//check distance to center of the checker is less than radius
		double distance = sqrt(pow(point * 100 - 50 - x, 2) + pow(checker * 100 - 50 - y, 2));
		if(distance < 50) {
			bool color = position[point - 1] < 0 ? true : false;	
			//x and y changed to actual coordinates where we need to redraw or remove a checker
			if(point < 13) {
				y = 1080 - checker * 100;
				point < 7 ? x = 1740 - point * 100 : x = 1580 - point * 100;
			}else {
				y = checker * 100 - 100;
				point < 19 ? x = (point - 13) * 100 + 380 : x = (point - 13) * 100 + 540;
			}
			//if hovering checker is less than 5 or we have less than 6 checkers on point, just remove it
			if(checker < 5 || abs(position[point - 1]) < 6) {
				for(int i{0}; i < 100; ++i) {
					memcpy((unsigned char *)mem + 4 * width * (i + (int)y) + (int)x * 4, image[i + (int)y] + (int)x * 4, 100 * 4);
				}
			}else {
				color ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
				cr->arc(x + 50, y + 50, 50, 0, 2 * M_PI);
				cr->fill();
				if(abs(position[point - 1]) > 6) {
					cr->set_source_rgb(1, 1, 1);
					cr->move_to(x + 18, y + 86);
					cr->show_text(fmt::format("{}", abs(position[point - 1]) - 1));
				}
			}
			
			hovering = true;
			hovering_checker = new HoveringChecker(point, checker, color, cur_x - x, cur_y - y);
			hovering_checker->set_background(mem, cur_x, cur_y);
			color ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
			cr->arc(x + 50, y + 50, 50, 0, 2 * M_PI);
			cr->fill();
			
		}
	}
	surface.damage(0,0,1920,1080);
	surface.commit();
}

void BoardView::checker_hover(double cur_x, double cur_y) {
	fmt::print("is drawable: {}\n", hovering_checker->is_drawable(cur_x, cur_y));
	if(hovering_checker->is_drawable(cur_x, cur_y)) {	
		double x = hovering_checker->get_x();
		double y = hovering_checker->get_y();
		unsigned char *background = hovering_checker->get_background();
		
		for(int i{0}; i < 100; ++i) {
			memcpy((unsigned char*)mem+4*width*(i+(int)y)+(int)x*4, background+4*100*i,100*4);
		}
		hovering_checker->set_background(mem, cur_x, cur_y);
		x = hovering_checker->get_x();
		y = hovering_checker->get_y();
		hovering_checker->get_color() ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
		cr->arc((int)x + 50, (int)y + 50, 50, 0, 2 * M_PI);
		cr->fill();
	
		surface.damage(0,0,1920,1080);
		surface.commit();
	}
}

void BoardView::checker_off(std::vector<int8_t> &position) {
	double buffer_x = hovering_checker->get_x();
	double buffer_y = hovering_checker->get_y();
	unsigned char *background = hovering_checker->get_background();
	for(int i{0}; i < 100; ++i) {
		memcpy((unsigned char*)mem+4*width*(i+(int)buffer_y)+(int)buffer_x*4, background+4*100*i,100*4);
	}
	
	int point = hovering_checker->get_point();
	int checker = hovering_checker->get_checker();
	int x,y;
	if(point < 13) {
		y = 1080 - checker * 100;
		point < 7 ? x = 1740 - point * 100 : x = 1580 - point * 100;
	}else {
		y = checker * 100 - 100;
		point < 19 ? x = (point - 13) * 100 + 380 : x = (point - 13) * 100 + 540;
	}
	hovering_checker->get_color() ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
	cr->arc(x + 50, y + 50, 50, 0, 2 * M_PI);
	cr->fill();
	
	if(abs(position[point - 1]) > 5 && checker == 5) {
		cr->set_source_rgb(1, 1, 1);
		cr->move_to(x + 18, y + 86);
		cr->show_text(fmt::format("{}", abs(position[point - 1])));
	}
		
	surface.damage(0,0,1920,1080);
	surface.commit();
	
	delete hovering_checker;
	hovering_checker = nullptr;
	hovering = false;
}

bool BoardView::is_hovering() {
	return hovering;
}
