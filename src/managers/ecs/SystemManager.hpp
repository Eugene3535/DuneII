#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include <vector>
#include <unordered_map>
#include <stddef.h>

#include <SFML/System/NonCopyable.hpp>

#include "managers/ecs/systems/base/System.hpp"

class SystemManager:
    private sf::NonCopyable
{
public:
    SystemManager() noexcept;
    ~SystemManager();

    void initialize() noexcept;

    template<class T>
    T* addSystem(entt::registry& registry) noexcept;

    template<class T>
    T* getSystem() noexcept;

    void updateAllSystems() noexcept;

    void clear() noexcept;

private:
    std::vector<BaseSystem*>                  m_sequentialAccessSystems;
    std::unordered_map<uint32_t, BaseSystem*> m_randomAccessSystems;

    std::vector<std::byte> m_rawMemory;
    size_t                 m_offset;
};

#include "managers/ecs/SystemManager.inl"

#endif // !SYSTEM_MANAGER_HPP