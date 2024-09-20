#ifndef MOVE_SYSTEM_HPP
#define MOVE_SYSTEM_HPP

#include "ecs/systems/base/System.hpp"

class MoveSystem : public System<MoveSystem>
{
public:
    MoveSystem(ecs::EntityManager& entityManager) noexcept;
    ~MoveSystem();

    void execute() noexcept override;
};

#endif // !MOVE_SYSTEM_HPP