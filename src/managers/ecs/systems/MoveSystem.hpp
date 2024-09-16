#ifndef MOVE_SYSTEM_HPP
#define MOVE_SYSTEM_HPP

#include "managers/ecs/systems/base/System.hpp"

class MoveSystem : public System<MoveSystem>
{
public:
    MoveSystem(entt::registry& registry) noexcept;
    ~MoveSystem();

    void execute() noexcept override;
};

#endif // !MOVE_SYSTEM_HPP