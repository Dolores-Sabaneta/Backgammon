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
			double x = 0, y = 0; //converted coordinates
			bool in = false;
			//We convert coordinates to a line of points going from 1 to 24
			if(cur_y < 1040 && cur_y > 640){
				fmt::print("down\n");
				y = -cur_y + 1040;
				if(cur_x < 500 && cur_x > 20) {
					in = true;
					x = -cur_x + 980;
				}else if(cur_x < 1040 && cur_x > 560) {
					in = true;
					x = -cur_x + 1040;
				}
			}else if(cur_y < 420 && cur_y > 20){
				fmt::print("up\n");
					y = cur_y - 20;
				if(cur_x < 500 && cur_x > 20) {
					in = true;
					x = cur_x + 940;
				}else if(cur_x < 1040 && cur_x > 560) {
					in = true;
					x = cur_x + 880;
				}
			}
			if(in) {
				int point = x / 80 + 1;
				int checker = y / 80 + 1;
				bool color;
				board.get_position()[point - 1] < 0 ? color = 0 : color = 1;
				double distance = sqrt(pow(point * 80 - 40 - x, 2) + pow(checker * 80 - 40 - y, 2));
				//we click on the circular shape of a checker
				if(distance < 40 && abs(board.get_position()[point - 1]) >= checker) {
					fmt::print("checker: {}, point: {}, distance: {}\n", checker, point, distance);
					surface_t hovering_surface = compositor.create_surface();
					subsurface_t hovering_subsurface = subcompositor.get_subsurface(hovering_surface, surface);
					board_view.start_hover(board.get_position(), surface, memory.get_mem(), point, checker, color, shm, hovering_surface, hovering_subsurface);
					
				}
			}
		}else if(button == BTN_LEFT && state == pointer_button_state::released) {
			fmt::print("released\n");
			if(board_view.is_hovering()) {
				fmt::print("hovering: {}\n", board_view.is_hovering());
				board_view.stop_hover(surface, memory.get_mem());
			}
		}
	};
	pointer.on_motion() = [&] (uint32_t, double x, double y) {
		if(board_view.is_hovering()) {
			board_view.hover(x - cur_x, y - cur_y, surface);
		}
		cur_x = x;
		cur_y = y;
	};
}
