#include <new>

#include "ecs/SystemManager.hpp"

SystemManager::SystemManager() noexcept:
    m_offset(0)
{

}

SystemManager::~SystemManager()
{
    clear();
};

void SystemManager::initialize() noexcept
{
    constexpr size_t memory_size = 1024 << 6; // 64 kB
    m_rawMemory.resize(memory_size);
}

void SystemManager::updateAllSystems() noexcept
{
    for(auto system : m_sequentialAccessSystems)
        system->execute();
}

void SystemManager::clear() noexcept
{
    m_rawMemory.clear();
    m_offset = 0;
}