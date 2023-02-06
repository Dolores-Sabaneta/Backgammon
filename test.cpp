//#include <wayland-client-protocol.hpp>
//#include <wayland-client.hpp>
#include <wayland-client-protocol-extra.hpp>

using namespace wayland;
int main() {
	display_t display;
	registry_t registry;
	compositor_t compositor;
	xdg_wm_base_t xdg_wm_base;
	shm_t shm;
	
	registry = display.get_registry();
	
	registry.on_global() = [&] (uint32_t name, const std::string &interface, uint32_t version) {
		if(interface == compositor_t::interface_name)
			registry.bind(name, compositor, version);
		else if(interface == shm_t::interface_name)
			registry.bind(name, shm, version);
	};
	return 0;
}
