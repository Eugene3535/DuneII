#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Music.hpp>

#include "scenes/Scene.hpp"
#include "scenes/intro/interactive_elements/Button.hpp"

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

    sf::Sprite* m_space;
    sf::Sprite* m_planet;

    Button* m_play;
    Button* m_exit;
    Button* m_settings;

    sf::Text*  m_info;
    sf::Music* m_theme;

    char m_memoryPool[sizeof(sf::Sprite) * 2 + sizeof(Button) * 3 + sizeof(sf::Text)];

    bool m_isPresented;
};

#endif // !TITLE_SCREEN_HPP