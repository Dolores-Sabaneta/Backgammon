
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fmt/core.h>

using namespace std;

int main() {

	vector<int> a {1,2,3};
	
	a[0]++;
	fmt::print("result: {}", a[0]);
	return 0;
}
