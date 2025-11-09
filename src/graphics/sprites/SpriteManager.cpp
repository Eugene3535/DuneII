#include <array>

#include "RapidXML/rapidxml_utils.hpp"

#include "resources/ogl/texture/Texture.hpp"
#include "graphics/sprites/SpriteManager.hpp"


SpriteManager::SpriteManager(const GLuint bufferHandle) noexcept:
	m_vbo(bufferHandle, GL_ARRAY_BUFFER)
{
	m_vbo.create(sizeof(float), 0, nullptr, GL_STATIC_DRAW);
}


void SpriteManager::createSprite(const std::string& name, const Texture& texture) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		const glm::ivec4 textureFrame(0, 0, texture.width, texture.height);
		createSprite(name, texture, textureFrame);
	}
}


void SpriteManager::createSprite(const std::string& name, const Texture& texture, const glm::ivec4& frame) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		const GLuint id = m_sprites.size();
		m_animations.emplace(name, sprite_range(id, 1));

		const glm::vec2 ratio = { 1.f / texture.width, 1.f / texture.height };
		addSprite(texture.handle, frame, ratio);

		m_vbo.update(0, sizeof(float), m_vertices.size(), static_cast<const void*>(m_vertices.data()));
	}
}


void SpriteManager::createLinearAnimaton(const std::string& name, const Texture& texture, int duration) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		const GLuint id = m_sprites.size();
		m_animations.emplace(name, sprite_range(id, duration));

		const glm::ivec2 size  = { texture.width, texture.height };
		const glm::vec2 ratio = 1.f / glm::vec2(size);
		const int frameWidth = texture.width / duration;
		const GLuint handle = texture.handle;
	
		for (int i = 0; i < duration; ++i)
		{
			glm::ivec4 frame = { i * frameWidth, 0, frameWidth, size.y };
			addSprite(handle, frame, ratio);
		}

		m_vbo.update(0, sizeof(float), m_vertices.size(), static_cast<const void*>(m_vertices.data()));
	}
}


void SpriteManager::createGridAnimaton(const std::string& name, const Texture& texture, int columns, int rows) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		const GLuint id = m_sprites.size();
		const GLuint duration = columns * rows;
		m_animations.emplace(name, sprite_range(id, duration));

		const glm::ivec2 size  = { texture.width, texture.height };
		const glm::vec2 ratio = 1.f / glm::vec2(size);

		const int32_t width  = size[0] / static_cast<float>(columns);
		const int32_t height = size[1] / static_cast<float>(rows);
		const GLuint handle = texture.handle;
	
		for (int y = 0; y < rows; ++y)
			for (int x = 0; x < columns; ++x)
			{
				glm::ivec4 frame = { x * width, y * height, width, height };
				addSprite(handle, frame, ratio);
			}

		m_vbo.update(0, sizeof(float), m_vertices.size(), static_cast<const void*>(m_vertices.data()));
	}
}


void SpriteManager::createCustomAnimaton(const std::string& name, const class Texture& texture, std::span<const glm::ivec4> frames) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		const GLuint id = m_sprites.size();
		m_animations.emplace(name, sprite_range(id, static_cast<GLuint>(frames.size())));

		const glm::vec2 ratio = { 1.f / texture.width, 1.f / texture.height };
		const GLuint handle = texture.handle;
	
		for (const auto& frame : frames)	
			addSprite(handle, frame, ratio);

		m_vbo.update(0, sizeof(float), m_vertices.size(), static_cast<const void*>(m_vertices.data()));
	}
}


void SpriteManager::loadSpriteSheet(const std::filesystem::path& filePath, const Texture& texture) noexcept
{
	auto document = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filePath.string().c_str());
	document->parse<0>(xmlFile.data());
	const auto spriteNode = document->first_node("sprites");

	const glm::ivec2 size = { texture.width, texture.height };
	const glm::vec2 ratio = 1.f / glm::vec2(size);
	const GLuint handle = texture.handle;

	for(auto animNode = spriteNode->first_node("animation");
		     animNode != nullptr;
		     animNode = animNode->next_sibling("animation"))
	{
		const std::string title = animNode->first_attribute("title")->value();
		const GLuint id = m_sprites.size();

		std::vector<glm::ivec4> frames;

		if(auto it = m_animations.find(title); it == m_animations.end())
		{
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

				frames.push_back({ x, y, w, h });

				cutNode = cutNode->next_sibling();
			}
		}

		if(!frames.empty())
		{
			m_animations.emplace(title, sprite_range(id, static_cast<GLuint>(frames.size())));

			for(const auto& frame : frames)
				addSprite(handle, frame, ratio);
		}
	}

	m_vbo.update(0, sizeof(float), m_vertices.size(), static_cast<const void*>(m_vertices.data()));
}


const GLBuffer& SpriteManager::getVertexBuffer() const noexcept
{
	return m_vbo;
}


std::span<const Sprite> SpriteManager::getSprites(const std::string& name) const noexcept
{
	if(auto it = m_animations.find(name); it != m_animations.end())
	{
		const auto sprites = m_sprites.data();
		size_t startFrame = static_cast<size_t>(it->second.first);
		size_t duration = static_cast<size_t>(it->second.second);

		return std::span<const Sprite>(sprites + startFrame, duration);
	}

	return std::span<const Sprite>();
}


void SpriteManager::addSprite(GLuint texture, const glm::ivec4& frame, const glm::vec2& ratio) noexcept
{
	auto& sprite   = m_sprites.emplace_back();
	sprite.frame   = m_vertices.size() >> 2;
	sprite.texture = texture;
	sprite.width   = frame.z;
	sprite.height  = frame.w;

	std::array<float, 16> quad = {};

	quad[4]  = static_cast<float>(frame.z);
	quad[8]  = static_cast<float>(frame.z);
	quad[9]  = static_cast<float>(frame.w);
	quad[13] = static_cast<float>(frame.w);

	float left   = frame.x * ratio[0];
	float top    = frame.y * ratio[1];
	float right  = (frame.x + frame.z) * ratio[0];
	float bottom = (frame.y + frame.w) * ratio[1];

	quad[2] = left;
	quad[3] = top;

	quad[6] = right;
	quad[7] = top;

	quad[10] = right;
	quad[11] = bottom;

	quad[14] = left;
	quad[15] = bottom;

	m_vertices.insert(m_vertices.end(), quad.begin(), quad.end());
}