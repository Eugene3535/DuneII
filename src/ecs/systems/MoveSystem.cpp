#include "ecs/systems/MoveSystem.hpp"

MoveSystem::MoveSystem(entt::registry& registry) noexcept :
    System(registry)
{
    
}

MoveSystem::~MoveSystem() = default;

void MoveSystem::execute(sf::Time dt) noexcept
{
    
}