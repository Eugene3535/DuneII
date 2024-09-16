#ifndef MOVE_SYSTEM_HPP
#define MOVE_SYSTEM_HPP

#include "managers/ecs/system/System.hpp"

class MoveSystem : public System<MoveSystem>
{
public:
    MoveSystem(class EntityManager& entityManager) noexcept;
    ~MoveSystem();

    void execute() noexcept override;
};

#endif // !MOVE_SYSTEM_HPP