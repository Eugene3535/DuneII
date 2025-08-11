#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "scenes/mission/tilemap/Tile.hpp"


Tile::Tile() noexcept: 
	m_texture(nullptr)
{

}


Tile::Tile(const sf::Texture* texture) noexcept: 
	m_texture(nullptr)
{
	setTexture(texture);
}


Tile::Tile(const sf::Texture* texture, const sf::IntRect& rectangle) noexcept: 
	m_texture(nullptr)
{
	setTexture(texture);
	setTextureRect(rectangle);
}


Tile::~Tile() = default;


void Tile::setTexture(const sf::Texture* texture) noexcept
{
	m_texture = texture;
	m_size = sf::Vector2f(texture->getSize());

	m_vertices[0].texCoords = sf::Vector2f(0, 0);
	m_vertices[1].texCoords = sf::Vector2f(m_size.x, 0);
	m_vertices[2].texCoords = sf::Vector2f(m_size);
	m_vertices[3].texCoords = sf::Vector2f(0, m_size.y);

	setPosition(sf::Vector2f());
}


void Tile::setTextureRect(const sf::IntRect& rectangle) noexcept
{
	m_size = sf::Vector2f(rectangle.size);
	m_vertices[0].texCoords = sf::Vector2f(rectangle.position.x, rectangle.position.y);
	m_vertices[1].texCoords = sf::Vector2f(rectangle.position.x + rectangle.size.x, rectangle.position.y);
	m_vertices[2].texCoords = sf::Vector2f(rectangle.position.x + rectangle.size.x, rectangle.position.y + rectangle.size.y);
	m_vertices[3].texCoords = sf::Vector2f(rectangle.position.x, rectangle.position.y + rectangle.size.y);

	setPosition(m_vertices[0].position);
}


void Tile::setColor(const sf::Color& color) noexcept
{
	m_vertices[0].color = color;
	m_vertices[1].color = color;
	m_vertices[2].color = color;
	m_vertices[3].color = color;
}


void Tile::setScale(float scale) noexcept
{
	m_size *= scale;
	setPosition(m_vertices[0].position);
}


void Tile::setDefaultScale() noexcept
{
	m_size = m_vertices[2].texCoords;
	setPosition(m_vertices[0].position);
}


void Tile::setPosition(const sf::Vector2f& point) noexcept
{
	m_vertices[0].position = point;
	m_vertices[1].position = sf::Vector2f(point.x + m_size.x, point.y);
	m_vertices[2].position = sf::Vector2f(point.x + m_size.x, point.y + m_size.y);
	m_vertices[3].position = sf::Vector2f(point.x, point.y + m_size.y);
}


const sf::Color& Tile::getColor() const noexcept
{
	return m_vertices[0].color;
}


const sf::Vector2f& Tile::getPosition() const noexcept
{
	return m_vertices[0].position;
}

// return object size in owner window in pixels
const sf::Vector2f& Tile::getSize() const noexcept
{
	return m_size;
}


void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = m_texture;
	target.draw(m_vertices, 4, sf::PrimitiveType::TriangleFan, states);
}
