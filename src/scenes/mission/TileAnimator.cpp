#include "TileAnimator.hpp"

TileAnimator::TileAnimator() noexcept:
	m_frames(nullptr),
	m_current_frame(nullptr),
	m_vertices(nullptr),
	m_delay(0),
	m_timer(0)
{
}

void TileAnimator::set_vertices(sf::Vertex* vertices) noexcept
{
	m_vertices = vertices;
}

void TileAnimator::set_frames(const std::vector<sf::IntRect>& frames) noexcept
{
	m_frames = &frames;

	if(m_vertices)
	{
		m_current_frame = frames.data();
		set_texture_rect(m_current_frame);
	}
}

void TileAnimator::set_delay(std::int32_t milliseconds) noexcept
{
	m_delay = milliseconds;
}

void TileAnimator::set_position(const sf::Vector2f& position) noexcept
{
	if(m_vertices)
	{
		sf::Vector2f size = m_vertices[2].texCoords - m_vertices[0].texCoords;

		m_vertices[0].position = position;
		m_vertices[1].position = { position.x + size.x, position.y };
		m_vertices[2].position = position + size;

		m_vertices[3].position = position;
		m_vertices[4].position = position + size;
		m_vertices[5].position = { position.x, position.y + size.y };
	}
}

void TileAnimator::set_color(const sf::Color& color) noexcept
{
	if(m_vertices)
	{
		m_vertices[0].color = color;
		m_vertices[1].color = color;
		m_vertices[2].color = color;
		m_vertices[3].color = color;
		m_vertices[4].color = color;
		m_vertices[5].color = color;
	}
}

const std::vector<sf::IntRect>* TileAnimator::get_frames() const noexcept
{
	return m_frames;
}

sf::Vector2f TileAnimator::get_position() const noexcept
{
	if(m_vertices)
		return m_vertices[0].position;

	return sf::Vector2f();
}

sf::Color TileAnimator::get_color() const noexcept
{
	if(m_vertices)
		return m_vertices[0].color;

	return sf::Color::White;
}

sf::Vector2f TileAnimator::get_size() const noexcept
{
	if(m_vertices)
		return (m_vertices[2].position - m_vertices[0].position);

	return sf::Vector2f();
}

void TileAnimator::update(std::int32_t milliseconds) noexcept
{
#ifdef DEBUG
	if(!(m_vertices && m_frames)) return;
#endif

	m_timer += milliseconds;

	if (m_timer > m_delay)
	{
		m_timer = 0;
		++m_current_frame;

		if (const sf::IntRect* frames_data = m_frames->data(); m_current_frame == (frames_data + m_frames->size() - 1))
			m_current_frame = frames_data;

		set_texture_rect(m_current_frame);
	}
}

void TileAnimator::set_texture_rect(const sf::IntRect* rectangle) noexcept
{
	m_vertices[0].texCoords = sf::Vector2f(rectangle->left,  rectangle->top);
	m_vertices[1].texCoords = sf::Vector2f(rectangle->left + rectangle->width, rectangle->top);
	m_vertices[2].texCoords = sf::Vector2f(rectangle->left + rectangle->width, rectangle->top + rectangle->height);
	
	m_vertices[3].texCoords = sf::Vector2f(rectangle->left,  rectangle->top);
	m_vertices[4].texCoords = sf::Vector2f(rectangle->left + rectangle->width, rectangle->top + rectangle->height);
	m_vertices[5].texCoords = sf::Vector2f(rectangle->left,  rectangle->top + rectangle->height);

	set_position(m_vertices[0].position);
}