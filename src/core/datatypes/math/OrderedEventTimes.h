/*
 * EventTime.h
 *
 *  Created on: Apr 9, 2020
 *      Author: mike
 */

#ifndef ORDEREDEVENTTIME_H_
#define ORDEREDEVENTTIME_H_

#include <set>

#include "Cloneable.h"

namespace RevBayesCore {

	class OrderedEventTimes : public Cloneable {

	public:

		                    OrderedEventTimes();
		virtual            ~OrderedEventTimes();

		OrderedEventTimes*  clone(void) const;

        bool                      operator==(const OrderedEventTimes &oet) const;
        bool                      operator!=(const OrderedEventTimes &oet) const;
        bool                      operator<(const  OrderedEventTimes &oet) const;
        bool                      operator<=(const OrderedEventTimes &oet) const { return operator<(oet) || operator==(oet); }

		// getters and setters
		bool                      addEvent(double time);
		bool                      removeEvent(double time);
		bool                      changeEventTime(double old_time, double new_time);
		const std::set<double>&   getEventTimes() const;
		size_t                    size() const;

	private:

		std::set<double>  event_times;

	};

    // Global functions using the class
    std::ostream&           operator<<(std::ostream& o, const OrderedEventTimes& x);                                         //!< Overloaded output operator


} /* namespace RevBayesCore */

#endif /* CORE_DATATYPES_MATH_ORDEREDEVENTTIME_H_ */
