#ifndef MESH_HPP
#define MESH_HPP

#include <cstdint>

struct Sprite
{
	uint32_t texture = 0;
	uint32_t frame = 0;
	uint32_t width = 0;
	uint32_t height = 0;
};


struct Animation
{
	uint32_t texture = 0;
	uint32_t begin = 0;
	uint32_t end = 0;
	size_t currentFrame = 0;
};


struct Shape
{
	uint32_t texture = 0;
	uint32_t count = 0;
};


struct Plane
{
	uint32_t texture = 0;
    uint32_t vao = 0;
	uint32_t count = 0;
};

#endif // !MESH_HPP