#include "RapidXML/rapidxml_utils.hpp"

#include "graphics/animation/AnimationManager.hpp"


AnimationManager::AnimationManager() noexcept = default;
AnimationManager::~AnimationManager() = default;


void AnimationManager::addFrame(const std::string& name, const sf::Vector2u textureSize) noexcept
{
	const sf::IntRect textureFrame = { { 0, 0 }, sf::Vector2i(textureSize) };
	addFrame(name, textureFrame);
}


void AnimationManager::addFrame(const std::string& name, const sf::IntRect& frame) noexcept
{
	auto& frames = m_frames[name];
	frames.push_back(frame);
}


void AnimationManager::createLinearAnimaton(const std::string& name, const sf::Vector2u textureSize, uint32_t duration) noexcept
{
	if(auto it = m_frames.find(name); it == m_frames.end())
	{
		auto& frames = m_frames[name];
		frames.reserve(duration);

		const int32_t frameHeight = textureSize.y;
		const int32_t frameWidth = textureSize.x / duration;
	
		for (int32_t i = 0; i < duration; ++i)
			frames.emplace_back(sf::IntRect({ i * frameWidth, 0 }, { frameWidth, frameHeight }));
	}
}


void AnimationManager::createGridAnimaton(const std::string& name, const sf::Vector2u textureSize, uint32_t columns, uint32_t rows) noexcept
{
	if(auto it = m_frames.find(name); it == m_frames.end())
	{
		auto& frames = m_frames[name];
		frames.reserve(columns * rows);

		const sf::Vector2i size  = sf::Vector2i(textureSize);
		const int32_t frameWidth  = size.x / static_cast<int32_t>(columns);
		const int32_t frameHeight = size.y / static_cast<int32_t>(rows);
	
		for (int32_t y = 0; y < rows; ++y)
			for (int32_t x = 0; x < columns; ++x)
				frames.emplace_back(sf::IntRect({ x * frameWidth, y * frameHeight }, { frameWidth, frameHeight }));
	}
}


void AnimationManager::createCustomAnimaton(const std::string& name, std::span<const sf::IntRect> frames) noexcept
{
	if(auto it = m_frames.find(name); it == m_frames.end())
	{
		auto& customFrames = m_frames[name];
		customFrames.insert(customFrames.end(), frames.begin(), frames.end());
	}
}


void AnimationManager::loadSpriteSheet(const std::filesystem::path& filePath) noexcept
{
	auto document = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filePath.string().c_str());
	document->parse<0>(xmlFile.data());
	const auto spriteNode = document->first_node("sprites");

	for(auto animNode = spriteNode->first_node("animation");
		     animNode != nullptr;
		     animNode = animNode->next_sibling("animation"))
	{
		const std::string title = animNode->first_attribute("title")->value();

		if(auto it = m_frames.find(title); it != m_frames.end())
			continue;
		
		std::vector<sf::IntRect> frames;
		auto cutNode = animNode->first_node("cut");
		
		while (cutNode)
		{
			auto pX = cutNode->first_attribute("x");
			auto pY = cutNode->first_attribute("y");
			auto pW = cutNode->first_attribute("w");
			auto pH = cutNode->first_attribute("h");

			int x = pX ? atoi(pX->value()) : 0;
			int y = pY ? atoi(pY->value()) : 0;
			int w = pW ? atoi(pW->value()) : 0;
			int h = pH ? atoi(pH->value()) : 0;

			frames.emplace_back(sf::IntRect({ x, y }, { w, h }));

			cutNode = cutNode->next_sibling();
		}
		
		if(!frames.empty())
			createCustomAnimaton(title, frames);
	}
}


std::span<const sf::IntRect> AnimationManager::getFrames(const std::string& name) const noexcept
{
	if(auto it = m_frames.find(name); it != m_frames.end())
		return std::span<const sf::IntRect>(it->second);
	
	return {};
}
