#include "ecs/systems/base/System.hpp"

uint32_t BaseSystem::s_type = 0;

BaseSystem::BaseSystem(entt::registry& registry) noexcept:
    m_registry(registry)
{

}

void BaseSystem::execute(sf::Time dt) noexcept
{

}

uint32_t BaseSystem::getSystemCount() noexcept
{
    return s_type; 
}