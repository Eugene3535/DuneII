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
        PROCESSING = 0,
        MAIN_MENU,
        CHOOSE_DESTINY,
        MISSION
    };

    using Status = std::pair<Type, bool>;

	Scene(class DuneII* game) noexcept;
	virtual ~Scene();
    
    virtual bool load(const std::string& info) noexcept;
    virtual void update(const sf::Time dt)     noexcept;
    virtual void resize(const sf::Vector2f& size) noexcept;

    bool isLoaded() const noexcept;

    Status getStatus() const noexcept;
    void reset() noexcept;

    const sf::View& getView() const noexcept;

protected:
    class DuneII* m_game;

    bool m_isLoaded;

    sf::View m_view;

    std::pair<Type, bool> m_status;
};

#endif // !SCENE_HPP