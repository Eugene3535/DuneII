#include "managers/ecs/EntityManager.hpp"
#include "managers/ecs/common/System.hpp"

uint32_t BaseSystem::s_type = 0;

BaseSystem::BaseSystem(EntityManager& entityManager) noexcept:
    m_entityManager(entityManager)
{

}

void BaseSystem::execute() noexcept
{

}

uint32_t BaseSystem::getSystemCount() noexcept
{
    return s_type; 
}