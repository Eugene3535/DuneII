#include "assets/AssetManager.hpp"
#include "animation/AnimationManager.hpp"
#include "ui/Cursor.hpp"

#define FRAME_RELEASED "Released"
#define FRAME_CAPTURED "Captured"

Cursor::Cursor() noexcept:
    m_isCaptured(false)
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
        setTexture(*texture);
        release();

        return true;
    }

    return false;
}

void Cursor::capture() noexcept
{
    const auto frame = m_frames[FRAME_CAPTURED];
    setTextureRect(frame);
    m_isCaptured = true;
}

void Cursor::release() noexcept
{
    const auto frame = m_frames[FRAME_RELEASED];
    setTextureRect(frame);
    setOrigin(static_cast<float>(frame.width >> 1), static_cast<float>(frame.height >> 1));
    setScale(0.5f, 0.5f);
    m_isCaptured = false;
}

bool Cursor::isCaptured() const noexcept
{
    return m_isCaptured;
}