/*
 * Package.h
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#ifndef PACKAGE_H_
#define PACKAGE_H_

#include "Event.h"
#include <vector>

class Package {
public:
	bool set_package(std::vector<Event*> &events);
	bool empty() const;
	Event& pop();
	void check(const Event* event);
private:
	std::vector<Event**> package;
	size_t get_priority(const std::vector<Event*> &events);
};

#endif /* PACKAGE_H_ */
