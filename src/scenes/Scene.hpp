#ifndef SCENE_HPP
#define SCENE_HPP

#include <cstdint>
#include <string>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

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