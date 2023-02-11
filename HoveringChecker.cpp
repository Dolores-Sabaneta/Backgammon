#include "HoveringChecker.hpp"

HoveringChecker::HoveringChecker(shm_t &shm, surface_t &surface) : surface{surface}, memory(80 * 80 * 4){
	pool = shm.create_pool(memory.get_fd(), 80 * 80 * 4);
	buffer = pool.create_buffer(0, 80, 80, 80 * 4, shm_format::argb8888);
	fmt::print("c\n");
	
}

surface_t &HoveringChecker::get_hovering_surface() {
	return surface;
}

HoveringChecker::~HoveringChecker() {

}
