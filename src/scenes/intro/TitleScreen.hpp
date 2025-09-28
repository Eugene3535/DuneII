#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include "scenes/Scene.hpp"
#include "scenes/intro/Button.hpp"

namespace sf
{
    class Music;
    class Text;
}


class TitleScreen:
    public Scene
{
public:
    TitleScreen(class DuneII* game) noexcept;
    ~TitleScreen();

    bool load(const std::string& info) noexcept override;
    void update(const sf::Time dt)     noexcept override;
    void resize(const sf::Vector2f& size) noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite* m_space;
    sf::Sprite* m_planet;
    Button*     m_play;
    Button*     m_exit;
    Button*     m_settings;

    class sf::Text*  m_info;
    class sf::Music* m_theme;

    void* m_memoryPool;

    bool m_isPresented;
};

#endif // !TITLE_SCREEN_HPP