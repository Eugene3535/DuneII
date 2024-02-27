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
	Scene() noexcept;
	virtual ~Scene();
    
    virtual bool load(const std::string& info) noexcept = 0;
    virtual void open()                        noexcept = 0;
    virtual void close()                       noexcept = 0;
    virtual void update(sf::Time dt)           noexcept = 0;

    void setCursorPosition(const sf::Vector2i& point) noexcept;

    sf::View&    getViewport()              noexcept;
    std::int32_t getLoadingProgress() const noexcept;
    bool         isLoaded()           const noexcept;

protected: // utils
    void         setSpriteSize(sf::Sprite& sprite, int width, int height);
    void         setSpriteSize(sf::Sprite& sprite, const sf::Vector2i& size);
    sf::Vector2i getSpriteSize(const sf::Sprite& sprite);

protected:
    sf::View     m_viewport;
    sf::Vector2i m_cursorPosition;
    std::int32_t m_loadingProgress;
    bool         m_isLoaded;
};

#endif // !SCENE_HPP