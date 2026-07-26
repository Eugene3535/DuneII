#pragma once

#include <cstdint>
#include <string_view>
#include <functional>

#include <cglm/struct/vec2.h>

#include "graphics/transform/Transform2D.hpp"


class Scene
{
public:
    enum Type : uint32_t
    {
        NONE = 0,
        MAIN_MENU,
        PICK_HOUSE,
        MISSION
    };

	Scene(struct Game* game, const Type type) noexcept;
	virtual ~Scene();
    
    virtual bool load(std::string_view info)   noexcept = 0;
    virtual void update(float dt)              noexcept = 0;
    virtual void draw(const mat4s& projection) noexcept = 0;
    virtual void resize(int width, int height) noexcept = 0;

    bool isLoaded() const noexcept;
    Type getType()  const noexcept;

protected:
    void setSpriteSizeInPixels(const struct Sprite2D& sprite, const vec2s newSize, Transform2D& transform) noexcept;

    class Game* m_game;
    bool        m_isLoaded;

private:
    const Type m_type;
};