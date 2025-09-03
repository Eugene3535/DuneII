#include "game/DuneII.hpp"
#include "scenes/intro/TitleScreen.hpp"


#define SPACE_TEXTURE_FILE              "space.jpg"
#define DINE_TEXTURE_FILE               "dune.png"
#define BUTTON_PLAY_TEXTURE_FILE        "button_play.png"
#define BUTTON_EXIT_RU_TEXTURE_FILE     "button_exit_ru.png"
#define BUTTON_SETTINGS_RU_TEXTURE_FILE "button_settings_ru.png"


namespace
{// set size in pixels
    void set_sprite_size(sf::Sprite& sprite, const sf::Vector2f& newSize) noexcept
    {
        if (auto& rect = sprite.getTextureRect(); rect.size.x > 0 && rect.size.y > 0)
        {
            float dx = newSize.x / rect.size.x;
            float dy = newSize.y / rect.size.y;
            sprite.setScale({dx, dy});
        }
    }
}


TitleScreen::TitleScreen(DuneII* game) noexcept:
    Scene(game),
    m_isPresented(false)
{

}


bool TitleScreen::load(const std::string& info) noexcept
{
    if(!m_isLoaded)
    {
        auto spaceTexture    = m_game->getAssets().get<sf::Texture>(SPACE_TEXTURE_FILE);
        auto duneTexture     = m_game->getAssets().get<sf::Texture>(DINE_TEXTURE_FILE);
        auto playTexture     = m_game->getAssets().get<sf::Texture>(BUTTON_PLAY_TEXTURE_FILE);
        auto exitTexture     = m_game->getAssets().get<sf::Texture>(BUTTON_EXIT_RU_TEXTURE_FILE);
        auto settingsTexture = m_game->getAssets().get<sf::Texture>(BUTTON_SETTINGS_RU_TEXTURE_FILE);

        if(!(spaceTexture && duneTexture && playTexture && exitTexture && settingsTexture))
            return false;

        m_space  = std::make_unique<sf::Sprite>(*spaceTexture);
        m_planet = std::make_unique<sf::Sprite>(*duneTexture);
        m_planet->setPosition({500, 0});

        m_settings = std::make_unique<Button>(settingsTexture, sf::IntRect({128, 600}, {128, 64}));
        m_play     = std::make_unique<Button>(playTexture, sf::IntRect({256, 600}, {128, 128}));
        m_exit     = std::make_unique<Button>(exitTexture, sf::IntRect({384, 600}, {128, 64}));
        
        const auto screenSize = m_game->window.getSize();
        resize(sf::Vector2f(screenSize));

        m_isLoaded = true;
    }

    return m_isLoaded;
}


void TitleScreen::update(const sf::Time dt) noexcept
{
    if( ! m_isPresented )
    {
        // show moving across the stars
    }
}


void TitleScreen::resize(const sf::Vector2f& size) noexcept
{
    set_sprite_size(*m_space, size);
    Scene::resize(size);
}


void TitleScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.blendMode = sf::BlendAlpha;
    target.draw(*m_space, states);

    if(true) // m_isPresented
    {
        target.draw(*m_planet, states);
        target.draw(*m_exit, states);
        target.draw(*m_settings, states);
        target.draw(*m_play, states);
    }
}