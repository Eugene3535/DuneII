#pragma once

#include <string>
#include <vector>
#include <span>
#include <unordered_map>
#include <filesystem>
#include <optional>

#include <cglm/struct/ivec4.h>

#include "graphics/geometry/GraphicsData.hpp"

class SpriteManager final
{
	using sprite_range = std::pair<uint32_t, uint32_t>; // start sprite number, count ( single = 1, animation = n )

public:
	SpriteManager() noexcept;
	SpriteManager(const SpriteManager&)              noexcept = delete;
	SpriteManager(SpriteManager&&)                   noexcept = delete;
	SpriteManager& operator = (const SpriteManager&) noexcept = delete;
	SpriteManager& operator = (SpriteManager&&)      noexcept = delete;
	~SpriteManager();

	void createSprite(const std::string& name, uint32_t texture) noexcept;
	void createSprite(const std::string& name, uint32_t texture, const ivec4s& frame) noexcept;

	void createLinearAnimaton(const std::string& name, uint32_t texture, int duration) noexcept;
	void createGridAnimaton(const std::string& name, uint32_t texture, int columns, int rows) noexcept;
	void createCustomAnimaton(const std::string& name, uint32_t texture, std::span<const ivec4s> frames) noexcept;
	
	void loadSpriteSheet(const std::filesystem::path& filePath, uint32_t texture) noexcept;

	std::optional<Sprite2D>   getSprite(const std::string& name) const noexcept;
	std::span<const Sprite2D> getAnimation(const std::string& name) const noexcept;

	void pushVerticesOnGPU(const uint32_t vertexBuffer) noexcept;

private:
	void addSprite(const uint32_t texture, const ivec4s frame, const vec2s ratio) noexcept;

	std::unordered_map<std::string, sprite_range> m_animations;
	std::vector<Sprite2D> m_sprites;
	std::vector<float> m_vertices;
};