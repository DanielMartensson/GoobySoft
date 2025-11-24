#ifndef Algorithms
#define Algorithms
#include <vector>
#include <cstring>
#include <string>
#include <cstdint>
#include <algorithm>

int Tools_Software_Algorithms_circularShift(const int index, const int shift, const bool reversed, const uint32_t size);

template<typename T> 
bool Tools_Software_Algorithms_circularCopy(const T arraySrc[], T arrayDest[], int startIndexSrc, int startIndexDest, int copyElements, const int arraySizeSrc, const int arraySizeDest) {
    /* Check if the sizes are zero */
    if(arraySizeSrc <= 0 || arraySizeDest <= 0){
        return false;
    }
   
    /* -----------------------------------------------------
       Internal lambda: normalize an index into [0, size)
       ----------------------------------------------------- 
    auto normalizeIndex = [](int index, int size) {
        return (index % size + size) % size;
    };*/

    /* -----------------------------------------------------
       Internal lambda: optimized bulk copy (memcpy or std::copy)
       ----------------------------------------------------- */
    auto copyBlock = [](const T* src, T* dst, int count) {
        if constexpr (std::is_trivially_copyable<T>::value) {
            std::memcpy(dst, src, count * sizeof(T));
        } else {
            std::copy(src, src + count, dst);
        }
    };

    /* -----------------------------------------------------
       Step 1: Normalize indices
       ----------------------------------------------------- */
    startIndexSrc  = Tools_Software_Algorithms_circularShift(startIndexSrc, 0, false, arraySizeSrc); //normalizeIndex(startIndexSrc,  arraySizeSrc);
    startIndexDest = Tools_Software_Algorithms_circularShift(startIndexDest, 0, false, arraySizeDest); //normalizeIndex(startIndexDest, arraySizeDest);

    // Avoid overflow on destination
    if (copyElements > arraySizeDest) {
        copyElements = arraySizeDest;
    }

    if (copyElements <= 0) {
        return false;
    }

    /* -----------------------------------------------------
       Step 2: Determine available space to end of buffers
       ----------------------------------------------------- */
    int srcToEnd  = arraySizeSrc  - startIndexSrc;
    int destToEnd = arraySizeDest - startIndexDest;

    // First contiguous block
    int block1 = std::min({copyElements, srcToEnd, destToEnd});

    /* -----------------------------------------------------
       Step 3: Copy block #1
       ----------------------------------------------------- */
    copyBlock(arraySrc + startIndexSrc, arrayDest + startIndexDest, block1);

    int remaining = copyElements - block1;
    if (remaining <= 0) {
        return true;
    }

    /* -----------------------------------------------------
       Step 4: Compute new positions after block1
       ----------------------------------------------------- */
    int srcIndex2  = (startIndexSrc  + block1) % arraySizeSrc;
    int destIndex2 = (startIndexDest + block1) % arraySizeDest;

    int srcToEnd2  = arraySizeSrc  - srcIndex2;
    int destToEnd2 = arraySizeDest - destIndex2;

    int block2 = std::min({remaining, srcToEnd2, destToEnd2});

    /* -----------------------------------------------------
       Step 5: Copy block #2
       ----------------------------------------------------- */
    copyBlock(arraySrc + srcIndex2, arrayDest + destIndex2, block2);

    remaining -= block2;
    if (remaining <= 0) {
        return true;
    }

    /* -----------------------------------------------------
       Step 6: Final wrap block (#3)
       ----------------------------------------------------- */
    int srcIndex3  = (srcIndex2  + block2) % arraySizeSrc;
    int destIndex3 = (destIndex2 + block2) % arraySizeDest;

    copyBlock(arraySrc + srcIndex3, arrayDest + destIndex3, remaining);

    /* It's working */
    return true;
}

std::string Tools_Software_Algorithms_getISO8601Time();
long long Tools_Software_Algorithms_getMicroSeconds();
long long Tools_Software_Algorithms_getMilliSeconds();
void Tools_Software_Algorithms_extractElementFromCharArray(const char arraySeparatedByZeros[], int indexNumber, char extracted[]);
float Tools_Software_Algorithms_calibration(float x1, float x2, float y1, float y2, float x);
void Tools_Software_Algorithms_goobySleep(const uint32_t milliseconds);

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
