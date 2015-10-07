/*
 * Event.cpp
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#include "Event.h"

Event::Event(double _time) : status(DEV_RELEASE), time(_time) {}

Event::Event(size_t _dev_id, size_t _id, double _time) : status(REQUEST), dev_id(_dev_id),
		id(_id), time(_time) {}

bool Event::is_request() const { return status == REQUEST; }

const double Event::get_time() const { return time; }

const std::size_t Event::get_sid() const { return dev_id; }

const std::size_t Event::get_rid() const { return id; }
