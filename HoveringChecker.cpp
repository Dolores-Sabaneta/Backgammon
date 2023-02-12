#include "HoveringChecker.hpp"

HoveringChecker::HoveringChecker(shm_t &shm, surface_t &surface, subsurface_t &subsurface, int point, int checker, bool color) : surface{surface}, subsurface{subsurface}, point{point}, checker{checker}, color{color}, memory(80 * 80 * 4){
	pool = shm.create_pool(memory.get_fd(), 80 * 80 * 4);
	buffer = pool.create_buffer(0, 80, 80, 80 * 4, shm_format::argb8888);
	surface.attach(buffer, 0, 0);
	subsurface.set_desync();
	//memset(memory.get_mem(), 0xFFFFFFFF, 80 * 80 * 4);
	//surface.damage(0, 0, 80, 80);
	//surface.commit();
}

surface_t &HoveringChecker::get_surface() {
	return surface;
}
subsurface_t &HoveringChecker::get_subsurface() {
	return subsurface;
}

void *HoveringChecker::get_mem() {
	return memory.get_mem();
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
double HoveringChecker::get_x(){
	return x;
}
double HoveringChecker::get_y() {
	return y;
}
void HoveringChecker::set_x(double x) {
	this->x = x;
}
void HoveringChecker::set_y(double y) {
	this->y = y;
}



HoveringChecker::~HoveringChecker() {

}
