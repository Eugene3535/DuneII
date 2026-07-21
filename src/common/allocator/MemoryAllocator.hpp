#pragma once

#include <cstdint>
#include <array>
#include <unordered_map>


template<size_t N>
class MemoryAllocator
{
public:
    MemoryAllocator() noexcept;
    ~MemoryAllocator();

    template<class T, class... Args>
    T* allocate(Args&&... args) noexcept;

    template<class T>
    void release(T* data) noexcept;

    void release(void* data, size_t size) noexcept;

private:
    std::array<uint8_t, N> m_memoryPool;
    size_t m_stride;

    std::unordered_map<size_t, std::vector<void*>> m_freeBlocks;
};


#include "common/MemoryAllocator.inl"