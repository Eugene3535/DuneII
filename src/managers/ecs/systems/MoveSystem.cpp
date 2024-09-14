#include "managers/ecs/EntityManager.hpp"
#include "managers/ecs/systems/MoveSystem.hpp"

BEGIN_NAMESPACE_ECS

MoveSystem::MoveSystem(EntityManager& entityManager) noexcept :
    System(entityManager)
{
}

MoveSystem::~MoveSystem()
{
}

void MoveSystem::execute() noexcept
{
    const auto& velocities = m_entityManager.velocities;
    auto& positions        = m_entityManager.positions;
    const auto& tables     = m_entityManager.tables;

    for (auto& pair : velocities)
    {
        auto& entity_table = tables[pair.first];
        auto& velocity = pair.second;
        auto& position = positions[entity_table.position].second;

        position.x += velocity.dx;
        position.y += velocity.dy;
    }
}

END_NAMESPACE_ECS