#include <glm/trigonometric.hpp>

#include "common/ExternalMath.hpp"



TrigTable::TrigTable() noexcept
{
	for (size_t i = 0; i < TABLE_SIZE; ++i) 
	{
		float radians = glm::radians(static_cast<float>(i));
		m_sinTable[i] = std::sin(radians);
		m_cosTable[i] = std::cos(radians);
	}
}


float TrigTable::getSin(int degrees) const noexcept
{
	int normalized = degrees % TABLE_SIZE;

	if (normalized < 0) 
		normalized += TABLE_SIZE;

	return m_sinTable[static_cast<size_t>(normalized)];
}


float TrigTable::getCos(int degrees) const noexcept 
{
	int normalized = degrees % TABLE_SIZE;

	if (normalized < 0) 
		normalized += TABLE_SIZE;

	return m_cosTable[static_cast<size_t>(normalized)];
}


// int32_t get_distance(const sf::Vector2i& from, const sf::Vector2i& to) noexcept
// {
// 	int32_t distance_x = abs(from.x - to.x);
// 	int32_t distance_y = abs(from.y - to.y);

// 	if (distance_x > distance_y) 
//         return (distance_x + (distance_y >> 1));

// 	return (distance_y + (distance_x >> 1));
// }