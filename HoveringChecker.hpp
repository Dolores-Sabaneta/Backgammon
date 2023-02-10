#pragma once

class HoveringChecker{
private:
	shm_pool_t pool;
	buffer_t buffer;
	surface_t surface;
	SharedMemory memory;
	double x, y;
public:
	HoveringCHecker();
};
