#include "assets/AssetManager.hpp"
#include "animation/AnimationManager.hpp"
#include "ui/Cursor.hpp"

Cursor::Cursor() noexcept:
    m_isCaptured(false)
{
    
}

Cursor::~Cursor() = default;

bool Cursor::load(AnimationManager& animator) noexcept
{
    m_frames = animator.loadFramesFromFile(CURSOR_FRAME_XML);

    return !m_frames.empty();
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