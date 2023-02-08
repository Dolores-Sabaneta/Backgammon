#include "Client.hpp"

void Client::OnEvent() {
	pointer.on_enter() = [&] (uint32_t serial, const surface_t& /*unused*/, int32_t /*unused*/, int32_t /*unused*/) {
		fmt::print("enter\n");
		cursor_surface.attach(cursor_buffer, 0, 0);
		cursor_surface.damage(0, 0, cursor_image.width(), cursor_image.height());
		cursor_surface.commit();
		pointer.set_cursor(serial, cursor_surface, 0, 0);
	};
	
	pointer.on_button() = [&] (uint32_t serial, uint32_t /*unused*/, uint32_t button, pointer_button_state state)
    {
      if(button == BTN_LEFT && state == pointer_button_state::pressed)
      {
        fmt::print("pressed\n");
        mouse_left = 1; 
      }
      if(button == BTN_LEFT && state == pointer_button_state::released)
      {
        fmt::print("released\n");
        mouse_left = 0;
        board.draw_position(surface, mem);
      }
    };
    
    pointer.on_motion() = [&] (uint32_t, double x, double y) {
    	fmt::print("x: {}, y: {}\n", x, y);
    	cursor_x = x;
    	cursor_y = y;
    	
    	for(int i = 0; i < 24; i++) {
    		int number = board.get_position()[i];
    		if(number != 0) {
    			if(i < 6) {
    				for(int j = 0; j < abs(number); j++) {
    				/*
    					if(x < 1040 - i * 80 && x > 960 - i * 80 && y < 1040 - j * 80 && y > 960 - j * 80) {
    				fmt::print("point: {}, checker: {}\n", i + 1, j + 1);
    					}
    					*/
    					if(sqrt(pow(1000 - i * 80 - x, 2) + pow(1000 - j * 80 - y, 2)) <= 40) {
    						fmt::print("point: {}, checker: {}\n", i + 1, j + 1);
    					}
    				}
    			}else if(i < 12) {
    			
    			}else if(i < 18) {
    			
    			}else if(i < 24) {
    			
    			}
    		}
    	}
    };
}


