#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class TileAnimator
{
public:
	TileAnimator() noexcept;

	void set_vertices(sf::Vertex* vertices)                 noexcept;
	void set_frames(const std::vector<sf::IntRect>& frames) noexcept;
	void set_delay(std::int32_t milliseconds)               noexcept;
	void set_position(const sf::Vector2f& position)         noexcept;
	void set_color(const sf::Color& color)                  noexcept;
	
	sf::Vertex*                     get_vertices() const noexcept;
	const std::vector<sf::IntRect>* get_frames()   const noexcept;
	sf::Vector2f                    get_position() const noexcept;
	sf::Color                       get_color()    const noexcept;
	sf::Vector2f                    get_size()     const noexcept;

	void update(std::int32_t milliseconds) noexcept;

private:
	void set_texture_rect(const sf::IntRect* rectangle) noexcept;

private:
	const std::vector<sf::IntRect>* m_frames;
	const sf::IntRect* m_current_frame;
	sf::Vertex*  m_vertices;
	std::int32_t m_delay;
	std::int32_t m_timer;
};