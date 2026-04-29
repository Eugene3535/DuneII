#ifndef SPRITE_2D_HPP
#define SPRITE_2D_HPP

#include <cstdint>


struct Sprite2D
{
	uint32_t texture;
	uint32_t frame;
	uint32_t width;
	uint32_t height;
};

#endif // !SPRITE_2D_HPP