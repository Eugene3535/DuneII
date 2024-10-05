#include <cassert>

#include "animation/AnimationManager.hpp"

AnimationManager::AnimationManager() noexcept
{

}

AnimationManager::~AnimationManager() = default;

Animation AnimationManager::createAnimation(const AnimationData& data) noexcept
{
	assert(!data.name.empty());

	const std::string name(data.name);

	if(auto it = m_animations.try_emplace(name); it.second)
	{
		assert(data.texture);

		auto& animation = it.first->second;
		auto& frames    = m_frames.emplace_back();

		animation.texture = data.texture;
		animation.frames  = frames.data();

		switch (data.layout)
		{
			case AnimationData::SINGLE:
			{
				frames.push_back(data.startFrame);
				animation.duration = 1u;
			}
			break;

			case AnimationData::LINEAR:
			{
				assert(data.duration);

				const uint32_t duration = data.duration;
				frames.reserve(static_cast<size_t>(duration));

				sf::IntRect frame = data.startFrame;
				const uint32_t offset = frame.width;

				for (uint32_t i = 0; i < duration; ++i)
				{
					frame.left = i * offset;
					frames.emplace_back(frame);
				}

				animation.duration = duration;
				animation.delay    = data.delay;
			}
			break;

			case AnimationData::GRID:
			{
				assert(data.columns);
				assert(data.rows);

				const uint32_t columns  = data.columns;
				const uint32_t rows     = data.rows;
				const uint32_t duration = rows * columns;
				frames.reserve(static_cast<size_t>(duration));

				sf::IntRect    frame  = data.startFrame;
				const uint32_t width  = frame.width;
				const uint32_t height = frame.height;

				for (uint32_t y = 0; y < rows; ++y)
				{
					for (uint32_t x = 0; x < columns; ++x)
					{
						frame.left = x * width;
						frame.top  = y * height;
						frames.emplace_back(frame);
					}
				}

				animation.duration = duration;
				animation.delay    = data.delay;
			}
			break;
		}

		return animation;
	}

	return {};
}

Animation AnimationManager::getAnimation(const std::string& name) const noexcept
{
    auto found = m_animations.find(name);

	assert(found != m_animations.end());

    return found->second;
}