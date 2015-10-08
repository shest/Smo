/*
 * Buffer.h
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "Event.h"
#include "Processor.h"
#include "Package.h"

using ReqBuffer = vector<PEvent>;
using PProcessor = unique_ptr<Processor>;
using RingDevBuffer = vector<PProcessor>;
using std::unique_ptr;

class Buffer {
public:
	Buffer() = default;
	Buffer(vector<std::unique_ptr<Processor>> &dev, const size_t size, const size_t src_cnt);
	unique_ptr<Event> put_in(PEvent &event);
	unique_ptr<Event> put_out(const double time);
	bool empty();
	const vector<std::size_t>& get_rejected() const;
	const vector<std::size_t>& get_processed() const;
	const vector<double>& get_wait_times() const;
	const vector<double>& get_proc_times() const;
	const vector<double>& get_working_times();
	const vector<double>& get_waiting_times();
private:
	RingDevBuffer devices;
	RingDevBuffer::iterator free_dev;
	ReqBuffer events;
	Package package;
	vector<std::size_t> rej_cnt;
	vector<std::size_t> proc_cnt;
	vector<double> wait_time;
	vector<double> proc_time;
	vector<double> waiting_dev_times;
	vector<double> working_dev_times;
	bool has_free_dev(const double event_time);
	void next_dev();
	PEvent put_on_process(double time, PEvent &event);
	bool has_free_space(PEvent &event);
	void reject(PEvent &event);
};

#endif /* BUFFER_H_ */
