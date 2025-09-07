#include "game/DuneII.hpp"
#include "scenes/intro/TitleScreen.hpp"


#define SPACE_TEXTURE_FILE              "space.jpg"
#define DINE_TEXTURE_FILE               "dune.png"
#define BUTTON_PLAY_TEXTURE_FILE        "button_play.png"
#define BUTTON_EXIT_RU_TEXTURE_FILE     "button_exit_ru.png"
#define BUTTON_SETTINGS_RU_TEXTURE_FILE "button_settings_ru.png"


// Sprite scaling and position factors when changing window size
#define PLANET_SCALE_FACTOR_WIDTH 2.f
#define PLANET_SCALE_FACTOR_HEIGHT 1.5f
#define PLANET_POSITION_FACTOR_X 1.3f
#define PLANET_POSITION_FACTOR_Y 3.f

#define BUTTON_SCALE_FACTOR_WIDTH 10.f
#define BUTTON_SCALE_FACTOR_HEIGHT 15.f
#define BUTTON_POSITION_FACTOR_X 2.f
#define BUTTON_POSITION_FACTOR_Y 1.2f

namespace
{// set size in pixels
    void set_sprite_size(sf::Sprite* sprite, const sf::Vector2f& newSize) noexcept
    {
        if (auto& rect = sprite->getTextureRect(); rect.size.x > 0 && rect.size.y > 0)
        {
            float dx = newSize.x / rect.size.x;
            float dy = newSize.y / rect.size.y;
            sprite->setScale({dx, dy});
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
        auto& assets = m_game->getAssets();
        auto spaceTexture    = assets.get<sf::Texture>(SPACE_TEXTURE_FILE);
        auto duneTexture     = assets.get<sf::Texture>(DINE_TEXTURE_FILE);
        auto playTexture     = assets.get<sf::Texture>(BUTTON_PLAY_TEXTURE_FILE);
        auto exitTexture     = assets.get<sf::Texture>(BUTTON_EXIT_RU_TEXTURE_FILE);
        auto settingsTexture = assets.get<sf::Texture>(BUTTON_SETTINGS_RU_TEXTURE_FILE);

        if(!(spaceTexture && duneTexture && playTexture && exitTexture && settingsTexture))
            return false;

        const auto windowSize = m_game->window.getSize();
        
        m_space = std::make_unique<sf::Sprite>(*spaceTexture);

        m_planet = std::make_unique<sf::Sprite>(*duneTexture);
        m_planet->setOrigin(sf::Vector2f(duneTexture->getSize() / 2u));

        m_settings = std::make_unique<Button>(settingsTexture);
        m_play     = std::make_unique<Button>(playTexture);
        m_exit     = std::make_unique<Button>(exitTexture);
        
        m_isLoaded = true;

//      IMPORTANT: Apply all transformations AFTER scene is loaded (is presented - later)
        resize(sf::Vector2f(windowSize));
    }

    return m_isLoaded;
}


void TitleScreen::update(const sf::Time dt) noexcept
{
    // if( ! m_isPresented )
    // {
    //     show moving across the stars
    // }

    const sf::Vector2f mousePosition { sf::Mouse::getPosition(m_game->window) };
    const bool isClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    m_settings->update(mousePosition, isClicked);
    m_play->update(mousePosition, isClicked);
    m_exit->update(mousePosition, isClicked);

    if(m_play->isPressed())
    {
        m_game->notifyChangeScene(this, DuneII::GameScene::MISSION);
    }
}


void TitleScreen::resize(const sf::Vector2f& size) noexcept
{
    Scene::resize(size);

    if(m_isLoaded)
    {
        {// space and planet
            set_sprite_size(m_space.get(), size);
            set_sprite_size(m_planet.get(), { size.x / PLANET_SCALE_FACTOR_WIDTH, size.y / PLANET_SCALE_FACTOR_HEIGHT });
            m_planet->setPosition({ size.x / PLANET_POSITION_FACTOR_X, size.y / PLANET_POSITION_FACTOR_Y });
        }

        {// buttons (size)
            const float width = size.x / BUTTON_SCALE_FACTOR_WIDTH;
            const float height = size.y / BUTTON_SCALE_FACTOR_HEIGHT;

            m_settings->setSize({ width, height });
            m_play->setSize({ width, width }); // <- this is not a typo
            m_exit->setSize({ width, height });
        }

        {// buttons (positions)
            const float centerX = size.x / BUTTON_POSITION_FACTOR_X;
            const float centerY = size.y / BUTTON_POSITION_FACTOR_Y;
            const float offset = size.x / BUTTON_SCALE_FACTOR_WIDTH * PLANET_POSITION_FACTOR_X;
  
            m_settings->setPosition({ centerX - offset, centerY });
            m_play->setPosition({ centerX, centerY });
            m_exit->setPosition({ centerX + offset, centerY });
        }
    }
}


void TitleScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isLoaded)
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
}