#ifndef MOVE_SYSTEM_HPP
#define MOVE_SYSTEM_HPP

#include "managers/ecs/common/System.hpp"

BEGIN_NAMESPACE_ECS

class MoveSystem : public System<MoveSystem>
{
public:
    MoveSystem(class EntityManager& entityManager) noexcept;
    ~MoveSystem();

    void execute() noexcept override;
};

END_NAMESPACE_ECS

#endif // !MOVE_SYSTEM_HPP