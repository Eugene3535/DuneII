#define _USE_MATH_DEFINES
#include <cmath>

#include "common/MathUtils.hpp"

int32_t pack_coordinates(int32_t x, int32_t y) noexcept 
{
    return (x & 0xFFFF) | ((y & 0xFFFF) << 16);
}

void unpack_coordinates(int32_t packed, int32_t* x, int32_t* y) noexcept 
{
    *x = packed & 0xFFFF;
    *y = (packed >> 16) & 0xFFFF;
}

int32_t get_distance(int32_t from, int32_t to) noexcept
{
    int32_t from_x;
    int32_t from_y;
    int32_t to_x;
    int32_t to_y;

    unpack_coordinates(from, &from_x, &from_y);
    unpack_coordinates(to, &to_x, &to_y);

	int32_t distance_x = abs(from_x - to_x);
	int32_t distance_y = abs(from_y - to_y);

	if (distance_x > distance_y) 
        return (distance_x + (distance_y >> 1));

	return (distance_y + (distance_x >> 1));
}