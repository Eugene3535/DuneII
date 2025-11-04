#ifndef MESH_HPP
#define MESH_HPP

#include <span>

#include <glad/glad.h>


struct Sprite
{
	void draw() noexcept;

	GLuint texture;
	GLuint frame;
	GLuint width;
	GLuint height;
};


struct SpriteSheet
{
	void draw() noexcept;

	GLuint texture;
	std::span<const GLuint> frames;
	size_t currentFrame;
};


struct Shape
{
	void draw() noexcept;

	GLuint texture;
	GLuint count;
};


struct Plane
{
	void draw() noexcept;

	GLuint texture;
    GLuint vao;
	GLuint count;
};

#endif // !MESH_HPP