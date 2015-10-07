/*
 * Source.h
 *
 *  Created on: 6 окт. 2015 г.
 *      Author: shest
 */

#ifndef SOURCE_H_
#define SOURCE_H_

#include "Event.h"

class Source {
public:
	Source() = default;
	Source(const size_t s_id, const double s_lambda);
	const size_t get_id() const;
	Event* make_request();
private:
	const size_t id = 0;       // source number
	size_t rid = -1;           // request number
	const double lambda = 1.0; // rate of occurrence
	double time = 0.0;         // time of event generation
	double get_new_time();
};

#endif /* SOURCE_H_ */
