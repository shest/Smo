/*
 * Simulator.h
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

//#include <cstddef>
#include <vector>
#include <queue>
#include "Event.h"

class Simulator {
public:
	Simulator() = default;
	Simulator(size_t buf_size, size_t n);
	void start(const std::vector<double> &sLambda, const std::vector<double> &pLambda,
							const double tao1, double tao2, bool step_mode);
	void print_res();
private:
	struct GreaterThanByTime {
			bool operator()(const Event* req1, const Event* req2) const {
				return req1->get_time() > req2->get_time();
			}
		};
	using OrderedEvents = std::priority_queue<Event*, std::vector<Event*>, GreaterThanByTime>;
	const size_t BUF_CAPACITY = 2;
	const size_t N_REQ = 100;
	std::vector<size_t> processed;
	std::vector<size_t> rejected;
	std::vector<double> wait_times;
	std::vector<double> proc_times;
	std::vector<double> waiting_times;
	std::vector<double> working_times;
};

#endif /* SIMULATOR_H_ */
