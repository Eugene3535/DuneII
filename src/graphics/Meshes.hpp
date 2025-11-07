#ifndef MESH_HPP
#define MESH_HPP

#include <cstdint>

struct Sprite
{
	void draw() noexcept;

	uint32_t texture = 0;
	uint32_t frame = 0;
	uint32_t width = 0;
	uint32_t height = 0;
};


struct Animation
{
	void draw() noexcept;

	uint32_t texture = 0;
	uint32_t begin = 0;
	uint32_t end = 0;
	size_t currentFrame = 0;
};


struct Shape
{
	void draw() noexcept;

	uint32_t texture = 0;
	uint32_t count = 0;
};


struct Plane
{
	void draw() noexcept;

	uint32_t texture = 0;
    uint32_t vao = 0;
	uint32_t count = 0;
};

#endif // !MESH_HPP