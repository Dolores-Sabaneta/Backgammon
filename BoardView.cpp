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
void BoardView::draw() {
	std::vector<int8_t> position = board.get_position();
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

void BoardView::checker_pressed(double cur_x, double cur_y) {
	std::vector<int8_t> position = board.get_position();
	
	//convert cursor positions to point and checker slot number, return if cursor is off slots
	int point{0};
	int checker{0};
	if(cur_y < 1070 && cur_y > 570) {
		checker = (-cur_y + 1070) / 100 + 1;
		if(cur_x < 1730 && cur_x > 1130) {
			point = (-cur_x + 1730) / 100 + 1;
		}else if(cur_x < 970 && cur_x > 370) {
			point = (-cur_x + 1570) / 100 + 1;
		}else return;
	}else if(cur_y < 500 && cur_y > 0) {
		checker = cur_y / 100 + 1;
		if(cur_x < 1730 && cur_x > 1130) {
			point = (cur_x + 670) / 100 + 1;
		}else if(cur_x < 970 && cur_x > 370) {
			point = (cur_x + 830) / 100 + 1;
		}else return;
	}else return;

	//check checker slot contains a checker
	int x{0};
	int y{0};
	fmt::print("point: {}, checker: {}\n", point, checker);
	if(abs(position[point - 1]) >= checker) {
		//x and y are the coordinates where we need to redraw or remove a checker
		if(point < 13) {
			y = 1080 - checker * 100;
			point < 7 ? x = 1740 - point * 100 : x = 1580 - point * 100;
		}else {
			y = checker * 100 - 100;
			point < 19 ? x = (point - 13) * 100 + 380 : x = (point - 13) * 100 + 540;
		}
		//check distance to center of the checker is less than radius
		double distance = sqrt(pow(x + 50 - cur_x, 2) + pow(y + 50 - cur_y, 2));
		if(distance < 50) {
			bool color = position[point - 1] < 0 ? true : false;	
			
			//position less than 6 : we remove checker
			//position is 6:
				//checker is 5 : draw checker on slot 5
				//checker is less than 5 : we remove checker and draw checker on slot 5, then draw number
			//position more than 6 : 
				//checker is 5 : draw checker on slot 5, then draw number
				//checker is less than 5 : we remove checker and draw checker on slot 5, then draw number
	/*
	if(position < 6 || checker < 5) {
		//remove checker
	}
	
	if(position >= 6) {
		//draw checker on slot 5
		if(!(checker == 5 && position == 6) {
			//draw number
		}
	}
	*/
	
	
	
			if(abs(position[point - 1]) < 6 || checker < 5) {
				//remove checker
				for(int i{0}; i < 100; ++i) {
					memcpy((unsigned char*)mem + 4 * width * (i + y) + x * 4, image[i + y] + x * 4, 100 * 4);
				}
			}
			if(abs(position[point - 1]) >= 6) {
				//calculate y for checker 5;
				int temp_y{0};
				if(point < 13) {
					temp_y = 1080 - 5 * 100;
				}else {
					temp_y = 5 * 100 - 100;
				}
				//draw checker on slot 5
				color ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
				cr->arc(x + 50, temp_y + 50, 50, 0, 2 * M_PI);
				cr->fill();
				if(!(checker == 5 && abs(position[point - 1]) == 6)) {
					//draw number
					cr->set_source_rgb(1, 1, 1);
					cr->move_to(x + 18, temp_y + 86);
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

void BoardView::checker_off(double cur_x, double cur_y) {
	std::vector<int8_t> position = board.get_position();
	
	//draw image over last position of hovering checker
	double buffer_x = hovering_checker->get_x();
	double buffer_y = hovering_checker->get_y();
	unsigned char *background = hovering_checker->get_background();
	for(int i{0}; i < 100; ++i) {
		memcpy((unsigned char*)mem+4*width*(i+(int)buffer_y)+(int)buffer_x*4, background+4*100*i,100*4);
	}
	int point = hovering_checker->get_point();
	int checker = hovering_checker->get_checker();
	bool color = hovering_checker->get_color();
	double center_x = hovering_checker->get_x() + 50;
	double center_y = hovering_checker->get_y() + 50;
	delete hovering_checker;
	hovering_checker = nullptr;
	hovering = false;
	
	//convert center_x and center_y to destination point slot
	int destination{0};
	bool move = true;
	if(center_y < 1080 && center_y > 580) {
		if(center_x < 1740 && center_x > 1140) {
			destination = (-center_x + 1740) / 100 + 1;
		}else if(center_x < 980 && center_x > 380) {
			destination = (-center_x + 1580) / 100 + 1;
		}else move = false;
	}else if(center_y < 500 && center_y > 0) {
		if(center_x < 1740 && center_x > 1140) {
			destination = (center_x + 680) / 100 + 1;
		}else if(center_x < 980 && center_x > 380) {
			destination = (center_x + 840) / 100 + 1;
		}else move = false;
	}else move = false;
	
	int x,y;
	// calculate x and y of checker
	if(point < 13) {
		y = 1080 - checker * 100;
		point < 7 ? x = 1740 - point * 100 : x = 1580 - point * 100;
	}else {
		y = checker * 100 - 100;
		point < 19 ? x = (point - 13) * 100 + 380 : x = (point - 13) * 100 + 540;
	}
	
	if(move) {
		try {
			board.move(point, destination);
			if(abs(position[point - 1]) < 6 && checker != abs(position[point - 1])) {
				//calculate temp_y of position
				int temp_y{0};
				if(point < 13) {
					temp_y = 1080 - abs(position[point - 1]) * 100;
				}else {
					temp_y = abs(position[point - 1]) * 100 - 100;
				}
				//draw checker and remove position
				draw_checker(x, y, color);
				remove_checker(x, temp_y);
			}else if(abs(position[point - 1]) == 6 && checker < 5) {
				//calculate temp_y of slot 5
				int temp_y{0};
				if(point < 13) {
					temp_y = 1080 - 5 * 100;
				}else {
					temp_y = 5 * 100 - 100;
				}
				//draw checker and draw checker on slot 5
				draw_checker(x, y, color);
				draw_checker(x, temp_y, color);
			}else if(abs(position[point - 1]) > 6 && checker < 5){
				//draw checker
				draw_checker(x, y, color);
			}
			point = destination;
			checker = std::min(abs(position[point - 1]) + 1, 5);
			//calculate x and y of destination
			if(point < 13) {
				y = 1080 - checker * 100;
				point < 7 ? x = 1740 - point * 100 : x = 1580 - point * 100;
			}else {
				y = checker * 100 - 100;
				point < 19 ? x = (point - 13) * 100 + 380 : x = (point - 13) * 100 + 540;
			}
			draw_checker(x, y, color);
			if(abs(position[point - 1]) + 1 > 5) {
				draw_number(x, y, abs(position[point - 1]) + 1);
			}
		}catch (std::invalid_argument &e) {
			fmt::print("error: {}\n", e.what());
			move = false;
		}
	}
	if(!move) {
		if(abs(position[point - 1]) < 6) {
			//draw checker
			draw_checker(x, y, color);
		}else if(abs(position[point - 1]) == 6) {
			//calculate temp_y of slot 5
			int temp_y{0};
			if(point < 13) {
				temp_y = 1080 - 5 * 100;
			}else {
				temp_y = 5 * 100 - 100;
			}
			if(checker < 5) {
				//draw checker and draw checker on slot 5, then draw number
				draw_checker(x, y, color);
				draw_checker(x, temp_y, color);
				draw_number(x, temp_y, abs(position[point - 1]));
			}else {
				//draw number
				draw_number(x, temp_y, abs(position[point - 1]));
			}
		}else if(abs(position[point - 1]) > 6) {
			//calculate temp_y of slot 5
			int temp_y{0};
			if(point < 13) {
				temp_y = 1080 - 5 * 100;
			}else {
				temp_y = 5 * 100 - 100;
			}
			if(checker < 5) {
				//draw checker and draw checker on slot 5, then draw number
				draw_checker(x, y, color);
				draw_checker(x, temp_y, color);
				draw_number(x, temp_y, abs(position[point - 1]));
			}else {
				//draw checker then draw number
				draw_checker(x, y, color);
				draw_number(x, temp_y, abs(position[point - 1]));
			}
		}
	}
	
	//move failed:
		//position less than 6: draw checker
		//position 6:
			//checker 5: draw number
			//checker less than 5: draw checker and draw checker on slot 5 then draw number
		//position more than 6: 
			//checker 5: draw checker then draw number
			//checker less than 5: draw checker and draw checker on slot 5 then draw number
	//move success
		//position was less than 6:
			//checker is position: do nothing
			//checker is not position: draw checker and remove position
		//position 6: 
			//checker is 5: do nothing
			//checker less than 5: draw checker and draw checker on slot 5
		//position more than 6:
			//checker is 5: do nothing
			//checker less than 5: draw checker
		//destination : draw checker on slot min(destination, 5)
		//destination bigger than 5: draw number
/*
if(move) {
	if(position < 6) {
		if(checker != position) {
			//draw checker and remove position
		}
	}else if(position == 6) {
		if(checker < 5) {
			//draw checker and draw checker on slot 5
		}
	}else if(position > 6) {
		if(checker < 5) {
			//draw checker
		}
	}
}else {
	if(position < 6) {
		//draw checker
	}else if(position == 6) {
		if(checker < 5) {
			//draw checker and draw checker on slot 5, then draw number
		}else {
			//draw number
		}	
	}else if(position > 6) {
		if(checker < 5) {
			//draw checker and draw checker on slot 5, then draw checker
		}else {
			//draw checker, then draw number
		}
	}
}
*/		
	//fmt::print("source: {}, destination: {}\n", hovering_checker->get_point(), destination);	
	surface.damage(0,0,1920,1080);
	surface.commit();
}

void BoardView::draw_checker(int x, int y, bool color) {
	fmt::print("draw_checker, x: {}, y: {}\n", x, y);
	color ? cr->set_source_rgb(1, 0, 0) : cr->set_source_rgb(0, 0, 1);
	cr->arc(x + 50, y + 50, 50, 0, 2 * M_PI);
	cr->fill();
}
void BoardView::draw_number(int x, int y, int checker) {
	cr->set_source_rgb(1, 1, 1);
	cr->move_to(x + 18, y + 86);
	cr->show_text(fmt::format("{}", checker));
}

void BoardView::remove_checker(int x, int y) {
	for(int i{0}; i < 100; ++i) {
		memcpy((unsigned char*)mem + 4 * width * (i + y) + x * 4, image[i + y] + x * 4, 100 * 4);
	}
}

bool BoardView::is_hovering() {
	return hovering;
}
