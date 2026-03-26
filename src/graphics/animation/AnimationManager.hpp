#ifndef ANIMATION_MANAGER_HPP
#define ANIMATION_MANAGER_HPP

#include <string>
#include <vector>
#include <span>
#include <unordered_map>
#include <filesystem>

#include <SFML/Graphics/Rect.hpp>


class AnimationManager final
{
public:
	AnimationManager()                                     noexcept;
	AnimationManager(const AnimationManager&)              noexcept = delete;
	AnimationManager(AnimationManager&&)                   noexcept = delete;
	AnimationManager& operator = (const AnimationManager&) noexcept = delete;
	AnimationManager& operator = (AnimationManager&&)      noexcept = delete;
	~AnimationManager();

	void addFrame(const std::string& name, const sf::Vector2u textureSize) noexcept;
	void addFrame(const std::string& name, const sf::IntRect& frame)       noexcept;

	void createLinearAnimaton(const std::string& name, const sf::Vector2u textureSize, uint32_t duration)             noexcept;
	void createGridAnimaton(const std::string& name, const sf::Vector2u textureSize, uint32_t columns, uint32_t rows) noexcept;
	void createCustomAnimaton(const std::string& name, std::span<const sf::IntRect> frames)                           noexcept;
	
	void loadSpriteSheet(const std::filesystem::path& filePath) noexcept;

	std::span<const sf::IntRect> getFrames(const std::string& name) const noexcept;

private:
	std::unordered_map<std::string, std::vector<sf::IntRect>> m_frames;
};

#endif // !ANIMATION_MANAGER_HPP