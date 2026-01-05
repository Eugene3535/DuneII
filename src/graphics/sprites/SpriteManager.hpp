#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include <string>
#include <vector>
#include <span>
#include <unordered_map>
#include <filesystem>
#include <optional>

#include <cglm/struct/ivec4.h>

#include "graphics/Meshes.hpp"


class SpriteManager final
{
	using sprite_range = std::pair<uint32_t, uint32_t>; // start sprite number, count

public:
	SpriteManager() noexcept;
	SpriteManager(const SpriteManager&)              noexcept = delete;
	SpriteManager(SpriteManager&&)                   noexcept = delete;
	SpriteManager& operator = (const SpriteManager&) noexcept = delete;
	SpriteManager& operator = (SpriteManager&&)      noexcept = delete;
	~SpriteManager();

	void createSprite(const std::string& name, const struct Texture& texture) noexcept;
	void createSprite(const std::string& name, const struct Texture& texture, const ivec4s& frame) noexcept;

	void createLinearAnimaton(const std::string& name, const struct Texture& texture, int duration) noexcept;
	void createGridAnimaton(const std::string& name, const struct Texture& texture, int columns, int rows) noexcept;
	void createCustomAnimaton(const std::string& name, const struct Texture& texture, std::span<const ivec4s> frames) noexcept;
	
	void loadSpriteSheet(const std::filesystem::path& filePath, const struct Texture& texture) noexcept;

	std::optional<Sprite> getSprite(const std::string& name) const noexcept;
	std::vector<Sprite> getAnimation(const std::string& name) const noexcept;

	void bind(bool toBind) const noexcept;

private:
	void addSprite(const uint32_t texture, const ivec4s frame, const vec2s ratio) noexcept;
	void pushVerticesOnGPU() noexcept;

	uint32_t m_vao;
	uint32_t m_vbo;

	std::unordered_map<std::string, sprite_range> m_animations;
	std::vector<Sprite> m_sprites;
	std::vector<float> m_vertices;
};

#endif // !SPRITE_MANAGER_HPP