#include "Algorithms.h"
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/chrono.hpp>
#include <chrono>
//#include <cstring>
#include <thread>

int Tools_Software_Algorithms_circularShift(const int index, const int shift, const bool backward, const uint32_t size){
    return backward ? ((index - shift) % size + size) % size : ((index + shift) % size + size) % size;
}

std::string Tools_Software_Algorithms_getISO8601Time() {
	boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
	std::string time = to_iso_extended_string(t);
	return time;
}

long long Tools_Software_Algorithms_getMicroSeconds() {
    // Get the current time from the system clock
    auto now = std::chrono::system_clock::now();

    // Convert the current time to time since epoch
    auto duration = now.time_since_epoch();

    // Convert to microseconds
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return microseconds;

    //auto now = boost::chrono::high_resolution_clock::now();
    //return boost::chrono::duration_cast<boost::chrono::microseconds>(now.time_since_epoch()).count();
}

long long Tools_Software_Algorithms_getMilliSeconds() {
    // Get the current time from the system clock
    auto now = std::chrono::system_clock::now();

    // Convert the current time to time since epoch
    auto duration = now.time_since_epoch();

    // Convert to milliseconds
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return milliseconds;
    //auto now = boost::chrono::high_resolution_clock::now();
    //return boost::chrono::duration_cast<boost::chrono::milliseconds>(now.time_since_epoch()).count();
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

void Tools_Software_Algorithms_goobySleep(const uint32_t milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}