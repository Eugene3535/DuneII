#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include "scenes/Scene.hpp"


namespace sf
{
    class Music;
    class Text;
    class Sprite;
}


class TitleScreen:
    public Scene
{
public:
    explicit TitleScreen(class DuneII* game) noexcept;
    ~TitleScreen();

    bool load(const std::string& info) noexcept override;
    void update(const sf::Time dt)     noexcept override;
    void resize(const sf::Vector2f& size) noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    class sf::Sprite* m_space;
    class sf::Sprite* m_planet;

    class Button* m_play;
    class Button* m_exit;
    class Button* m_settings;

    class sf::Text*  m_info;
    class sf::Music* m_theme;

    void* m_memoryPool;

    bool m_isPresented;
};

#endif // !TITLE_SCREEN_HPP