#include <SFML/Graphics/RenderTarget.hpp>

#include "assets/AssetManager.hpp"
#include "animation/AnimationManager.hpp"
#include "ui/Cursor.hpp"

#define FRAME_RELEASED "Released"
#define FRAME_CAPTURED "Captured"

Cursor::Cursor() noexcept:
    m_vertexFrame(sf::PrimitiveType::Lines, 16),
    m_isCaptured(true),
    m_isSelected(false),
    m_tick()
{

}

Cursor::~Cursor() noexcept = default;

bool Cursor::load(AnimationManager& animator) noexcept
{
    m_frames = animator.loadFramesFromFile(CURSOR_FRAME_XML);

    for(auto frame: {FRAME_RELEASED, FRAME_CAPTURED})
        if(auto found = m_frames.find(frame); found == m_frames.end())
            return false;

    if(auto texture = Assets->getResource<sf::Texture>(CROSSHAIRS_TILESHEET_PNG); texture != nullptr)
    {
        m_sprite = std::make_unique<sf::Sprite>(*texture);
        release();
        m_sprite->setScale({0.5f, 0.5f});

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
        m_sprite->setTextureRect(frame);
        m_sprite->setOrigin({static_cast<float>(frame.size.x >> 1), static_cast<float>(frame.size.y >> 1)});
        m_isCaptured = true;
    }
}

void Cursor::release() noexcept
{
    if(m_isCaptured || m_isSelected)
    {
        const auto frame = m_frames[FRAME_RELEASED];
        m_sprite->setTextureRect(frame);
        m_sprite->setOrigin({static_cast<float>(frame.size.x >> 1), static_cast<float>(frame.size.y >> 1)});
        m_isSelected = false;
        m_isCaptured = false;
    }
}

void Cursor::update(const sf::Vector2f& position, sf::Time dt) noexcept
{
    m_sprite->setPosition(position);

    static const sf::Time delay = sf::milliseconds(250);
    m_tick += dt;

    if(m_tick > delay)
        m_tick = sf::Time::Zero;
}

void Cursor::setVertexFrame(const sf::IntRect& frame) noexcept
{
    static constexpr float offset = 12.f;

    const auto leftBottom  = sf::Vector2f(frame.position.x, frame.position.y + frame.size.y);
    const auto leftTop     = sf::Vector2f(frame.position.x, frame.position.y);
    const auto rightTop    = sf::Vector2f(frame.position.x + frame.size.x, frame.position.y);
    const auto rightBottom = sf::Vector2f(frame.position.x + frame.size.x, frame.position.y + frame.size.y);

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
    target.draw(*m_sprite, states);

    static const sf::Time delay = sf::milliseconds(125);

    if(m_isSelected && m_tick < delay)
        target.draw(m_vertexFrame, states);
}