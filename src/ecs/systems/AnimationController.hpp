#ifndef ANIMATION_CONTROLLER_HPP
#define ANIMATION_CONTROLLER_HPP

#include "ecs/systems/base/System.hpp"

class AnimationController : public System<AnimationController>
{
public:
    AnimationController(entt::registry& registry) noexcept;
    ~AnimationController();

    void execute(sf::Time dt) noexcept override;
};

#endif // !ANIMATION_CONTROLLER_HPP