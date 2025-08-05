#include <memory>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "common/FileProvider.hpp"
#include "animation/AnimationManager.hpp"

AnimationManager::AnimationManager() noexcept
{

}

AnimationManager::~AnimationManager() = default;

const Animation* AnimationManager::createAnimation(const AnimationData& data) noexcept
{
	if(data.name.empty() || !data.texture)
		return nullptr;

	const std::string name(data.name);

	if(auto it = m_animations.try_emplace(name, Animation(data.texture)); it.second)
	{
		auto& animation = it.first->second;
		auto& frames    = m_frames.emplace_back();
		animation.setTexture(*data.texture);
		animation.setTextureRect(data.startFrame);
		
		animation.isLooped = data.isLooped;

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
				if(!data.duration)
					return nullptr;

				const uint32_t duration = data.duration;
				frames.reserve(static_cast<size_t>(duration));

				sf::IntRect frame = data.startFrame;
				const uint32_t offset = frame.size.x;

				for (uint32_t i = 0; i < duration; ++i)
				{
					frames.emplace_back(frame);
					frame.position.x += offset;
				}

				animation.duration = duration;
				animation.delay    = data.delay;
			}
			break;

			case AnimationData::GRID:
			{
				if(data.columns == 0 || data.rows == 0)
					return nullptr;

				const int32_t columns  = static_cast<int32_t>(data.columns);
				const int32_t rows     = static_cast<int32_t>(data.rows);
				const int32_t duration = static_cast<int32_t>(rows * columns);
				frames.reserve(static_cast<size_t>(duration));

				const int32_t left   = data.startFrame.position.x;
				const int32_t top    = data.startFrame.position.y;
				const int32_t width  = data.startFrame.size.x;
				const int32_t height = data.startFrame.size.y;

				for (int32_t y = 0; y < rows; ++y)
					for (int32_t x = 0; x < columns; ++x)
						frames.emplace_back(sf::IntRect({left + x * width, top + y * height}, {width, height}));

				animation.duration = duration;
				animation.delay    = data.delay;
			}
			break;
		}

		animation.frames = frames.data();

		return &animation;
	}

	return nullptr;
}

const Animation* AnimationManager::getAnimation(const std::string& name) const noexcept
{
    auto found = m_animations.find(name);

    return (found != m_animations.end()) ? &found->second : nullptr;
}

std::unordered_map<std::string, sf::IntRect> AnimationManager::loadFramesFromFile(const std::string& file_name) noexcept
{
	std::unordered_map<std::string, sf::IntRect> frames;

	if(const auto file_path = FileProvider::findPathToFile(file_name); !file_path.empty())
	{
		auto document = std::make_unique<rapidxml::xml_document<char>>();
		rapidxml::file<char> xmlFile(file_path.string().c_str());
		document->parse<0>(xmlFile.data());

		const auto sprite_node = document->first_node("sprites");
		auto anim_node = sprite_node->first_node("animation");

		while(anim_node)
		{
			auto title_attr = anim_node->first_attribute("title");

			std::string title = title_attr ? title_attr->value() : std::string();

			if (title.empty())
				continue;

			if(auto it = frames.try_emplace(title); it.second)
			{
				auto& frame   = it.first->second;
				auto cut_node = anim_node->first_node("cut");

				while(cut_node)
				{
					auto x = cut_node->first_attribute("x");
					auto y = cut_node->first_attribute("y");
					auto w = cut_node->first_attribute("w");
					auto h = cut_node->first_attribute("h");

					int left   = x ? atoi(x->value()) : 0;
					int top    = y ? atoi(y->value()) : 0;
					int width  = w ? atoi(w->value()) : 0;
					int height = h ? atoi(h->value()) : 0;

					frame = sf::IntRect({left, top}, {width, height});

					cut_node = cut_node->next_sibling();
				}
			}

			anim_node = anim_node->next_sibling("animation");
		}
	}

	return frames;
}