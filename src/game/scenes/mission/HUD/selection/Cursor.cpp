#include <SFML/Graphics/RenderTarget.hpp>

#include "resources/files/assets/AssetManager.hpp"
#include "graphics/animation/AnimationManager.hpp"
#include "game/scenes/mission/HUD/selection/Cursor.hpp"


#define FRAME_RELEASED "Released"
#define FRAME_CAPTURED "Captured"

constexpr static sf::Time blink_period    = sf::milliseconds(250);
constexpr static sf::Time blink_loop_time = sf::milliseconds(500);


Cursor::Cursor() noexcept:
    m_vertexFrame(sf::PrimitiveType::Lines, 16),
    m_isCaptured(true),
    m_isSelected(false),
    m_tick()
{

}


Cursor::~Cursor() noexcept = default;


bool Cursor::load(AnimationManager& animations, AssetManager& assets) noexcept
{
    animations.loadSpriteSheet(FileProvider::findPathToFile(CURSOR_FRAMES_XML));

    if (auto frames = animations.getFrames(FRAME_RELEASED); !frames.empty())
        m_releasedFrame = frames[0];

    if (auto frames = animations.getFrames(FRAME_CAPTURED); !frames.empty())
        m_capturedFrame = frames[0];

    if(m_releasedFrame == sf::IntRect() || m_capturedFrame == sf::IntRect())
        return false;

    if(auto texture = assets.get<sf::Texture>(CROSSHAIRS_TILESHEET_PNG))
    {
        m_sprite = std::make_unique<sf::Sprite>(*texture, m_releasedFrame);
        release();
        m_sprite->setScale({0.5f, 0.5f});

        return true;
    }

    return false;
}


void Cursor::select() noexcept
{
    m_isSelected = true;
}


void Cursor::capture() noexcept
{
    if(!m_isCaptured)
    {
        const auto frame = m_capturedFrame;
        m_sprite->setTextureRect(frame);
        m_sprite->setOrigin({static_cast<float>(frame.size.x >> 1), static_cast<float>(frame.size.y >> 1)});
        m_isCaptured = true;
    }
}


void Cursor::release() noexcept
{
    if(m_isCaptured || m_isSelected)
    {
        const auto frame = m_releasedFrame;
        m_sprite->setTextureRect(frame);
        m_sprite->setOrigin({static_cast<float>(frame.size.x >> 1), static_cast<float>(frame.size.y >> 1)});
        m_isSelected = false;
        m_isCaptured = false;
    }
}


void Cursor::update(const sf::Vector2f& position, sf::Time dt) noexcept
{
    m_sprite->setPosition(position);

    m_tick += dt;

    if(m_tick > blink_loop_time)
        m_tick = sf::Time::Zero;
}


void Cursor::setVertexFrame(const sf::IntRect& frame) noexcept
{
    const float offset = 12.f;

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

    if(m_isSelected && m_tick < blink_period)
        target.draw(m_vertexFrame, states);
}