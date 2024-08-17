#pragma once
#include <assert.h>

template <typename t>
class utl_vector {
public:
    constexpr t& operator[](int i) noexcept { return memory[i]; }
    constexpr const t& operator[](int i) const noexcept { return memory[i]; }

    t* memory;
    int allocation_count;
    int growSize;
    int size;
    t* elements;
};