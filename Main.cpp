
//#include "smo.h"

#include "Simulator.h"
#include <algorithm>
#include <memory>

int main() {
	auto worker = std::make_unique<Simulator>();
	srand((unsigned)time(NULL));
	std::vector<double> sl = {3, 3, 3};
	std::vector<double> dl = {3, 3};
	worker->start(sl, dl, 0.1, 0.5, 1);
	worker->print_res();
	return 0;
}
