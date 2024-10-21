#include <SFML/Graphics/RenderTarget.hpp>

#include "assets/AssetManager.hpp"
#include "animation/AnimationManager.hpp"
#include "ui/Cursor.hpp"

#define FRAME_RELEASED "Released"
#define FRAME_CAPTURED "Captured"

Cursor::Cursor() noexcept:
    m_sprite(),
    m_vertexFrame(sf::Lines, 16),
    m_isCaptured(true),
    m_isSelected(false)
{

}

Cursor::~Cursor() = default;

bool Cursor::load(AnimationManager& animator) noexcept
{
    m_frames = animator.loadFramesFromFile(CURSOR_FRAME_XML);

    for(auto frame: {FRAME_RELEASED, FRAME_CAPTURED})
        if(auto found = m_frames.find(frame); found == m_frames.end())
            return false;

    if(auto texture = Assets->getResource<sf::Texture>(CROSSHAIRS_TILESHEET_PNG); texture != nullptr)
    {
        m_sprite.setTexture(*texture);
        release();
        m_sprite.setScale(0.5f, 0.5f);

        return true;
    }

    return false;
}

void Cursor::select() noexcept
{
    if(!m_isSelected)
    {
        m_isSelected = true;
    }
}

void Cursor::capture() noexcept
{
    if(!m_isCaptured)
    {
        const auto frame = m_frames[FRAME_CAPTURED];
        m_sprite.setTextureRect(frame);
        m_sprite.setOrigin(static_cast<float>(frame.width >> 1), static_cast<float>(frame.height >> 1));
        m_isCaptured = true;
    }
}

void Cursor::release() noexcept
{
    if(m_isCaptured || m_isSelected)
    {
        const auto frame = m_frames[FRAME_RELEASED];
        m_sprite.setTextureRect(frame);
        m_sprite.setOrigin(static_cast<float>(frame.width >> 1), static_cast<float>(frame.height >> 1));
        m_isSelected = false;
        m_isCaptured = false;
    }
}

void Cursor::setPosition(const sf::Vector2f& position) noexcept
{
    m_sprite.setPosition(position);
}

void Cursor::setVertexFrame(const sf::IntRect& frame) noexcept
{
    static constexpr float offset = 16.f;

    const auto leftBottom  = sf::Vector2f(frame.left, frame.top + frame.height);
    const auto leftTop     = sf::Vector2f(frame.left, frame.top);
    const auto rightTop    = sf::Vector2f(frame.left + frame.width, frame.top);
    const auto rightBottom = sf::Vector2f(frame.left + frame.width, frame.top + frame.height);

    m_vertexFrame[0].position = sf::Vector2f(leftBottom.x, leftBottom.y - offset);
    m_vertexFrame[1].position = leftBottom;
    m_vertexFrame[2].position = leftBottom;
    m_vertexFrame[3].position = sf::Vector2f(leftBottom.x + offset, leftBottom.y);

    m_vertexFrame[4].position = sf::Vector2f(leftTop.x, leftTop.y + offset);
    m_vertexFrame[5].position = leftTop;
    m_vertexFrame[6].position = leftTop;
    m_vertexFrame[7].position = sf::Vector2f(leftTop.x + offset, leftTop.y);

    m_vertexFrame[8].position = sf::Vector2f(rightTop.x - offset, rightTop.y);
    m_vertexFrame[9].position = rightTop;
    m_vertexFrame[10].position = rightTop;
    m_vertexFrame[11].position = sf::Vector2f(rightTop.x, rightTop.y + offset);

    m_vertexFrame[12].position = sf::Vector2f(rightBottom.x, rightBottom.y - offset);
    m_vertexFrame[13].position = rightBottom;
    m_vertexFrame[14].position = rightBottom;
    m_vertexFrame[15].position = sf::Vector2f(rightBottom.x - offset, rightBottom.y);
}

bool Cursor::isSelected() const noexcept
{
    return m_isSelected;
}

bool Cursor::isCaptured() const noexcept
{
    return m_isCaptured;
}

void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);

    if(m_isSelected)
        target.draw(m_vertexFrame, states);
}