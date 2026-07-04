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

	Scene(class Game* game, const Type type) noexcept;
	virtual ~Scene();
    
    virtual bool load(std::string_view info)   noexcept;
    virtual void update(float dt)              noexcept;
    virtual void draw(const mat4s& projection) noexcept;
    virtual void resize(int width, int height) noexcept;

    void setCursor(float x, float y) noexcept;
    void setMouse(int button, int action, int mods) noexcept;
    void setKeyboard(int key, int scancode, int action, int mods) noexcept;

    bool isLoaded() const noexcept;
    Type getType()  const noexcept;

protected:
    void setSpriteSizeInPixels(const struct Sprite2D& sprite, const vec2s newSize, Transform2D& transform) noexcept;

    class Game* m_game;
    bool        m_isLoaded;

    ivec2s m_size;
    vec2s m_cursor;

    struct
    {
        int button;
        int action;
        int mods;
    } m_mouse;

    struct
    {
        int key;
        int scancode;
        int action;
        int mods;
    } m_keyboard;

private:
    const Type m_type;
};