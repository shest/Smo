/*
 * Event.h
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <cstddef>
enum eventType { REQUEST, DEV_RELEASE, NONE };

class Event {
public:
	Event() = default;
	Event(double _time);
	size_t foo();
	Event(size_t _dev_id, size_t _id, double _time);
	bool is_request() const;
	const double get_time() const;
	const size_t get_sid() const;
	const size_t get_rid() const;
private:
	eventType status = NONE;    // type of event (request or proc. release)
	const double time = 0.0;    // time of event occurrence
	const size_t dev_id = -1;   // device id
	const size_t id = -1;       // request number
};

#endif /* EVENT_H_ */
