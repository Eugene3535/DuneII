#include "ecs/SystemManager.hpp"

SystemManager::SystemManager() noexcept:
    m_offset(0)
{
    m_rawMemory.resize(max_memory_size);
}

SystemManager::~SystemManager()
{
    clear();
};

void SystemManager::update(sf::Time dt) noexcept
{
    for(auto system : m_sequentialAccessSystems)
        system->execute(dt);
}

void SystemManager::clear() noexcept
{
    m_rawMemory.clear();
    m_offset = 0;
}