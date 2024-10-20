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

    return true;
}

void Cursor::capture() noexcept
{
    m_isCaptured = true;
}

void Cursor::release() noexcept
{
    m_isCaptured = false;;
}

bool Cursor::isCaptured() const noexcept
{
    return m_isCaptured;
}