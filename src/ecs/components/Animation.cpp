#include "ecs/components/Animation.hpp"


Animation::Animation(const sf::Texture* texture) noexcept:
    sf::Sprite(*texture),
    currentFrame(0),
    delay(),
    timer(),
    isLooped(false),
    isOver(false)
{

}

Animation::Animation(const sf::Texture* texture, const sf::IntRect& startFrame) noexcept:
    sf::Sprite(*texture, startFrame),
    currentFrame(0),
    delay(),
    timer(),
    isLooped(false),
    isOver(false)
{

}