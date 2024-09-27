#include "ecs/systems/MoveSystem.hpp"

BEGIN_NAMESPACE_ECS

MoveSystem::MoveSystem(ecs::EntityManager& entityManager) noexcept :
    System(entityManager)
{
    
}

MoveSystem::~MoveSystem() = default;

void MoveSystem::execute(sf::Time dt) noexcept
{
    
}

END_NAMESPACE_ECS