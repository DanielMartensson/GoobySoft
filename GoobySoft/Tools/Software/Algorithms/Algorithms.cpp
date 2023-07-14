#include "Algorithms.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstring>

std::string Tools_Software_Algorithms_getISO8601Time() {
	boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
	std::string time = to_iso_extended_string(t);
	return time;
}

void Tools_Software_Algorithms_extractElementFromCharArray(const char arraySeparatedByZeros[], int indexNumber, char extracted[]) {
    int items_count = 0;
    while (*arraySeparatedByZeros) {
        if (items_count == indexNumber) {
            std::sprintf(extracted, "%s", arraySeparatedByZeros);
            break;
        }
        arraySeparatedByZeros += std::strlen(arraySeparatedByZeros) + 1;
        items_count++;
    }
}

float Tools_Software_Algorithms_calibration(float x1, float x2, float y1, float y2, float x) {
    float deltaX = x2 - x1;
    if (deltaX == 0.0f) {
        deltaX = 1.0f;
    }
    float deltaY = y2 - y1;
    float k = deltaY / deltaX;
    float m = y2 - k * x2;
    return k * x + m;
}