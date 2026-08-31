#include "common/tasks/TaskDataHolder.hpp"

#define TASK_DATA_POOL_SIZE 1024


TaskDataHolder::TaskDataHolder() noexcept:
    m_stride(0)
{
    m_memoryPool.resize(TASK_DATA_POOL_SIZE);
}


TaskDataHolder::~TaskDataHolder() = default;


void* TaskDataHolder::allocate(uint32_t size) noexcept
{
    auto& freeTaskData = m_freeTaskDataList[size];

    if (!freeTaskData.empty())
    {
        void* data = freeTaskData.back();
        freeTaskData.pop_back();

        return data;
    }

    if (m_stride < m_memoryPool.size())
    {
        const uint32_t offset = m_stride;
        m_stride += size;

        return static_cast<void*>(m_memoryPool.data() + offset);
    }

    return nullptr;
}


void TaskDataHolder::release(void* data, uint32_t size) noexcept
{
    if (size < 2) // ignoring cases with 0 and 1
        return;

    auto& freeTaskData = m_freeTaskDataList[size];
    freeTaskData.push_back(data);
}