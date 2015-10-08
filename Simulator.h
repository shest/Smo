/*
 * Simulator.h
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <vector>
#include "Event.h"

using std::vector;

class Simulator {
public:
	Simulator() = default;
	Simulator(size_t buf_size, size_t n);
	void start(const std::vector<double> &sLambda, const std::vector<double> &pLambda,
							const double tao1, double tao2, bool step_mode);
	void print_res();
private:
	const size_t BUF_CAPACITY = 2;
	const size_t N_REQ = 100;
	vector<size_t> processed;
	vector<size_t> rejected;
	vector<double> wait_times;
	vector<double> proc_times;
	vector<double> waiting_times;
	vector<double> working_times;
};

#endif /* SIMULATOR_H_ */
