#include "scenes/mission/animated_tile/AnimatedTile.hpp"

AnimatedTile::AnimatedTile() noexcept: 
	m_texture(nullptr)
{
}

AnimatedTile::AnimatedTile(const sf::Texture* texture) noexcept: 
	m_texture(nullptr)
{
	setTexture(texture);
}

AnimatedTile::~AnimatedTile()
{
}

void AnimatedTile::setTexture(const sf::Texture* texture) noexcept
{
	m_texture = texture;
	m_size = static_cast<sf::Vector2i>(texture->getSize());

	m_vertices[0].texCoords = sf::Vector2f(0, 0);
	m_vertices[1].texCoords = sf::Vector2f(m_size.x, 0);
	m_vertices[2].texCoords = sf::Vector2f(m_size);
	m_vertices[3].texCoords = sf::Vector2f(0, m_size.y);

	setPosition(0, 0);
}

void AnimatedTile::setTextureRect(const sf::IntRect& rectangle) noexcept
{
	m_size = sf::Vector2i(rectangle.width, rectangle.height);

	m_vertices[0].texCoords = sf::Vector2f(rectangle.left, rectangle.top);
	m_vertices[1].texCoords = sf::Vector2f(rectangle.left + rectangle.width, rectangle.top);
	m_vertices[2].texCoords = sf::Vector2f(rectangle.left + rectangle.width, rectangle.top + rectangle.height);
	m_vertices[3].texCoords = sf::Vector2f(rectangle.left, rectangle.top + rectangle.height);

	setPosition(m_vertices[0].position.x, m_vertices[0].position.y);
}

void AnimatedTile::setColor(const sf::Color& color) noexcept
{
	m_vertices[0].color = color;
	m_vertices[1].color = color;
	m_vertices[2].color = color;
	m_vertices[3].color = color;
}

const sf::Color& AnimatedTile::getColor() const noexcept
{
	return m_vertices[0].color;
}


const sf::Vector2i& AnimatedTile::getSize() const noexcept
{
	return m_size;
}

sf::Vector2i AnimatedTile::getPosition() const noexcept
{
	return static_cast<sf::Vector2i>(m_vertices[0].position);
}

void AnimatedTile::setPosition(int x, int y) noexcept
{
	sf::Vector2f point(x, y);

	m_vertices[0].position = point;
	m_vertices[1].position = sf::Vector2f(point.x + m_size.x, point.y);
	m_vertices[2].position = sf::Vector2f(point.x + m_size.x, point.y + m_size.y);
	m_vertices[3].position = sf::Vector2f(point.x, point.y + m_size.y);
}

void AnimatedTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(m_texture)
	{
		states.texture = m_texture;
		target.draw(m_vertices, 4, sf::TriangleFan, states);
	}
}
