#ifndef Algorithms
#define Algorithms
#include <vector>
#include <string>

std::string Tools_Software_Algorithms_getISO8601Time();

template <typename T, std::size_t N>
int Tools_Software_Algorithms_findIndexOf(const T(&arr)[N], const T& value) {
    for (std::size_t i = 0; i < N; i++) {
        if (arr[i] == value) {
            return static_cast<int>(i);
        }
    }
    return -1;  // Not found
}

template <typename T>
int Tools_Software_Algorithms_findIndexOf(const std::vector<T>& arr, const T& value) {
    for (std::size_t i = 0; i < arr.size(); i++) {
        if (arr.at(i).compare(value) == 0) {
            return static_cast<int>(i);
        }
    }
    return -1;  // Not found
}

template <typename T, std::size_t N>
constexpr std::size_t Tools_Software_Algorithms_getArraySize(const T(&)[N]) {
    return N;
}

template <typename T, std::size_t N>
std::vector<T> Tools_Software_Algorithms_arrayToVector(const T(&arr)[N]) {
    return std::vector<T>(arr, arr + N);
}

#endif // !Algorithms
