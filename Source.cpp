/*
 * Source.cpp
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#include <algorithm>
#include "Source.h"

Source::Source(const size_t s_id, const double s_lambda) : id(s_id), lambda(s_lambda) { }

const size_t Source::get_id() const { return id; }

unique_ptr<Event> Source::make_request() {
	++rid;
	get_new_time();
	//cout << "event: " << id << "." << rid << " " << time << endl;
	return std::make_unique<Event>(id, rid, time);
	//return new Event(id, rid, get_new_time());
}

double Source::get_new_time() {
	return time -= log(rand() / (double)RAND_MAX) / lambda; // event generation by simplest law (puasson's)
}
