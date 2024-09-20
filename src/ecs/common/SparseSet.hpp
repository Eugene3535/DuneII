#ifndef SPARSE_SET_HPP
#define SPARSE_SET_HPP

#include <vector>

#include "ecs/common/ECSTypes.hpp"

BEGIN_NAMESPACE_ECS

template<class Id, class T>
class SparseSet
{
    static constexpr uint32_t Undefined = UINT32_MAX;

public:
    template<class ...Args>
    std::pair<Id, T&>     emplace(Args&& ...args) noexcept;
    void                  erase(Id id)            noexcept;
    bool                  has(Id id)        const noexcept;
    std::vector<T>&       getObjects()            noexcept;
    const std::vector<T>& getObjects()      const noexcept;
    size_t                getSize()         const noexcept;
    void                  reserve(size_t size)    noexcept;

    T&                   operator [](Id id)       noexcept;
    const T&             operator [](Id id) const noexcept;

private:
    std::vector<size_t> m_idToIndex;
    std::vector<Id>     m_indexToId;
    std::vector<Id>     m_freeIds;
    std::vector<T>      m_objects;
};

#include "ecs/common/SparseSet.inl"

END_NAMESPACE_ECS

#endif // !SPARSE_SET_HPP