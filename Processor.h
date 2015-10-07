/*
 * Processor.h
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <cstddef>

class Processor {
public:
	Processor() = default;
	Processor(const int d_id, const double t1, const double t2, const double d_lambda);
	const std::size_t get_id() const;
	double get_waiting_time() const;
	double get_working_time() const;
	double get_release_time() const;
	void add_waiting(const double time);
	bool is_working(double event_time);
	void process();
private:
	const std::size_t id = 0;                 // device number
	const double tao1 = 0.0, tao2 = 1.0; // device parameters
	const double lambda = 1.0;           // rate of occurrence
	double waiting_time = 0.0;
	double working_time = 0.0;
};

#endif /* PROCESSOR_H_ */

