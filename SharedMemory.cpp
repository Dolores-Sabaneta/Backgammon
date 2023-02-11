#include "SharedMemory.hpp"

int SharedMemory::get_fd() {
	return fd;
}
void *SharedMemory::get_mem() {
	return mem;
}
SharedMemory::SharedMemory(int size) : size{size} {
	std::stringstream ss;
	std::random_device device;
	std::default_random_engine engine(device());
	std::uniform_int_distribution<unsigned int> distribution(0, std::numeric_limits<unsigned int>::max());
	ss << distribution(engine);
	name = ss.str();
	fd = memfd_create(name.c_str(), 0);
	
	fmt::print("memory name: {}, fd: {}\n", name, fd);
	if(fd < 0) throw std::runtime_error("shm_open failed.");
	if(ftruncate(fd, size) < 0) throw std::runtime_error("ftruncate failed.");
	mem = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(mem == MAP_FAILED) throw std::runtime_error("mmap failed.");
}

SharedMemory::~SharedMemory() {
	delete mem;
}
