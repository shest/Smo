/*
 * Package.cpp
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#include "Package.h"
#include <algorithm>
#include <iostream>

bool Package::set_package(std::vector<Event*> &events) {
	if (!package.empty()) return true;
	auto priority = get_priority(events);
	if (priority == -1) return false;
	for (auto &it : events) {
		if (it && it->get_sid() == priority) {
			package.push_back(&it);
		}
	}
	// ??? check package - should be in descending order
	sort(package.begin(), package.end(), [](Event** r1, Event** r2)
										{return (*r1)->get_rid() < (*r2)->get_rid(); });
	for (auto v : package) {
		std::cout << (*v)->get_sid() << "." << (*v)->get_rid() << std::endl;
	}
	return true;
}

bool Package::empty() const { return package.empty(); }

Event& Package::pop() {
		Event& req = **package.back();
		*package.back() = nullptr;
		package.pop_back();
		return req;
}

void Package::check(const Event* event) {
	for (auto it = package.begin(); it != package.end(); it++) {
		if (**it == event) {
			package.erase(it);
			return;
		}
	}
}

size_t Package::get_priority(const std::vector<Event*> &events) {
	size_t min = -1;
	for (auto it : events) {
		if (it) {
			size_t cur = it->get_sid();
			if (cur == 0) return 0;
			if (cur < min) {
				min = cur;
			}
		}
	}
	return min;
}


