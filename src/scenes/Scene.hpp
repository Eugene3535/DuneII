#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>


class Scene:
    public sf::Drawable
{
public:
    enum Type: uint32_t
    {
        NONE,
        MAIN_MENU,
        MISSION
    };

	Scene(class DuneII* game) noexcept;
	virtual ~Scene();
    
    virtual bool load(const std::string& info) noexcept;
    virtual void update(const sf::Time dt)     noexcept;
    virtual sf::Vector2i resize(const sf::Vector2u& size) noexcept;

    bool isLoaded() const noexcept;

    std::pair<Type, bool> getStatus() const noexcept;

protected:
    class DuneII* m_game;

protected:
    bool m_isLoaded;
};

#endif // !SCENE_HPP