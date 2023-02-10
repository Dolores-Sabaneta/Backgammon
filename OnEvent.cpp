#include "Client.hpp"

void Client::OnEvent() {
	pointer.on_enter() = [&] (uint32_t serial, const surface_t& /*unused*/, int32_t /*unused*/, int32_t /*unused*/) {
		fmt::print("enter\n");
		cursor_surface.attach(cursor_buffer, 0, 0);
		cursor_surface.damage(0, 0, cursor_image.width(), cursor_image.height());
		cursor_surface.commit();
		pointer.set_cursor(serial, cursor_surface, 0, 0);
	};
	pointer.on_button() = [&] (uint32_t serial, uint32_t /*unused*/, uint32_t button, pointer_button_state state) {      
		if(button == BTN_LEFT && state == pointer_button_state::pressed) {
			fmt::print("pressed\n");
			double x, y;
			if(cur_y < 1040 && cur_y > 640){
				fmt::print("down\n");
				cur_x <  
				
			}else if(cur_y < 420 && cur_y > 20){
				fmt::print("up\n");
				y = abs(cur_y);
			}
		}else if(button == BTN_LEFT && state == pointer_button_state::released) {
			fmt::print("released\n");
		}
	};
	pointer.on_motion() = [&] (uint32_t, double x, double y) {
		cur_x = x;
		cur_y = y;
	};
}
