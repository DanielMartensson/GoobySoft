#ifndef Algorithms
#define Algorithms
#include <vector>
#include <string>

std::string Tools_Software_Algorithms_getISO8601Time();
int_least64_t Tools_Software_Algorithms_getMicroSeconds();
int_least64_t Tools_Software_Algorithms_getMilliSeconds();
void Tools_Software_Algorithms_extractElementFromCharArray(const char arraySeparatedByZeros[], int indexNumber, char extracted[]);
float Tools_Software_Algorithms_calibration(float x1, float x2, float y1, float y2, float x);

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

template <typename T, int Size>
void Tools_Software_Algorithms_rotateOneStepBack(T(&arr)[Size], int numElementsToRotate) {
    T temp = arr[0];
    for (int i = 0; i <= numElementsToRotate - 1; i++) {
        arr[i] = arr[i + 1];
    }
    arr[numElementsToRotate - 1] = temp;
}

template<typename T, int Size>
void Tools_Software_Algorithms_shiftArrayElements(T(&arr)[Size], int beginIndex) {
    if (beginIndex < 0 || beginIndex >= Size) {
        return;
    }
    T elementToMove = arr[beginIndex];
    for (int i = beginIndex; i < Size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    arr[Size - 1] = elementToMove;
}

#endif // !Algorithms
