/*
 * Package.cpp
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#include "Package.h"
#include <algorithm>
#include <iostream>

bool Package::set_package(vector<PEvent> &events) {
	if (!package.empty()) return true;
	auto priority = get_priority(events);
	if (priority == -1) return false;
	for (auto &it : events) {
		if (it && it->get_sid() == priority) {
			package.emplace_back(&it);
		}
	}
	sort(package.begin(), package.end(), [](const PPEvent &r1, const PPEvent &r2)
										{return (*r1)->get_rid() > (*r2)->get_rid(); });
//	for (auto v : package) {
//		std::cout << (*v)->get_sid() << "." << (*v)->get_rid() << std::endl;
//	}
	return true;
}

bool Package::empty() const { return package.empty(); }

PEvent Package::pop() {
		auto event = package.back()->release();
		package.pop_back();
		return std::make_unique<Event>(*event);
}

void Package::check(PEvent &event) {
	for (auto it = package.begin(); it != package.end(); it++) {
		if (**it == event) {
			package.erase(it);
			return;
		}
	}
}

size_t Package::get_priority(vector<PEvent> &events) {
	auto min_it = std::min_element(events.begin(), events.end(), [](PEvent &lp, PEvent &rp) -> bool {
		if (lp && rp) return lp->get_sid() < rp->get_sid();
		if (lp && !rp) return true;
		return false;
	} );
	if (*min_it) return min_it->get()->get_sid();
	return -1;
}


