/*
 * Simulator.cpp
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#include "Simulator.h"
#include "Buffer.h"
#include "Source.h"
#include <iostream>
#include <algorithm>

Simulator::Simulator(size_t buf_size, size_t n) : BUF_CAPACITY(buf_size), N_REQ(n) {}

void Simulator::start(const std::vector<double> &sLambda, const std::vector<double> &pLambda,
							const double tao1, double tao2, bool step_mode) {
	std::vector<Source*> sources;
	std::vector<Processor*> devices;
	OrderedEvents ordered_events;
	size_t ind = 0;
	for (auto lambda : sLambda) {
		sources.push_back(new Source(ind++, lambda));
		ordered_events.push(sources.back()->make_request());
	}
	ind = 0;
	for (auto lambda : pLambda) {
		devices.push_back(new Processor(ind++, tao1, tao2, lambda));
	}
	Buffer *buffer = new Buffer(devices, BUF_CAPACITY, sources.size());

	//if (step_mode) initscr();
	// cycle of event processing
	for (size_t i = 0; i < N_REQ;) {
		Event* pEvent = ordered_events.top();
		ordered_events.pop();
		if (pEvent->is_request()) {
			if (step_mode) {
				std::cout << "Request " << pEvent->get_sid() << "." << pEvent->get_rid()
						<< "    " << pEvent->get_time() << std::endl;
				//getch();
			}
			i++;
			ordered_events.push(sources[pEvent->get_sid()]->make_request());
			Event* pRel = buffer->put_in(pEvent);
			if (pRel) {
				ordered_events.push(pRel);
			}
		}
		else {
			if (step_mode) {
				std::cout << "Device Release " << pEvent->get_time() << std::endl;
				//getch();
			}
			Event* pRel = buffer->put_out(pEvent->get_time());
			if (pRel) {
				ordered_events.push(pRel);
			}
		}
	}
	// process remainded requests in buffer
	while (!buffer->empty()) {
		Event* pEvent = ordered_events.top();
		ordered_events.pop();
		if (!pEvent->is_request()) {
			if (Event* pRel = buffer->put_out(pEvent->get_time())) ordered_events.push(pRel);
		}
	}
	processed = buffer->get_processed();
	rejected = buffer->get_rejected();
	wait_times = buffer->get_wait_times();
	proc_times = buffer->get_proc_times();
	waiting_times = buffer->get_waiting_times();
	working_times = buffer->get_working_times();
}

void Simulator::print_res() {
	std::vector<double> release_time;
	for (decltype(waiting_times.size()) i = 0; i < waiting_times.size(); ++i) {
		release_time.push_back(waiting_times[i] + working_times[i]);
	}
	std::cout << std::endl;
	std::cout << "Source  N req    P_r       T in sys     T in buf       T proc" << std::endl;
	for (decltype(processed.size()) i = 0; i < processed.size(); i++) {
		size_t all = processed[i] + rejected[i];
		double t_all = wait_times[i] + proc_times[i];
		std::cout << i << "       " << all << "    " << (double) rejected[i] / all << "    " << t_all / all
			<< "    " << wait_times[i] / all << "      " << proc_times[i] / all << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Device      K " << std::endl;
	double whole_time = *max_element(release_time.begin(), release_time.end());
	for (decltype(working_times.size()) i = 0; i < working_times.size(); i++) {
		std::cout << i << "       " << working_times[i] / whole_time << std::endl;
	}
}
