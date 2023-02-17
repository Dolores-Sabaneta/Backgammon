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
			board_view.checker_pressed(cur_x, cur_y);
		}else if(button == BTN_LEFT && state == pointer_button_state::released) {
			if(board_view.is_hovering()) board_view.checker_off(cur_x, cur_y);
		}
	};
	pointer.on_motion() = [&] (uint32_t, double x, double y) {
			if(board_view.is_hovering()) board_view.checker_hover(x, y);
			cur_x = x;
			cur_y = y;
	};
	keyboard.on_key() = [&] (uint32_t , uint32_t , uint32_t key, keyboard_key_state state) {
		if(key == KEY_Q && state == keyboard_key_state::pressed) {
			running = false;
		}else if(key == KEY_F && state == keyboard_key_state::pressed) {
			xdg_toplevel.set_fullscreen(output);
		}else if(key == KEY_D && state == keyboard_key_state::pressed) {
			xdg_toplevel.unset_fullscreen();
		}
	
	};
}
