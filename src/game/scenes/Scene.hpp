#ifndef SCENE_HPP
#define SCENE_HPP

#include <cstdint>
#include <string_view>

#include <cglm/call/vec2.h>

#include "graphics/Meshes.hpp"
#include "graphics/transform/Transform2D.hpp"

class Scene
{
public:
    enum Type : uint32_t
    {
        NONE = 0,
        MAIN_MENU,
        CHOOSE_DESTINY,
        MISSION
    };

	Scene(class DuneII* game) noexcept;
	virtual ~Scene();
    
    virtual bool load(std::string_view info)         noexcept;
    virtual void update(float dt)                    noexcept;
    virtual void draw()                              noexcept;
    virtual void resize(int width, int height)       noexcept;
    virtual void click(int mouseButton)              noexcept;
    virtual void setCursorPosition(float x, float y) noexcept;

    bool isLoaded() const noexcept;

protected:
    void setSpriteSizeInPixels(const Sprite& sprite, vec2 newSize, Transform2D& transform) noexcept;

    class DuneII* m_game;

    bool m_isLoaded;
};

#endif // !SCENE_HPP