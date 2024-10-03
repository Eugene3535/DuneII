#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include <vector>
#include <unordered_map>
#include <cstddef>
#include <new>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include "ecs/systems/base/System.hpp"

class SystemManager:
    private sf::NonCopyable
{
    static constexpr size_t max_memory_size = 1024 << 3; // 8 kB

public:
    SystemManager() noexcept;
    ~SystemManager();

    void update(sf::Time dt) noexcept;
    void clear() noexcept;

    template<class T, class... Args>
    T* addSystem(entt::registry& registry, Args&& ... args) noexcept;

    template<class T>
    T* getSystem() noexcept;

private:
    std::vector<BaseSystem*>                  m_sequentialAccessSystems;
    std::unordered_map<uint32_t, BaseSystem*> m_randomAccessSystems;

    std::vector<std::byte> m_rawMemory;
    size_t                 m_offset;
};

#include "ecs/SystemManager.inl"

#endif // !SYSTEM_MANAGER_HPP