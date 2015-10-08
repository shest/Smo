/*
 * Buffer.cpp
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#include "Buffer.h"
#include <algorithm>


Buffer::Buffer(vector<std::unique_ptr<Processor>> &dev, const size_t size, const size_t src_cnt) :
	wait_time(src_cnt), proc_time(src_cnt), proc_cnt(src_cnt), rej_cnt(src_cnt),
	waiting_dev_times(dev.size()), working_dev_times(dev.size()), events(size) {
	size_t ind = 0;
	for (auto &d : dev) {
			devices.emplace_back(d.release());
	}
	free_dev = devices.begin();
}

unique_ptr<Event> Buffer::put_in(PEvent &event) {
	double time = event->get_time();
	size_t sid = event->get_sid();
	if (has_free_dev(time)) {
		++proc_cnt[sid];
		// check if device has not being working before event
		free_dev->get()->add_waiting(time - free_dev->get()->get_release_time());
		free_dev->get()->process();
		// calculate process time for source
		double rtime = free_dev->get()->get_release_time();
		proc_time[sid] += rtime - time;
		//cout << "on dev: " << sid << "." << event->get_rid() << " gt " << time << " rt" << rtime;
		//cout << endl;
		next_dev();
		return std::make_unique<Event>(rtime);
	}
	else if (!has_free_space(event)) {
		++rej_cnt[sid];
		reject(event);
	}
	return nullptr;
}

unique_ptr<Event> Buffer::put_out(const double time) {
	if (package.set_package(events)) {
		PEvent event = package.pop();
		return put_on_process(time, event);
	}
	return nullptr;
}

bool Buffer::empty() {
	for (auto &it : events) {
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
		if (!free_dev->get()->is_working(event_time)) {
			return true;
		}
	}
	return false;
}

void Buffer::next_dev() {
	if (++free_dev == devices.end()) free_dev = devices.begin();
}

unique_ptr<Event> Buffer::put_on_process(double time, PEvent &event) {
	//find device which has been done at this time according to ring
	double rel_time;
	for (decltype(devices.size()) i = 0; i < devices.size(); i++, next_dev()) {
		if (free_dev->get()->get_release_time() == time) {
			size_t sid = event->get_sid();
			++proc_cnt[sid];
			// calculate wait time for source of this event
			wait_time[sid] += time - event->get_time();
			// calculate process time for this event
			(*free_dev)->process();
			rel_time = free_dev->get()->get_release_time();
			proc_time[sid] += rel_time - time;
			//cout << "from buffer: " << sid << "." << event->get_rid() << " " << "in dev: " << time << "end proc " << rel_time;
			//cout << endl;
			break;
		}
	}
	next_dev();
	return std::make_unique<Event>(rel_time);
	//return new Event(rel_time);
}

// statement to buffer - to free space from beginning
bool Buffer::has_free_space(PEvent &event) {
	for (auto &it : events) {
		if (!it) {
			it.reset(event.release());
			return true;
		}
	}
	return false;
}

// request rejection - the oldest event
void Buffer::reject(PEvent &event) {
	auto min_it = std::min_element(events.begin(), events.end(), [](PEvent &lp, PEvent &rp) -> bool {
		if (lp && rp) return lp->get_time() < rp->get_time();
		if (lp && !rp) return true;
		return false;
	} );
	wait_time[(*min_it)->get_sid()] += event->get_time() - (*min_it)->get_time();
	package.check(*min_it);
	min_it->reset(event.release());
}
