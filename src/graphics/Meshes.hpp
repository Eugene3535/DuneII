#ifndef MESH_HPP
#define MESH_HPP

#include <cstdint>

// Data structures representing a set of graphical data of entities

namespace mesh
{
	struct Sprite
	{
		uint32_t texture;
		uint32_t frame;
		uint32_t width;
		uint32_t height;
	};


	struct Animation
	{
		uint32_t texture;
		uint32_t begin;
		uint32_t end;
		uint32_t currentFrame;
	};


	struct Plane
	{
		uint32_t texture;
		uint32_t vao;
		uint32_t count; // indices
	};


	struct Landscape
	{
		uint32_t texture;
		uint32_t vao;
		uint32_t vbo[2];
		uint32_t count; // indices
		uint32_t program;
	};


	struct Buildings
	{
		uint32_t texture;
		uint32_t vao;
	};
}

#endif // !MESH_HPP