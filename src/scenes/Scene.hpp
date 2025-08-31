#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>


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
    virtual void resize(const sf::Vector2f& size) noexcept;

    bool isLoaded() const noexcept;
    std::pair<Type, bool> getStatus() const noexcept;

    const sf::View& getView() const noexcept;

protected:
    class DuneII* m_game;

    bool m_isLoaded;

    sf::View m_view;
};

#endif // !SCENE_HPP