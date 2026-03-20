#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include <memory>

#include "game/scenes/Scene.hpp"


namespace sf
{
    class Text;
    class Music;
}

class TitleScreen:
    public Scene
{
public:
    explicit TitleScreen(class DuneII* game) noexcept;
    ~TitleScreen();

    bool load(std::string_view data) noexcept override;
    void update(sf::Time dt)         noexcept override;
    void resize(sf::Vector2u size)   noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::unique_ptr<sf::Sprite> m_space;
    std::unique_ptr<sf::Sprite> m_planet;

    std::unique_ptr<class Button> m_play;
    std::unique_ptr<class Button> m_exit;
    std::unique_ptr<class Button> m_settings;

    std::unique_ptr<class sf::Text> m_info;
    class sf::Music* m_theme;

    bool m_isPresented;
};

#endif // !TITLE_SCREEN_HPP