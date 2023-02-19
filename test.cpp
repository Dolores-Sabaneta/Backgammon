//#include <wayland-client-protocol.hpp>
//#include <wayland-client.hpp>
//#include <wayland-client-protocol-extra.hpp>
//#include <cairomm/surface.h>
#include <vector>

class Ball {

public:
	std::vector<int> &get_position() {
		return position;
	}
private:
	std::vector<int> position{1,2,3};
};

int main() {
	Ball ball;

	return 0;
}
