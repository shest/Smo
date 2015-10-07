/*
 * Processor.cpp
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#include "Processor.h"

#include <algorithm>

Processor::Processor(const int d_id, const double t1, const double t2, const double d_lambda) :
			id(d_id), tao1(t1), tao2(t2), lambda(d_lambda) { }

const size_t Processor::get_id() const { return id; }

double Processor::get_waiting_time() const { return waiting_time; }

double Processor::get_working_time() const { return working_time; }

double Processor::get_release_time() const { return waiting_time + working_time; }

void Processor::add_waiting(const double time) { waiting_time += time; }

bool Processor::is_working(double event_time) { return event_time < working_time + waiting_time; }

void Processor::process() {
	double rnd = rand() / (double)RAND_MAX;
	working_time += tao1 * (1 - rnd) + tao2 * rnd;  // process event by equal law
}
