#ifndef ENTITY_PREVIEW_HPP
#define ENTITY_PREVIEW_HPP

#include <vector>
#include <span>

#include <cglm/struct/ivec2.h>

#include "common/Enums.hpp"


class EntityPreview
{
public:
    EntityPreview(class Engine* engine) noexcept;
    ~EntityPreview();

    bool loadFromTexture(const struct Texture2D& texture) noexcept;
    void createIcons(const ivec2s position, const ivec2s size) noexcept;

    void setPreviewIcon(EntityIcon icon) noexcept;
    void setConstructionIcon(EntityIcon icon) noexcept;

    void draw() const noexcept;
    void draw(EntityIcon icon, float progress) const noexcept;

private:
    const vec2s* getTexCoords(EntityIcon icon) const noexcept;

    class Engine* m_engine;

    uint32_t m_texture;
    uint32_t m_defaultProgram;
    uint32_t m_progressProgram;
    uint32_t m_vertexBufferObject;
    uint32_t m_vertexArrayObject;

    struct
    {
        int32_t top;
        int32_t bottom;
        int32_t progress;
    } m_uniforms;

    std::vector<vec2s> m_textureGrid;
};

#endif // !ENTITY_PREVIEW_HPP