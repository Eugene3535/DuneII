#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "graphics/quad/Quad.hpp"


Quad::Quad() noexcept: 
	m_texture(nullptr)
{

}


Quad::Quad(const sf::Texture* texture) noexcept: 
	m_texture(nullptr)
{
	setTexture(texture);
}


Quad::Quad(const sf::Texture* texture, const sf::IntRect& rectangle) noexcept: 
	m_texture(nullptr)
{
	setTexture(texture);
	setTextureRect(rectangle);
}


void Quad::setTexture(const sf::Texture* texture) noexcept
{
	m_texture = texture;
	m_size = sf::Vector2f(texture->getSize());

	m_vertices[0].texCoords = sf::Vector2f(0, 0);
	m_vertices[1].texCoords = sf::Vector2f(m_size.x, 0);
	m_vertices[2].texCoords = sf::Vector2f(m_size);
	m_vertices[3].texCoords = sf::Vector2f(0, m_size.y);

	setPosition(sf::Vector2f());
}


void Quad::setTextureRect(const sf::IntRect& rectangle) noexcept
{
	m_size = sf::Vector2f(rectangle.size);
	m_vertices[0].texCoords = sf::Vector2f(rectangle.position.x, rectangle.position.y);
	m_vertices[1].texCoords = sf::Vector2f(rectangle.position.x + rectangle.size.x, rectangle.position.y);
	m_vertices[2].texCoords = sf::Vector2f(rectangle.position.x + rectangle.size.x, rectangle.position.y + rectangle.size.y);
	m_vertices[3].texCoords = sf::Vector2f(rectangle.position.x, rectangle.position.y + rectangle.size.y);

	setPosition(m_vertices[0].position);
}


void Quad::setColor(const sf::Color& color) noexcept
{
	m_vertices[0].color = color;
	m_vertices[1].color = color;
	m_vertices[2].color = color;
	m_vertices[3].color = color;
}


void Quad::setScale(float scale) noexcept
{
	m_size *= scale;
	setPosition(m_vertices[0].position);
}


void Quad::setDefaultScale() noexcept
{
	m_size = m_vertices[2].texCoords;
	setPosition(m_vertices[0].position);
}


void Quad::setPosition(const sf::Vector2f& point) noexcept
{
	m_vertices[0].position = point;
	m_vertices[1].position = sf::Vector2f(point.x + m_size.x, point.y);
	m_vertices[2].position = sf::Vector2f(point.x + m_size.x, point.y + m_size.y);
	m_vertices[3].position = sf::Vector2f(point.x, point.y + m_size.y);
}


sf::Color Quad::getColor() const noexcept
{
	return m_vertices[0].color;
}


const sf::Vector2f& Quad::getPosition() const noexcept
{
	return m_vertices[0].position;
}

// return object size in owner window in pixels
const sf::Vector2f& Quad::getSize() const noexcept
{
	return m_size;
}


void Quad::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = m_texture;
	target.draw(m_vertices, 4, sf::PrimitiveType::TriangleFan, states);
}
