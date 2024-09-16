#include <new>

#include "managers/ecs/systems/MoveSystem.hpp"
#include "managers/ecs/SystemManager.hpp"

SystemManager::SystemManager() noexcept:
    m_offset(0)
{

}

SystemManager::~SystemManager()
{
    m_rawMemory.clear();
    m_offset = 0;
};

bool SystemManager::initialize(entt::registry& registry) noexcept
{
    constexpr size_t memory_size = 1024 << 6;
    m_rawMemory.resize(memory_size);

    addSystem<MoveSystem>(registry);

    return true; 
}

void SystemManager::updateAllSystems() noexcept
{
    for(auto* system : m_sequentialAccessSystems)
        system->execute();
}