#define _USE_MATH_DEFINES
#include <cmath>

#include <SFML/System/Vector2.hpp>

#include "common/MathUtils.hpp"

int32_t get_distance(const sf::Vector2i& from, const sf::Vector2i& to) noexcept
{
	int32_t distance_x = abs(from.x - to.x);
	int32_t distance_y = abs(from.y - to.y);

	if (distance_x > distance_y) 
        return (distance_x + (distance_y >> 1));

	return (distance_y + (distance_x >> 1));
}