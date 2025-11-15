#ifndef SCENE_HPP
#define SCENE_HPP

#include <cstdint>
#include <string_view>

#include <glm/vec2.hpp>


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
    
    virtual bool load(std::string_view info)    noexcept;
    virtual void update(float dt)               noexcept;
    virtual void draw()                         noexcept;
    virtual void resize(const glm::ivec2& size) noexcept;
    virtual void click(int mouseButton)         noexcept;

    bool isLoaded() const noexcept;

protected:
    void setSpriteSizeInPixels(const class Sprite& sprite, const glm::vec2& newSize, class Transform2D& transform) noexcept;

    class DuneII* m_game;

    bool m_isLoaded;
};

#endif // !SCENE_HPP