#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include <string>
#include <vector>

#include <span>
#include <unordered_map>
#include <filesystem>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "resources/ogl/buffers/GlBuffer.hpp"
#include "graphics/Meshes.hpp"


class SpriteManager final
{
	using sprite_range = std::pair<GLuint, GLuint>; // start sprite number, count

public:
	SpriteManager(const GLuint bufferHandle) noexcept;

	void createSprite(const std::string& name, const class Texture& texture) noexcept;
	void createSprite(const std::string& name, const class Texture& texture, const glm::ivec4& frame) noexcept;

	void createLinearAnimaton(const std::string& name, const class Texture& texture, int duration) noexcept;
	void createGridAnimaton(const std::string& name, const class Texture& texture, int columns, int rows) noexcept;
	void createCustomAnimaton(const std::string& name, const class Texture& texture, std::span<const glm::ivec4> frames) noexcept;
	
	void loadSpriteSheet(const std::filesystem::path& filePath, const class Texture& texture) noexcept;

	const GLBuffer& getVertexBuffer() const noexcept;
	std::span<const Sprite> getSprites(const std::string& name) const noexcept;

private:
	void addSprite(GLuint texture, const glm::ivec4& frame, const glm::vec2& ratio) noexcept;

	GLBuffer m_vbo;
	std::unordered_map<std::string, sprite_range> m_animations;
	std::vector<Sprite> m_sprites;
	std::vector<float> m_vertices;
};

#endif // !SPRITE_MANAGER_HPP