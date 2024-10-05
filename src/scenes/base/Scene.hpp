#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Scene:
    public sf::Drawable,
    private sf::NonCopyable
{
public:
	Scene(struct Game& game) noexcept;
	virtual ~Scene();
    
    virtual bool load(const std::string& info) noexcept;
    virtual void update(sf::Time dt)           noexcept;

    bool isLoaded() const noexcept;

protected:
    struct Game& m_game;

protected:
    bool m_isLoaded;
};

#endif // !SCENE_HPP