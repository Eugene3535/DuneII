#include <cassert>

#include "game/scenes/mission/action/ActionDataAllocator.hpp"


ActionDataAllocator::ActionDataAllocator(size_t size) noexcept:
    m_stride(0)
{
    assert(size > 0);
    m_memoryPool.resize(size);
}


ActionDataAllocator::~ActionDataAllocator() = default;


void* ActionDataAllocator::allocate(size_t size) noexcept
{
    auto& freeActionData = m_freeActionDataList[size];

    if(!freeActionData.empty())
    {
        void* p = freeActionData.back();
        freeActionData.pop_back();

        return p;
    }

    if (m_stride < m_memoryPool.size())
    {
        const size_t offset = m_stride;
        m_stride += size;

        return static_cast<void*>(m_memoryPool.data() + offset);
    }

    return nullptr;
}


void ActionDataAllocator::release(void* p, size_t size) noexcept
{
    auto& freeActionData = m_freeActionDataList[size];
    freeActionData.push_back(p);
}