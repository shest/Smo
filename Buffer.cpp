/*
 * Buffer.cpp
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#include "Buffer.h"
#include <limits>

Buffer::Buffer(RingDevBuffer &dev, std::size_t size, std::size_t src_cnt) :
	devices(dev), wait_time(src_cnt), proc_time(src_cnt),
	proc_cnt(src_cnt), rej_cnt(src_cnt), waiting_dev_times(dev.size()), working_dev_times(dev.size()),
	events(size, nullptr) {}

Event* Buffer::put_in(Event* event) {
	double time = event->get_time();
	std::size_t sid = event->get_sid();
	if (has_free_dev(time)) {
		++proc_cnt[sid];
		// check if device not being working before event
		Processor* dev = *free_dev;
		dev->add_waiting(time - dev->get_release_time());
		dev->process();
		// calculate process time for source
		double rtime = dev->get_release_time();
		proc_time[sid] += rtime - time;
		//cout << "on dev: " << sid << "." << event->get_rid() << " gt " << time << " rt" << rtime;
		//cout << endl;
		next_dev();
		return new Event(rtime);
	}
	else if (!has_free_space(event)) {
		++rej_cnt[sid];
		reject(event);
	}
	return nullptr;
}

Event* Buffer::put_out(const double time) {
	if (package.set_package(events)) {
		return put_on_process(time, &package.pop());
	}
	return nullptr;
}

bool Buffer::empty() {
	for (auto it : events) {
		if (it) return false;
	}
	return true;
}

const std::vector<std::size_t>& Buffer::get_rejected() const { return rej_cnt; }

const std::vector<std::size_t>& Buffer::get_processed() const { return proc_cnt; }

const std::vector<double>& Buffer::get_wait_times() const { return wait_time; }

const std::vector<double>& Buffer::get_proc_times() const { return proc_time; }

const std::vector<double>& Buffer::get_working_times() {
	for (auto &i : working_dev_times) {
		i = devices[i]->get_working_time();
	}
	return working_dev_times;
}

const std::vector<double>& Buffer::get_waiting_times() {
	for (auto &i : waiting_dev_times) {
		i = devices[i]->get_waiting_time();
	}
	return waiting_dev_times;
}

bool Buffer::has_free_dev(const double event_time) {
	for (std::size_t i = 0; i < devices.size(); ++i, next_dev()) {
		if (!(*free_dev)->is_working(event_time)) {
			return true;
		}
	}
	return false;
}

void Buffer::next_dev() {
	if (++free_dev == devices.end()) free_dev = devices.begin();
}

Event* Buffer::put_on_process(double time, const Event* event) {
	//find device which has been done at this time according to ring
	double rel_time;
	for (decltype(devices.size()) i = 0; i < devices.size(); i++, next_dev()) {
		if ((*free_dev)->get_release_time() == time) {
			std::size_t sid = event->get_sid();
			++proc_cnt[sid];
			// calculate wait time for source of this event
			wait_time[sid] += time - event->get_time();
			// calculate process time for this event
			(*free_dev)->process();
			rel_time = (*free_dev)->get_release_time();
			proc_time[sid] += rel_time - time;
			//cout << "from buffer: " << sid << "." << event->get_rid() << " " << "in dev: " << time << "end proc " << rel_time;
			//cout << endl;
			break;
		}
	}
	next_dev();
	return new Event(rel_time);
}

// statement in buffer - on free space from beginning
bool Buffer::has_free_space(Event* event) {
	for (auto &it : events) {
		if (!it) {
			it = event;
			return true;
		}
	}
	return false;
}

// request rejection - the oldest event
void Buffer::reject(Event* event) {
	double min_time = std::numeric_limits<double>::max();
	Event** min_it = nullptr;
	for (auto &it : events) {
		if (it) {
			double time = it->get_time();
			if (time < min_time) {
				min_time = time;
				min_it = &it;
			}
		}
	}
	wait_time[(*min_it)->get_sid()] += event->get_time() - (*min_it)->get_time();
	package.check(*min_it);
	*min_it = event;
//	Event * min = std::min_element(events.begin(), events.end(), [](Event * x, Event * y) { return x->get_time() < y->get_time(); });
//	package.check(*min);
//	min = event;
}
