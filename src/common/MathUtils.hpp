#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <cstdint>

int32_t pack_coordinates(int32_t x, int32_t y) noexcept;
void    unpack_coordinates(int32_t packed, int32_t* x, int32_t* y) noexcept;

int32_t get_distance(int32_t from, int32_t to) noexcept;

#endif // !MATH_UTILS_HPP