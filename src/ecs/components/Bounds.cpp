#include "ecs/components/Bounds.hpp"

BEGIN_NAMESPACE_ECS

Bounds::Bounds() noexcept:
    sf::IntRect()
{

}

Bounds::Bounds(int32_t rectLeft, int32_t rectTop, int32_t rectWidth, int32_t rectHeight) noexcept:
    sf::IntRect(rectLeft, rectTop, rectWidth, rectHeight)
{

}

Bounds::Bounds(sf::Vector2i& position, sf::Vector2i& size) noexcept:
    sf::IntRect(position, size)
{

}

END_NAMESPACE_ECS