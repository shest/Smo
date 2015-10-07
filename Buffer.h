/*
 * Buffer.h
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <vector>
#include "Event.h"
#include "Processor.h"
#include "Package.h"

using ReqBuffer = std::vector<Event*>;
using RingDevBuffer = std::vector<Processor*>;

class Buffer {
public:
	Buffer() = default;
	Buffer(RingDevBuffer &dev, size_t size, size_t src_cnt);
	Event* put_in(Event* event);
	Event* put_out(const double time);
	bool empty();
	const std::vector<std::size_t>& get_rejected() const;
	const std::vector<std::size_t>& get_processed() const;
	const std::vector<double>& get_wait_times() const;
	const std::vector<double>& get_proc_times() const;
	const std::vector<double>& get_working_times();
	const std::vector<double>& get_waiting_times();
private:
	RingDevBuffer devices;
	RingDevBuffer::iterator free_dev = devices.begin();
	ReqBuffer events;
	Package package;
	std::vector<std::size_t> rej_cnt;
	std::vector<std::size_t> proc_cnt;
	std::vector<double> wait_time;
	std::vector<double> proc_time;
	std::vector<double> waiting_dev_times;
	std::vector<double> working_dev_times;
	bool has_free_dev(const double event_time);
	void next_dev();
	Event* put_on_process(double time, const Event* event);
	bool has_free_space(Event* event);
	void reject(Event* event);
};

#endif /* BUFFER_H_ */
