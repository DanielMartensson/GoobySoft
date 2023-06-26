#include "Algorithms.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstring>

std::string Tools_Software_Algorithms_getISO8601Time() {
	boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
	std::string time = to_iso_extended_string(t);
	return time;
}
