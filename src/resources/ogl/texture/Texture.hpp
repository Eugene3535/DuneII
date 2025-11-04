#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <array>
#include <filesystem>

#include <glad/glad.h>

// There are a number of different types of textures. These are:

// GL_TEXTURE_1D:                   Images in this texture all are 1-dimensional. They have width, but no height or depth.
// GL_TEXTURE_2D:                   Images in this texture all are 2-dimensional. They have width and height, but no depth.
// GL_TEXTURE_3D:                   Images in this texture all are 3-dimensional. They have width, height, and depth.
// GL_TEXTURE_RECTANGLE:            The image in this texture (only one image. No mipmapping) is 2-dimensional. Texture coordinates used for these textures are not normalized.
// GL_TEXTURE_BUFFER:               The image in this texture (only one image. No mipmapping) is 1-dimensional. The storage for this data comes from a Buffer Object.
// GL_TEXTURE_CUBE_MAP:             There are exactly 6 distinct sets of 2D images, each image being of the same size and must be of a square size. These images act as 6 faces of a cube.
// GL_TEXTURE_1D_ARRAY:             Images in this texture all are 1-dimensional. However, it contains multiple sets of 1-dimensional images, all within one texture. The array length is part of the texture's size.
// GL_TEXTURE_2D_ARRAY:             Images in this texture all are 2-dimensional. However, it contains multiple sets of 2-dimensional images, all within one texture. The array length is part of the texture's size.
// GL_TEXTURE_CUBE_MAP_ARRAY:       Images in this texture are all cube maps. It contains multiple sets of cube maps, all within one texture. The array length * 6 (number of cube faces) is part of the texture size.
// GL_TEXTURE_2D_MULTISAMPLE:       The image in this texture (only one image. No mipmapping) is 2-dimensional. Each pixel in these images contains multiple samples instead of just one value.
// GL_TEXTURE_2D_MULTISAMPLE_ARRAY: Combines 2D array and 2D multisample types. No mipmapping.

// More information is available here: https://www.khronos.org/opengl/wiki/texture

struct Texture
{
	bool loadFromImage(const struct StbImage& image, bool repeat, bool smooth) noexcept;
	bool loadFromFile(const std::filesystem::path& filePath, bool repeat, bool smooth) noexcept;
	bool loadCubeMap(const std::array<std::filesystem::path, 6>& facePaths) noexcept;

	void setSmooth(bool smooth)    noexcept;
	void setRepeated(bool repeate) noexcept;

	GLuint handle = 0;
	GLuint type   = 0;
	GLint format  = 0;
	GLint width   = 0;
    GLint height  = 0;
	GLint depth   = 0;

	bool isSmooth   = false;
	bool isRepeated = false;
};

#endif // !TEXTURE_HPP