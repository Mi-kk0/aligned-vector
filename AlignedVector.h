#ifndef ALIGNEDVECTOR_ALIGNEDVECTOR_H
#define ALIGNEDVECTOR_ALIGNEDVECTOR_H
#include <cstddef>
#include <utility>
#include <new>

template <typename T, std::size_t Align = 64>
class AlignedVector {
    static_assert(Align != 0 && (Align & (Align-1)) == 0,
        "Align must be power of 2");
    static_assert(Align >= alignof(T),
        "Align must be at least alignof(T)");
    T* pStart;
    T* pEnd;
    T* pCapacity;

    static T* allocate(std::size_t count) {
        return static_cast<T*>(::operator new (count * sizeof(T), static_cast<std::align_val_t>(Align)));
    }
    static void deallocate(T* p) noexcept {
        ::operator delete (p, static_cast<std::align_val_t>(Align));
    }
    void grow() {
        const std::size_t cap = capacity() == 0 ? 1 : capacity() * 2;

        T* newVecStart = allocate(cap);
        std::size_t index = 0;
        try {
            for (; index < this->size(); index++) {
                new (newVecStart + index) T(std::move_if_noexcept(pStart[index]));
            }
        }
        catch (...) {
            for (std::size_t i = 0; i < index; i++)
            {
                newVecStart[i].~T();
            }
            deallocate(newVecStart);
            throw;
        }
        for (std::size_t i = 0; i < index; i++) {
            pStart[i].~T();
        }
        deallocate(pStart);
        pStart = newVecStart;
        pEnd = pStart + index;
        pCapacity = pStart + cap;


    };

public:
    explicit AlignedVector(std::size_t initial_size) {
        pStart = allocate(initial_size);
        pEnd = pStart;
        pCapacity = pStart + initial_size;
    }
    ~AlignedVector() {
        for (std::size_t i = 0; i < this->size(); i++) {
            pStart[i].~T();
        }
        deallocate(pStart);
    }
    AlignedVector(AlignedVector&& other)  noexcept {
        this->pStart = other.pStart;
        this->pEnd = other.pEnd;
        this->pCapacity = other.pCapacity;

        other.pStart = nullptr;
        other.pEnd = nullptr;
        other.pCapacity = nullptr;
    }
    AlignedVector& operator=(const AlignedVector&) = delete;
    AlignedVector& operator=(AlignedVector&& other)  noexcept {
        if (this == &other) {
            return *this;
        }
        for (std::size_t i = 0; i < this->size(); i++) {
            pStart[i].~T();
        }
        deallocate(pStart);
        this->pStart = other.pStart;
        this->pEnd = other.pEnd;
        this->pCapacity = other.pCapacity;

        other.pStart = nullptr;
        other.pEnd = nullptr;
        other.pCapacity = nullptr;

        return *this;
    }

    std::size_t size() const {
        return pEnd - pStart;
    }
    std::size_t capacity() const {
        return pCapacity - pStart;
    }


    void push_back(T data) {
        if (this->size() == this->capacity()) {
            grow();
        }
        new (pEnd) T(std::move(data));
        ++pEnd;
    }

    T* data()const {
        return pStart;
    }

    T& operator[](std::size_t index) const {
        return pStart[index];
    }

    T&& operator[](std::size_t index) {
        return pStart[index];
    }

};


#endif
