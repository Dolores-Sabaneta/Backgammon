#include "HoveringChecker.hpp"

HoveringChecker::HoveringChecker(int point, int checker, bool color, double offset_x, double offset_y) : point{point}, checker{checker}, color{color}, offset_x{offset_x}, offset_y{offset_y} {
	background = new unsigned char[100 * 100 * 4];
	if(point < 13) {
		y = 1080 - checker * 100;
		point < 7 ? x = 1740 - point * 100 : x = 1580 - point * 100;
	}else {
		y = checker * 100 - 100;
		point < 19 ? x = (point - 13) * 100 + 380 : x = (point - 13) * 100 + 540;
	}
	//fmt::print("x_offset: {}, y_offset: {}, x: {}, y: {}\n", offset_x, offset_y,x,y);
}
void HoveringChecker::set_background(void *mem, double cur_x, double cur_y) {
	double limit_x = cur_x - offset_x;
	double limit_y = cur_y - offset_y;
	if(limit_x < 1820 && limit_x > 0){
		x = limit_x;
	}else limit_x <= 0 ? x = 0 : x = 1820;
	if(limit_y < 980 && limit_y > 0) {
		y = limit_y;
	}else limit_y <= 0 ? y = 0 : y = 980;
	
	fmt::print("x: {}, y: {}, cur_x: {}, cur_y: {}, offset_x: {}, offset_y: {}\n", x, y, cur_x, cur_y, offset_x, offset_y);
	for(int i{0}; i < 100; ++i) {
		memcpy(background + i * 100 * 4, (unsigned char *)mem + 4 * 1920 * (i + (int)y) + (int)x * 4, 100 * 4);
	}
}

unsigned char *HoveringChecker::get_background() {
	return background;
}
int HoveringChecker::get_point() {
	return point;
}
int HoveringChecker::get_checker() {
	return checker;
}
bool HoveringChecker::get_color() {
	return color;
}
double HoveringChecker::get_x() {
	return x;
}
double HoveringChecker::get_y() {
	return y;
}
double HoveringChecker::get_x_offset() {
	return offset_x;
}
double HoveringChecker::get_y_offset() {
	return offset_y;
}
bool HoveringChecker::is_drawable(double cur_x, double cur_y) {
	double limit_x = cur_x - offset_x;
	double limit_y = cur_y - offset_y;
	//check if at least one axis of the checker top corner falls inside the window
	if((limit_x < 1820 && limit_x > 0) || (limit_y < 980 && limit_y > 0)) {
		return true;
	}else return false;
}
HoveringChecker::~HoveringChecker() {
	delete[] background;
}
