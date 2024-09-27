#ifndef MOVE_SYSTEM_HPP
#define MOVE_SYSTEM_HPP

#include "ecs/systems/base/System.hpp"

BEGIN_NAMESPACE_ECS

class MoveSystem : public System<MoveSystem>
{
public:
    MoveSystem(ecs::EntityManager& entityManager) noexcept;
    ~MoveSystem();

    void execute(sf::Time dt) noexcept override;
};

END_NAMESPACE_ECS

#endif // !MOVE_SYSTEM_HPP