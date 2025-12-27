#include "resources/files/FileProvider.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"
#include "game/scenes/mission/builder/Builder.hpp"
#include "game/scenes/mission/HUD/HeadUpDisplay.hpp"


HeadUpDisplay::HeadUpDisplay(const Builder& builder) noexcept:
    m_builder(builder)
{
    
}


HeadUpDisplay::~HeadUpDisplay()
{

}


void HeadUpDisplay::initCrosshairs(std::span<const Sprite> crosshairs) noexcept
{
    m_cursor = crosshairs[0];
    m_transform.setOrigin(static_cast<float>(m_cursor.width >> 1), static_cast<float>(m_cursor.height >> 1));
    m_transform.setScale(0.5f, 0.5f);
}


void HeadUpDisplay::update(const vec2s position, float dt) noexcept
{
    m_transform.setPosition(position);
}


void HeadUpDisplay::draw() const noexcept
{
    glBindTextureUnit(0, m_cursor.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_cursor.frame, 4);
    glBindTextureUnit(0, 0);
}


const Transform2D& HeadUpDisplay::getTransform() const noexcept
{
    return m_transform;
}