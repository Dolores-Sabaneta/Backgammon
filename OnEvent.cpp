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
			mouse_left = 1;

			if(cursor_x > 20 && cursor_x < 500) {
				//check pointer is left up
				if(cursor_y > 20 && cursor_y < 420) {
					int point = (int)((cursor_x - 20) / 80 + 13);
					int checker = (int)((cursor_y - 20) / 80 + 1);
					if(abs(board.get_position()[point - 1]) >= checker) {
						double distance_x = 60 + 80 * (point - 13) - cursor_x;
						double distance_y = 60 + 80 * (checker - 1) - cursor_y;
						double distance_total = sqrt(pow(distance_x, 2) + pow(distance_y, 2));
						if(distance_total < 40) {
							fmt::print("cursor_x: {}, cursor_y: {}, point: {}, checker: {}, distance: {}\n", cursor_x, cursor_y, point, checker, distance_total);
							hover = 1;
							last_hover_position_x = 20 + 80 * (point - 13);
							last_hover_position_y = 20 + 80 * (checker - 1);
						} 
					}					
				//check pointer is left down
				}else if(cursor_y > 640 && cursor_y < 1040) {
					int point = (int)((20 - cursor_x) / 80 + 13);
					int checker = (int)((640 - cursor_y) / 80 + 6);
					if(abs(board.get_position()[point - 1]) >= checker) {
						double distance_x = 60 + 80 * (-point + 12) - cursor_x;
						double distance_y = 680 + 80 * (-checker + 5) - cursor_y;
						double distance_total = sqrt(pow(distance_x, 2) + pow(distance_y, 2));
						if(distance_total < 40) {
							fmt::print("cursor_x: {}, cursor_y: {}, point: {}, checker: {}, distance: {}\n", cursor_x, cursor_y, point, checker, distance_total);
							hover = 1;
						} 
					}
				}
			//check pointer is right
			}else if(cursor_x > 560 && cursor_x < 1040) {
				//check pointer is right up
				if(cursor_y > 20 && cursor_y < 420) {
					int point = (int)((cursor_x - 560) / 80 + 19);
					int checker = (int)((cursor_y - 20) / 80 + 1);
						if(abs(board.get_position()[point - 1]) >= checker) {
							double distance_x = 600 + 80 * (point - 19) - cursor_x;
							double distance_y = 60 + 80 * (checker - 1) - cursor_y;
							double distance_total = sqrt(pow(distance_x, 2) + pow(distance_y, 2));
							if(distance_total < 40) {
								fmt::print("cursor_x: {}, cursor_y: {}, point: {}, checker: {}, distance: {}\n", cursor_x, cursor_y, point, checker, distance_total);
								hover = 1;
							} 
						}
				//check pointer is right down
				}else if(cursor_y > 640 && cursor_y < 1040) {
					int point = (int)((560 - cursor_x) / 80 + 7);
					int checker = (int)((640 - cursor_y) / 80 + 6);
						if(abs(board.get_position()[point - 1]) >= checker) {
							double distance_x = 600 + 80 * (-point + 6) - cursor_x;
							double distance_y = 680 + 80 * (-checker + 5) - cursor_y;
							double distance_total = sqrt(pow(distance_x, 2) + pow(distance_y, 2));
							if(distance_total < 40) {
								fmt::print("cursor_x: {}, cursor_y: {}, point: {}, checker: {}, distance: {}\n", cursor_x, cursor_y, point, checker, distance_total);
								hover = 1;
							} 
						}
				}
			}			
		}else if(button == BTN_LEFT && state == pointer_button_state::released) {
			fmt::print("released\n");
			board.draw_position(surface, mem);
			mouse_left = 0;
			hover = 0;
		}
	};
    
	pointer.on_motion() = [&] (uint32_t, double x, double y) {
		if(hover) {
			double checker_x = last_hover_position_x + x - cursor_x;
			double checker_y = last_hover_position_y + y - cursor_y;
			fmt::print("last_x: {}, last_y: {}\n", last_hover_position_x, last_hover_position_y);
			board.hover_checker(checker_x, checker_y, last_hover_position_x, last_hover_position_y, surface, mem);
			last_hover_position_x = last_hover_position_x + x - cursor_x;
			last_hover_position_y = last_hover_position_y + y - cursor_y;    		
		}    	
		cursor_x = x;
		cursor_y = y;
    	/*
    	if(!mouse_left) {
			//check pointer is left
			if(x > 20 && x < 500) {
				//check pointer is left up
				if(y > 20 && y < 420) {
					int point = (int)((x - 20) / 80 + 13);
					int checker = (int)((y - 20) / 80 + 1);
					if(abs(board.get_position()[point - 1]) >= checker) {
						if(sqrt(pow(60+80*(point-13)-x,2) + pow(60+80*(checker-1)-y,2)) < 40) {
							fmt::print("x: {}, y: {}, point: {}, checker: {}, distance: {}\n", x, y, point, checker, sqrt(pow(60 + 80 * (point - 13) - x, 2) + pow(60 + 80 * (checker - 1) - y, 2)));
						//board.hover_checker(,, last_hover_x, last_hover_y, surface, mem);
						} 
					}					
				//check pointer is left down
				}else if(y > 640 && y < 1040) {
					int point = (int)((20 - x) / 80 + 13);
					int checker = (int)((640 - y) / 80 + 6);
					if(abs(board.get_position()[point - 1]) >= checker) {
						if(sqrt(pow(60+80*(-point+12)-x,2) + pow(680+80*(-checker+5)-y,2)) < 40) {
							fmt::print("x: {}, y: {}, point: {}, checker: {}, distance: {}\n", x, y, point, checker, sqrt(pow(60+80*(-point + 12)-x,2) + pow(680+80*(-checker + 5)-y,2)));
						} 
					}
				}
			//check pointer is right
			}else if(x > 560 && x < 1040) {
				//check pointer is right up
				if(y > 20 && y < 420) {
					int point = (int)((x - 560) / 80 + 19);
					int checker = (int)((y - 20) / 80 + 1);
						if(abs(board.get_position()[point - 1]) >= checker) {
							if(sqrt(pow(600+80*(point-19)-x,2) + pow(60+80*(checker-1)-y,2)) < 40) {
								fmt::print("x: {}, y: {}, point: {}, checker: {}, distance: {}\n", x, y, point, checker, sqrt(pow(600 + 80 * (point - 19) - x, 2) + pow(60 + 80 * (checker - 1) - y, 2)));
							} 
						}
				//check pointer is right down
				}else if(y > 640 && y < 1040) {
					int point = (int)((560 - x) / 80 + 7);
					int checker = (int)((640 - y) / 80 + 6);
						if(abs(board.get_position()[point - 1]) >= checker) {
							if(sqrt(pow(600+80*(-point+6)-x,2) + pow(680+80*(-checker+5)-y,2)) < 40) {
								fmt::print("x: {}, y: {}, point: {}, checker: {}, distance: {}\n", x, y, point, checker, sqrt(pow(600+80*(-point + 6)-x,2) + pow(680+80*(-checker + 5)-y,2)));
							} 
						}
				}
			}
			
			//last_hover_x = ;
			//last_hover_y = ;
		}
		*/
	};
}


