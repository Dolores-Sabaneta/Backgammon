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
			board_view.checker_pressed(board.get_position(), cur_x, cur_y);
		}else if(button == BTN_LEFT && state == pointer_button_state::released) {
			if(board_view.is_hovering()) board_view.checker_off(board.get_position());
		}
	};
	pointer.on_motion() = [&] (uint32_t, double x, double y) {
		if(x < 1080 && x > 0 && y < 1080 && y > 0) {
			if(board_view.is_hovering()) board_view.checker_hover(x - cur_x, y - cur_y);
			cur_x = x;
			cur_y = y;
		}
	};
}
