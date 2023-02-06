#include "Client.hpp"

void Client::OnInit() {
	//registry = display.get_registry();
	registry.on_global() = [&] (uint32_t name, const std::string &interface, uint32_t version) {
		fmt::print("name: {}, interface: {}, version: {}\n", name, interface, version);
		if(interface == compositor_t::interface_name)
			registry.bind(name, compositor, version);
		else if(interface == xdg_wm_base_t::interface_name)
			registry.bind(name, xdg_wm_base, version);
		else if(interface == shm_t::interface_name)
			registry.bind(name, shm, version);
	};
	display.roundtrip();
	surface = compositor.create_surface();
	
	if(xdg_wm_base) {
		xdg_wm_base.on_ping() = [&] (uint32_t serial) {xdg_wm_base.pong(serial);};
		xdg_surface = xdg_wm_base.get_xdg_surface(surface);
		xdg_surface.on_configure() = [&] (uint32_t serial) {xdg_surface.ack_configure(serial);};
		xdg_toplevel  = xdg_surface.get_toplevel();
		xdg_toplevel.set_title("window");
		xdg_toplevel.on_close() = [&] () {running = false;};
	}
		surface.commit();
		display.roundtrip();
		
		
	int stride = width * 4;
	int size = stride * height;
	
	std::stringstream ss;
	std::random_device device;
	std::default_random_engine engine(device());
	std::uniform_int_distribution<unsigned int> distribution(0, std::numeric_limits<unsigned int>::max());
	ss << distribution(engine);
	std::string name = ss.str();
	
	fd = memfd_create(name.c_str(), 0);
	if(fd < 0) throw std::runtime_error("shm_open failed.");
	if(ftruncate(fd, width * height * 4) < 0) throw std::runtime_error("ftruncate failed.");
	mem = mmap(nullptr, width * height * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(mem == MAP_FAILED) throw std::runtime_error("mmap failed.");
	
	
	shm_pool_t pool = shm.create_pool(fd, size);
	buffer = pool.create_buffer(0, width, height, stride, shm_format::argb8888);
	
}
