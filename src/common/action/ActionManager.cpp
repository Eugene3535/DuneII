#include <cassert>

#include "common/action/ActionManager.hpp"


ActionManager::ActionManager(size_t size) noexcept:
    m_stride(0)
{
    assert(size > 0);
    m_memoryPool.resize(size);
}


ActionManager::~ActionManager() = default;


void* ActionManager::allocate(size_t size) noexcept
{
    auto& freeActionData = m_freeActionDataList[size];

    if(!freeActionData.empty())
    {
        void* data = freeActionData.back();
        freeActionData.pop_back();

        return data;
    }

    if (m_stride < m_memoryPool.size())
    {
        const size_t offset = m_stride;
        m_stride += size;

        return static_cast<void*>(m_memoryPool.data() + offset);
    }

    return nullptr;
}


void ActionManager::release(void* data, size_t size) noexcept
{
    auto& freeActionData = m_freeActionDataList[size];
    freeActionData.push_back(data);
}