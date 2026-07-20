#include <array>

#include "RapidXML/rapidxml_utils.hpp"

#include "graphics/vao/VertexBufferLayout.hpp"
#include "graphics/texture/Texture2D.hpp"
#include "graphics/sprites/SpriteManager.hpp"


SpriteManager::SpriteManager() noexcept:
	m_vbo(0),
	m_vao(0)
{

}


SpriteManager::~SpriteManager()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}


void SpriteManager::createSprite(const std::string& name, uint32_t texture) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		GLint width;
		GLint height;
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_WIDTH, &width);
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_HEIGHT, &height);

		const ivec4s textureFrame = { 0, 0, width, height };
		createSprite(name, texture, textureFrame);
	}
}


void SpriteManager::createSprite(const std::string& name, uint32_t texture, const ivec4s& frame) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		const GLuint id = m_sprites.size();
		m_animations.emplace(name, sprite_range(id, 1));

		GLint width;
		GLint height;
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_WIDTH, &width);
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_HEIGHT, &height);
		const vec2s ratio = { 1.f / width, 1.f / height };

		addSprite(texture, frame, ratio);
	}
}


void SpriteManager::createLinearAnimaton(const std::string& name, uint32_t texture, int duration) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		const GLuint id = m_sprites.size();
		m_animations.emplace(name, sprite_range(id, duration));

			GLint width;
		GLint height;
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_WIDTH, &width);
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_HEIGHT, &height);

		const ivec2s size  = { width, height };
		const vec2s ratio = { 1.f / size.x, 1.f / size.y };
		const int frameWidth = width / duration;
	
		for (int i = 0; i < duration; ++i)
		{
			ivec4s frame = { i * frameWidth, 0, frameWidth, size.y };
			addSprite(texture, frame, ratio);
		}
	}
}


void SpriteManager::createGridAnimaton(const std::string& name, uint32_t texture, int columns, int rows) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		const GLuint id = m_sprites.size();
		const GLuint duration = columns * rows;
		m_animations.emplace(name, sprite_range(id, duration));

		GLint texWidth;
		GLint texHeight;
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_WIDTH, &texWidth);
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_HEIGHT, &texHeight);

		const ivec2s  size   = { texWidth, texHeight };
		const vec2s   ratio  = { 1.f / size.x, 1.f / size.y };
		const int32_t width  = size.x / columns;
		const int32_t height = size.y / rows;
	
		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < columns; ++x)
			{
				ivec4s frame = { x * width, y * height, width, height };
				addSprite(texture, frame, ratio);
			}
		}
	}
}


void SpriteManager::createCustomAnimaton(const std::string& name, uint32_t texture, std::span<const ivec4s> frames) noexcept
{
	if(auto it = m_animations.find(name); it == m_animations.end())
	{
		const GLuint id = m_sprites.size();
		m_animations.emplace(name, sprite_range(id, static_cast<GLuint>(frames.size())));

		GLint width;
		GLint height;
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_WIDTH, &width);
		glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_HEIGHT, &height);
		const vec2s ratio = { 1.f / width, 1.f / height };
	
		for (const auto& frame : frames)	
			addSprite(texture, frame, ratio);
	}
}


void SpriteManager::loadSpriteSheet(const std::filesystem::path& filePath, uint32_t texture) noexcept
{
	auto document = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filePath.string().c_str());
	document->parse<0>(xmlFile.data());
	const auto spriteNode = document->first_node("sprites");

	GLint width;
	GLint height;
	glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_WIDTH, &width);
	glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_HEIGHT, &height);

	const ivec2s size = { width, height };
	const vec2s ratio = { 1.f / size.x, 1.f / size.y };

	for(auto animNode = spriteNode->first_node("animation");
		     animNode != nullptr;
		     animNode = animNode->next_sibling("animation"))
	{
		const std::string title = animNode->first_attribute("title")->value();
		const GLuint id = m_sprites.size();

		std::vector<ivec4s> frames;

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
				addSprite(texture, frame, ratio);
		}
	}
}


std::optional<Sprite2D> SpriteManager::getSprite(const std::string& name) const noexcept
{
	if(auto it = m_animations.find(name); it != m_animations.end())
	{
		const auto sprites = m_sprites.data();
		const auto sprite = sprites + static_cast<ptrdiff_t>(it->second.first);

		return std::make_optional<Sprite2D>(*sprite);
	}

	return std::nullopt;
}


std::span<const Sprite2D> SpriteManager::getAnimation(const std::string& name) const noexcept
{
	if(auto it = m_animations.find(name); it != m_animations.end())
	{
		const size_t startFrame = static_cast<size_t>(it->second.first);
		const size_t duration = static_cast<size_t>(it->second.second);

		return { m_sprites.data() + startFrame, duration };
	}

	return {};
}


void SpriteManager::bind(bool toBind) const noexcept
{
	glBindVertexArray(toBind ? m_vao : 0);
}


void SpriteManager::pushVerticesOnGPU() noexcept
{
	if (m_vertices.empty())
		return;

	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);

	glCreateBuffers(1, &m_vbo);
	glNamedBufferData(m_vbo, 0, nullptr, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float4 };
	VertexBufferLayout layout(attributes);
	layout.createVertexInputState(m_vao, m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), static_cast<const void*>(m_vertices.data()), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::vector<float>().swap(m_vertices);
}


void SpriteManager::addSprite(const uint32_t texture, const ivec4s frame, const vec2s ratio) noexcept
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

	const float left   = frame.x * ratio.x;
	const float top    = frame.y * ratio.y;
	const float right  = (frame.x + frame.z) * ratio.x;
	const float bottom = (frame.y + frame.w) * ratio.y;

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