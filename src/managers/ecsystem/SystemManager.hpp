#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include <vector>
#include <unordered_map>
#include <cstddef>

#include "managers/ecs/common/System.hpp"
#include "managers/ecs/EntityManager.hpp"

class SystemManager
{
public:
    SystemManager() noexcept;
    ~SystemManager();

    bool initialize(EntityManager& entityManager) noexcept;

    template<class T>
    T* addSystem(EntityManager& entityManager) noexcept;

    template<class T>
    T* getSystem() noexcept;

    void updateAllSystems() noexcept;

private:
    std::vector<BaseSystem*>                  m_sequentialAccessSystems;
    std::unordered_map<uint32_t, BaseSystem*> m_randomAccessSystems;

    std::vector<std::byte> m_rawMemory;
    size_t                 m_offset;
};

#include "managers/ecsystem/SystemManager.inl"

#endif // !SYSTEM_MANAGER_HPP