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
	cr->set_font_size(64);
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
				point < 7 ? x = 1050 - point * 80 : x = 990 - point * 80;
				y = 970;
			}else if(point < 25) {
				point < 19 ? x = 30 + (point - 13) * 80 : x = 90 + (point - 13) * 80;
				y = 30;
			}
			int gravity = point < 13 ? (-80) : 80;
			for(int j {0}; j < std::min(abs(position[point - 1]), 5); ++j){
				cr->arc(x + 40, y + 40, 40, 0, 2 * M_PI);
				cr->fill();
				y += gravity;
			}
			//we need to remove or add 80 to y
			y += point < 13 ? 80 : (-80);
			if(abs(position[point - 1]) > 5) {
				cr->set_source_rgb(1, 1, 1);
				// we add so much to y because of the gravity variable
				cr->move_to(x + 18, y + 64);
				cr->show_text(fmt::format("{}", abs(position[point - 1])));
			}
		}
	}
	surface.damage(0, 0, 1080, 1080);
	surface.commit();
}

void BoardView::checker_pressed(std::vector<int8_t> &position, double cur_x, double cur_y) {
	double x{0};
	double y{0};
	//convert cur_x and cur_y to continuous line of points to detect a press more easly
	if(cur_y < 1040 && cur_y > 640) {
		y = -cur_y + 1040;
		if(cur_x < 1040 && cur_x > 560) {
			x = -cur_x + 1040;
		}else if(cur_x < 500 && cur_x > 20) {
			x = -cur_x + 980;
		}
	}else if(cur_y < 420 && cur_y > 20) {
		y = cur_y - 20;
		if(cur_x < 1040 && cur_x > 560) {
			x = cur_x + 880;
		}else if(cur_x < 500 && cur_x > 20) {
			x = cur_x + 940;
		}
	}
	
	//check checker slot contains a checker
	int point = x / 80 + 1;
	int checker = y / 80 + 1;
	if(abs(position[point - 1]) >= checker) {
		//check distance to center of the checker is less than radius
		double distance = sqrt(pow(point * 80 - 40 - x, 2) + pow(checker * 80 - 40 - y, 2));
		if(distance < 40) {
			bool color = position[point - 1] < 0 ? true : false;	
			//x and y changed to actual coordinates where we need to redraw or remove a checker
			if(point < 13) {
				y = 1050 - checker * 80;
				point < 7 ? x = 1050 - point * 80 : x = 990 - point * 80;
			}else {
				y = checker * 80 - 50;
				point < 19 ? x = (point - 13) * 80 + 30 : x = (point - 13) * 80 + 90;
			}
			//if hovering checker is less than 5 or we have less than 6 checkers on point, just remove it
			if(checker < 5 || abs(position[point - 1]) < 6) {
				for(int i{0}; i < 80; ++i) {
					memcpy((unsigned char *)mem + 4 * width * (i + (int)y) + (int)x * 4, image[i + (int)y] + (int)x * 4, 80 * 4);
				}
			}else {
				//cr->move_to(0, 0);
				color ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
				cr->arc(x + 40, y + 40, 40, 0, 2 * M_PI);
				cr->fill();
				if(abs(position[point - 1]) > 6) {
					cr->set_source_rgb(1, 1, 1);
					cr->move_to(x + 18, y + 64);
					cr->show_text(fmt::format("{}", abs(position[point - 1]) - 1));
				}
			}
			hovering = true;
			hovering_checker = new HoveringChecker(point, checker, color);
			hovering_checker->set_background(mem, 0, 0);
			color ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
			cr->arc(x + 40, y + 40, 40, 0, 2 * M_PI);
			cr->fill();
		}
	}
	surface.damage(0,0,1080,1080);
	surface.commit();

}

void BoardView::checker_hover(double movement_x, double movement_y) {
	double x = hovering_checker->get_x();
	double y = hovering_checker->get_y();
	unsigned char *background = hovering_checker->get_background();
	
	for(int i{0}; i < 80; ++i) {
		memcpy((unsigned char*)mem+4*width*(i+(int)y)+(int)x*4, background+4*80*i,80*4);
	}
	hovering_checker->set_background(mem, movement_x, movement_y);
	x += movement_x;
	y += movement_y;
	hovering_checker->get_color() ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
	cr->arc((int)x + 40, (int)y + 40, 40, 0, 2 * M_PI);
	cr->fill();
	
	surface.damage(0,0,1080,1080);
	surface.commit();
}

void BoardView::checker_off(std::vector<int8_t> &position) {
	double buffer_x = hovering_checker->get_x();
	double buffer_y = hovering_checker->get_y();
	unsigned char *background = hovering_checker->get_background();
	for(int i{0}; i < 80; ++i) {
		memcpy((unsigned char*)mem+4*width*(i+(int)buffer_y)+(int)buffer_x*4, background+4*80*i,80*4);
	}
	
	int point = hovering_checker->get_point();
	int checker = hovering_checker->get_checker();
	int x,y;
	if(point < 13) {
		y = 1050 - checker * 80;
		point < 7 ? x = 1050 - point * 80 : x = 990 - point * 80;
	}else {
		y = checker * 80 - 50;
		point < 19 ? x = (point - 13) * 80 + 30 : x = (point - 13) * 80 + 90;
	}
	hovering_checker->get_color() ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
	cr->arc(x + 40, y + 40, 40, 0, 2 * M_PI);
	cr->fill();
	
	if(abs(position[point - 1]) > 5 && checker == 5) {
		cr->set_source_rgb(1, 1, 1);
		cr->move_to(x + 18, y + 64);
		cr->show_text(fmt::format("{}", abs(position[point - 1])));
	}
		
	surface.damage(0,0,1080,1080);
	surface.commit();
	
	delete hovering_checker;
	hovering_checker = nullptr;
	hovering = false;
}

bool BoardView::is_hovering() {
	return hovering;
}
