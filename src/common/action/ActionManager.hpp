#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>


class ActionManager
{
public:
    ActionManager(size_t size) noexcept;
    ~ActionManager();

    void* allocate(size_t size) noexcept;
    void release(void* actionData, size_t size) noexcept;

private:
    std::vector<uint8_t> m_memoryPool;
    size_t m_stride;

    std::unordered_map<size_t, std::vector<void*>> m_freeActionDataList;
};