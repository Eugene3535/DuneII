#ifndef ANIMATION_MANAGER_HPP
#define ANIMATION_MANAGER_HPP

#include <list>
#include <vector>
#include <unordered_map>
#include <string>

#include <SFML/Graphics/Rect.hpp>

class AnimationManager
{
public:
    AnimationManager() noexcept;
    ~AnimationManager();

	const class Animation* createAnimation(const class AnimationData& data)  noexcept;
    const class Animation* getAnimation(const std::string& name) const noexcept;

    std::unordered_map<std::string, sf::IntRect> loadFramesFromFile(const std::string& file_name) noexcept;

private:
	std::unordered_map<std::string, class Animation> m_animations;
    std::list<std::vector<sf::IntRect>> m_frames;
};

#endif //!ANIMATION_MANAGER_HPP