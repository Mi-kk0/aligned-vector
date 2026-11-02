#ifndef ALIGNEDVECTOR_ALIGNEDVECTOR_H
#define ALIGNEDVECTOR_ALIGNEDVECTOR_H
#include <cstdint>

template <typename T>
class AlignedVector {
    T* pStart;
    T* pEnd;
    T* pCapacity;
public:
    explicit AlignedVector(size_t initial_size) {
        pStart = new T[initial_size];
        pEnd = pStart;
        pCapacity = pStart + sizeof(T) * initial_size;
    }
};


#endif
