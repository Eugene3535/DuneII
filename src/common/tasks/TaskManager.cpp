#include "common/tasks/TaskManager.hpp"

#define TASK_POOL_SIZE 1024

TaskManager::TaskManager() noexcept:
    m_stride(0)
{
    m_memoryPool.resize(TASK_POOL_SIZE);
}


TaskManager::~TaskManager() = default;


void* TaskManager::allocate(uint32_t size) noexcept
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


void TaskManager::release(void* data, uint32_t size) noexcept
{
    auto& freeTaskData = m_freeTaskDataList[size];
    freeTaskData.push_back(data);
}