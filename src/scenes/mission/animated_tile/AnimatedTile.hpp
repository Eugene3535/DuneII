#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Color.hpp>

// Non-transformable lightweight graphical object with animation capability
class AnimatedTile: 
	public sf::Drawable
{
public:
	AnimatedTile() noexcept;
	AnimatedTile(const sf::Texture* texture) noexcept;
	~AnimatedTile();

	void setTexture(const sf::Texture* texture) noexcept;
	void setTextureRect(const sf::IntRect& rectangle) noexcept;
	void setColor(const sf::Color& color) noexcept;
	void setPosition(int x, int y) noexcept;

	const sf::Color&    getColor()    const noexcept;
	const sf::Vector2i& getSize()     const noexcept;
	sf::Vector2i        getPosition() const noexcept;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Vertex         m_vertices[4u];
	const sf::Texture* m_texture;
	sf::Vector2i       m_size;
};