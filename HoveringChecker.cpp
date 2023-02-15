#include "HoveringChecker.hpp"

HoveringChecker::HoveringChecker(int point, int checker, bool color) : point{point}, checker{checker}, color{color} {
	background = new unsigned char[80 * 80 * 4];
	
	if(point < 13) {
		y = 1050 - checker * 80;
		point < 7 ? x = 1050 - point * 80 : x = 990 - point * 80;
	}else {
		y = checker * 80 - 50;
		point < 19 ? x = (point - 13) * 80 + 30 : x = (point - 13) * 80 + 90;
	}
}
void HoveringChecker::set_background(void *mem, double movement_x, double movement_y) {
	x += movement_x;
	y += movement_y;
	fmt::print("x: {:.2f}, y: {:.2f}, intx: {}, inty: {}\n",x,y,(int)x, (int)y);
	for(int i{0}; i < 80; ++i) {
		memcpy(background + i * 80 * 4, (unsigned char *)mem + 4 * 1080 * (i + (int)y) + (int)x * 4, 80 * 4);
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
HoveringChecker::~HoveringChecker() {
	delete[] background;
}
