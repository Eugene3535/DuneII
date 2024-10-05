#ifndef ANIMATION_MANAGER_HPP
#define ANIMATION_MANAGER_HPP

#include <list>
#include <vector>
#include <unordered_map>
#include <string>

#include <SFML/System/NonCopyable.hpp>

#include "animation/AnimationData.hpp"

class AnimationManager:
	private sf::NonCopyable
{
public:
    AnimationManager() noexcept;
    ~AnimationManager();

	Animation createAnimation(const AnimationData& data)  noexcept;
    Animation getAnimation(const std::string& name) const noexcept;

private:
	std::unordered_map<std::string, Animation> m_animations;
    std::list<std::vector<sf::IntRect>>        m_frames;
};

#endif //!ANIMATION_MANAGER_HPP