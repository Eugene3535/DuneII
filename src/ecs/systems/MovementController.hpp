#ifndef MOVE_CONTROLLER_HPP
#define MOVE_CONTROLLER_HPP

#include "ecs/systems/base/System.hpp"

class MovementController : public System<MovementController>
{
public:
    MovementController(entt::registry& registry) noexcept;
    ~MovementController();

    void execute(sf::Time dt) noexcept override;
};

#endif // !MOVE_CONTROLLER_HPP