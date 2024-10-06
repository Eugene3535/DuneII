#include <SFML/Graphics/Sprite.hpp>

#include "ecs/components/Animation.hpp"
#include "ecs/systems/AnimationController.hpp"

AnimationController::AnimationController(entt::registry& registry) noexcept :
    System(registry)
{
    
}

AnimationController::~AnimationController() = default;

void AnimationController::execute(sf::Time dt) noexcept
{
    auto view = m_registry.view<Animation>();

    for (auto [entity, animation] : view.each())
    {
        if( ! animation.isOver )
        {
            animation.timer += dt;

            if(animation.timer > animation.delay)
            {
                animation.currentFrame++;
                animation.timer = sf::Time::Zero;

                if(animation.currentFrame == animation.duration)
                {
                    if(animation.isLooped)
                    {
                        animation.currentFrame = 0;
                    }
                    else
                    {
                        animation.isOver = true;
                        continue;
                    }
                }

                animation.setTextureRect(animation.frames[animation.currentFrame]);
            }
        }
    }
}