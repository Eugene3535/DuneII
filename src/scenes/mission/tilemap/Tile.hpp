#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
	class Texture;
	class Color;
}


class Tile : 
	public sf::Drawable
{
public:
	Tile() noexcept;
	Tile(const class sf::Texture* texture) noexcept;
	Tile(const class sf::Texture* texture, const sf::IntRect& rectangle) noexcept;
	~Tile();

	void setTexture(const class sf::Texture* texture) noexcept;
	void setTextureRect(const sf::IntRect& rectangle) noexcept;
	void setColor(const class sf::Color& color)       noexcept;
	void setScale(float scale)                        noexcept;
	void setDefaultScale()                            noexcept;
	void setPosition(const sf::Vector2f& point)       noexcept;

	const class sf::Color& getColor()    const noexcept;
	const sf::Vector2f&    getPosition() const noexcept;
	const sf::Vector2f&    getSize()     const noexcept;

private:
	void draw(class sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vertex   m_vertices[4u];
	sf::Vector2f m_size;
	const class sf::Texture* m_texture;
};


#endif // !TILE_HPP