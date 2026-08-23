#pragma once

#include <cstdint>
#include <string_view>
#include <functional>

#include <cglm/struct/vec2.h>

#include "graphics/transform/Transform2D.hpp"


class Scene
{
public:
	Scene(struct Game* game) noexcept;
	virtual ~Scene();
    
    virtual bool load(std::string_view info)   noexcept = 0;
    virtual void update(float dt)              noexcept = 0;
    virtual void draw(const mat4s& projection) noexcept = 0;
    virtual void resize(int width, int height) noexcept = 0;

    bool isLoaded() const noexcept;

protected:
    void setSpriteSizeInPixels(const struct Sprite2D& sprite, const vec2s newSize, Transform2D& transform) noexcept;

    struct Game* m_game;
    bool         m_isLoaded;
};