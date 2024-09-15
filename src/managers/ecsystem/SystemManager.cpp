#include <new>

#include "managers/ecsystem/systems/MoveSystem.hpp"
#include "managers/ecsystem/SystemManager.hpp"

SystemManager::SystemManager() noexcept:
    m_offset(0)
{

}

SystemManager::~SystemManager()
{
    m_rawMemory.clear();
    m_offset = 0;
};

bool SystemManager::initialize(EntityManager& entityManager) noexcept
{
    constexpr size_t memory_size = 1024 << 6;
    m_rawMemory.resize(memory_size);

    addSystem<MoveSystem>(entityManager);

    return true; 
}

void SystemManager::updateAllSystems() noexcept
{
    for(auto* system : m_sequentialAccessSystems)
        system->execute();
}