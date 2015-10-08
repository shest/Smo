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
#include <memory>

using std::unique_ptr;
using std::vector;
using PEvent = unique_ptr<Event>;
using PPEvent = unique_ptr<Event>* ;

class Package {
public:
	bool set_package(vector<PEvent> &events);
	bool empty() const;
	PEvent pop();
	void check(PEvent &event);
private:
	vector<PPEvent> package;
	size_t get_priority(vector<PEvent> &events);
};

#endif /* PACKAGE_H_ */
