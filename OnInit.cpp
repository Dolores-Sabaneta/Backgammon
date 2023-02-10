#include "Client.hpp"

void Client::OnInit() {
	registry = display.get_registry();
	registry.on_global() = [&] (uint32_t name, const std::string &interface, uint32_t version) {
		//fmt::print("name: {}, interface: {}, version: {}\n", name, interface, version);
		if(interface == compositor_t::interface_name)
			registry.bind(name, compositor, version);
		else if(interface == xdg_wm_base_t::interface_name)
			registry.bind(name, xdg_wm_base, version);
		else if(interface == shm_t::interface_name)
			registry.bind(name, shm, version);
		else if(interface == seat_t::interface_name)
			registry.bind(name, seat, version);
	};
	display.roundtrip();
	
	surface = compositor.create_surface();
	
	xdg_wm_base.on_ping() = [&] (uint32_t serial) {xdg_wm_base.pong(serial);};
	xdg_surface = xdg_wm_base.get_xdg_surface(surface);
	xdg_surface.on_configure() = [&] (uint32_t serial) {xdg_surface.ack_configure(serial);};
	xdg_toplevel  = xdg_surface.get_toplevel();
	//xdg_toplevel.set_fullscreen(output);
	xdg_toplevel.on_close() = [&] () {running = false;};
	surface.commit();
	display.roundtrip();
  		
	int stride = width * 4;
	int size = stride * height;
	
	pool = shm.create_pool(memory.get_fd(), width * height * 4);
	buffer = pool.create_buffer(0, width, height, stride, shm_format::argb8888);
	surface.attach(buffer, 0, 0);
	
	pointer = seat.get_pointer();
	cursor_theme_t cursor_theme = cursor_theme_t("default", 16, shm);
	cursor_t cursor = cursor_theme.get_cursor("cross");
	cursor_image = cursor.image(0);
	cursor_buffer = cursor_image.get_buffer();
	cursor_surface = compositor.create_surface(); 
	boardview.draw(board.get_position(), surface, memory.get_mem());
}
