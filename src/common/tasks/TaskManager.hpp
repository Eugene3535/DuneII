#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>


class TaskManager
{
public:
    TaskManager() noexcept;
    ~TaskManager();

    void* allocate(uint32_t size) noexcept;
    void release(void* taskData, uint32_t size) noexcept;

private:
    std::vector<uint8_t> m_memoryPool;
    size_t m_stride;

    std::unordered_map<uint32_t, std::vector<void*>> m_freeTaskDataList;
};