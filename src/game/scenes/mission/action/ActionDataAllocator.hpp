#ifndef ACTION_DATA_ALLOCATOR_HPP
#define ACTION_DATA_ALLOCATOR_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>


class ActionDataAllocator
{
public:
    ActionDataAllocator(size_t size) noexcept;
    ~ActionDataAllocator();

    void* allocate(size_t size) noexcept;
    void release(void* actionData, size_t size) noexcept;

private:
    std::vector<uint8_t> m_memoryPool;
    size_t m_stride;

    std::unordered_map<size_t, std::vector<void*>> m_freeActionDataList;
};

#endif // !ACTION_DATA_ALLOCATOR_HPP