#include "ecs/systems/base/System.hpp"

uint32_t BaseSystem::s_type = 0;

BaseSystem::BaseSystem(ecs::EntityManager& entityManager) noexcept:
    m_entityManager(entityManager)
{

}

void BaseSystem::execute(sf::Time dt) noexcept
{

}

uint32_t BaseSystem::getSystemCount() noexcept
{
    return s_type; 
}