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
        PICK_HOUSE,
        MISSION
    };

	Scene(class DuneII* game, const Type type) noexcept;
	virtual ~Scene();
    
    virtual bool load(std::string_view info)   noexcept;
    virtual void update(float dt)              noexcept;
    virtual void draw()                        noexcept;
    virtual void resize(int width, int height) noexcept;

    bool isLoaded() const noexcept;
    Type getType()  const noexcept;

protected:
    void setSpriteSizeInPixels(const Sprite& sprite, vec2 newSize, Transform2D& transform) noexcept;

    class DuneII* m_game;
    bool          m_isLoaded;

private:
    const Type m_type;
};

#endif // !SCENE_HPP