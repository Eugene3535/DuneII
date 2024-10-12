#include "ecs/systems/base/System.hpp"

uint32_t BaseSystem::s_type = 0;

BaseSystem::BaseSystem(entt::registry& registry) noexcept:
    m_registry(registry),
    m_isEnabled(true)
{

}

void BaseSystem::execute(sf::Time dt) noexcept
{

}

uint32_t BaseSystem::getSystemCount() noexcept
{
    return s_type; 
}

void BaseSystem::enable() noexcept
{
    m_isEnabled = true;
}

void BaseSystem::disable() noexcept
{
    m_isEnabled = false;
}

bool BaseSystem::isEnabled() const noexcept
{
    return m_isEnabled;
}