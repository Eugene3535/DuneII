#include <SFML/Graphics/Text.hpp>

#include "game/DuneII.hpp"
#include "scenes/intro/interactive_elements/Button.hpp"
#include "scenes/choosing_houses/Destiny.hpp"
#include "scenes/intro/TitleScreen.hpp"


// Sprite scaling and position factors when changing window size
#define PLANET_SCALE_FACTOR_WIDTH 2.f
#define PLANET_SCALE_FACTOR_HEIGHT 1.5f
#define PLANET_POSITION_FACTOR_X 1.3f
#define PLANET_POSITION_FACTOR_Y 3.f

#define BUTTON_SCALE_FACTOR_WIDTH 10.f
#define BUTTON_SCALE_FACTOR_HEIGHT 15.f
#define BUTTON_POSITION_FACTOR_X 2.f
#define BUTTON_POSITION_FACTOR_Y 1.2f

#define TEXT_INFO_SCALE_FACTOR_WIDTH 3.f
#define TEXT_INFO_SCALE_FACTOR_HEIGHT 20.f

#define INTRO_TEXT_PRESENTATION "Какие-то компании представляют"

namespace
{// set size in pixels
    void set_sprite_size(sf::Sprite* sprite, const sf::Vector2f& newSize) noexcept
    {
        if (const auto& rect = sprite->getTextureRect(); rect.size.x > 0 && rect.size.y > 0)
        {
            float dx = newSize.x / rect.size.x;
            float dy = newSize.y / rect.size.y;
            sprite->setScale({dx, dy});
        }
    }

    void set_text_size(sf::Text* text, const sf::Vector2f& newSize) noexcept
    {
        if (auto rect = text->getLocalBounds(); rect.size.x > 0.f && rect.size.y > 0.f)
        {
            float dx = newSize.x / rect.size.x;
            float dy = newSize.y / rect.size.y;
            text->setScale({dx, dy});
        }
    }

//  Stages
    constexpr sf::Time intro_text_present_start_time   = sf::seconds(5);
    constexpr sf::Time intro_text_present_end_time     = sf::seconds(15);
    constexpr sf::Time intro_title_screen_present_time = sf::seconds(20);

    bool is_moving_planet_begin;
    bool is_moving_planet_end;
    bool is_show_planet;
    bool is_intro_active_phase_end;
}


TitleScreen::TitleScreen(DuneII* game) noexcept:
    Scene(game),
    m_isPresented(false),
    m_space(nullptr),
    m_planet(nullptr),
    m_play(nullptr),
    m_exit(nullptr),
    m_settings(nullptr),
    m_info(nullptr),
    m_theme(nullptr),
    m_memoryPool(nullptr)
{
#ifdef DEBUG
    m_isPresented = true;
#endif
}


TitleScreen::~TitleScreen()
{
    if(m_memoryPool)
    {
        m_info->~Text();
        free(m_memoryPool);
    }   
}


bool TitleScreen::load(const std::string& info) noexcept
{
    if(!m_isLoaded)
    {
        auto& assets = m_game->getAssets();
        auto spaceTexture    = assets.get<sf::Texture>(SPACE_JPG);
        auto duneTexture     = assets.get<sf::Texture>(DINE_PNG);
        auto playTexture     = assets.get<sf::Texture>(BUTTON_PLAY_PNG);
        auto exitTexture     = assets.get<sf::Texture>(BUTTON_EXIT_RU_PNG);
        auto settingsTexture = assets.get<sf::Texture>(BUTTON_SETTINGS_RU_PNG);
        auto font            = assets.get<sf::Font>(AVANTENRBOOK_TTF);
        m_theme              = assets.get<sf::Music>(OPENING_FLAC);

        if(!(spaceTexture && duneTexture && playTexture && exitTexture && settingsTexture && font && m_theme))
            return false;

        if(m_memoryPool = malloc(sizeof(sf::Sprite) * 2 + sizeof(Button) * 3 + sizeof(sf::Text)))
        {
            char* offset = static_cast<char*>(m_memoryPool);
            m_space = new (offset) sf::Sprite(*spaceTexture);
            offset += sizeof(sf::Sprite);

            m_planet = new (offset) sf::Sprite(*duneTexture);
            offset += sizeof(sf::Sprite);
            m_planet->setOrigin(sf::Vector2f(duneTexture->getSize() / 2u));

            m_settings = new (offset) Button(settingsTexture); offset += sizeof(Button);
            m_play     = new (offset) Button(playTexture);     offset += sizeof(Button);
            m_exit     = new (offset) Button(exitTexture);     offset += sizeof(Button);

            m_exit->getSprite().setColor(sf::Color(0, 0, 0, 0));
            m_settings->getSprite().setColor(sf::Color(0, 0, 0, 0));
            m_play->getSprite().setColor(sf::Color(0, 0, 0, 0));

            std::string info(INTRO_TEXT_PRESENTATION);
            m_info = new (offset) sf::Text(*font, sf::String::fromUtf8(info.begin(), info.end()));
            m_info->setFillColor({0, 0, 0, 0});
            
            m_isLoaded = true;

//  IMPORTANT: Apply all transformations AFTER scene is loaded (is presented - later)
            const auto screenSize = sf::Vector2f(m_game->window.getSize());
            resize(screenSize);
            m_planet->setPosition({ screenSize.x * PLANET_POSITION_FACTOR_X, screenSize.y / PLANET_POSITION_FACTOR_Y });
#ifdef DEBUG
            m_planet->setPosition({ screenSize.x / PLANET_POSITION_FACTOR_X, screenSize.y / PLANET_POSITION_FACTOR_Y });
#endif
            m_theme->play();
        }
    }

    return m_isLoaded;
}


void TitleScreen::update(const sf::Time dt) noexcept
{
    if(m_isLoaded)
    {
        if(m_isPresented)
        {
            const sf::Vector2f mousePosition { sf::Mouse::getPosition(m_game->window) };
            const bool isClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

            m_settings->update(mousePosition, isClicked);
            m_play->update(mousePosition, isClicked);
            m_exit->update(mousePosition, isClicked);

            if(m_play->isPressed())
            {
                m_theme->stop();
                m_status = { Scene::Type::CHOOSE_DESTINY, true };
            }
        }
        else
        {
            auto offset = m_theme->getPlayingOffset();
            bool isPresentTextTime = (offset > intro_text_present_start_time && offset < intro_text_present_end_time);
            bool isPresentPlanetTime = (offset > intro_text_present_end_time);
            
            if(isPresentTextTime)
            {
                if(auto color = m_info->getFillColor(); color.a < (uint8_t)255)
                {
                    ++color.r;
                    ++color.g;
                    ++color.b;
                    ++color.a;
                    m_info->setFillColor(color);
                }
            }

            if(isPresentPlanetTime)
            {
                if(auto color = m_info->getFillColor(); color.a > (uint8_t)10)
                {
                    color.r -= 5;
                    color.g -= 5;
                    color.b -= 5;
                    color.a -= 5;
                    m_info->setFillColor(color);
                }
                else 
                {
                    m_info->setFillColor(sf::Color(0, 0, 0, 0));
                    is_moving_planet_begin = true;
                    is_show_planet = true;
                }

                if(!is_moving_planet_end)
                {
                    const auto screenSize = sf::Vector2f(m_game->window.getSize());
                    const auto targetPosition = screenSize.x / PLANET_POSITION_FACTOR_X;
                    
                    if(m_planet->getPosition().x > targetPosition)
                        m_planet->move({-2, 0});
                    else
                    {
                        is_moving_planet_begin = false;
                        is_moving_planet_end = true;
                        resize(screenSize);
                    }
                        
                }

                if(is_moving_planet_end)
                {
                    is_intro_active_phase_end = true;

                    constexpr sf::Color normalColor = { 150, 150, 150, 150 };
                    sf::Color currentColor = m_play->getSprite().getColor();

                    if(currentColor != normalColor)
                    {
                        ++currentColor.r;
                        ++currentColor.g;
                        ++currentColor.b;
                        ++currentColor.a;

                        m_exit->getSprite().setColor(currentColor);
                        m_settings->getSprite().setColor(currentColor);
                        m_play->getSprite().setColor(currentColor);
                    }
                    else m_isPresented = true;
                }
            }
        }
    }
}


void TitleScreen::resize(const sf::Vector2f& size) noexcept
{
    Scene::resize(size);

    if(m_isPresented)
    {
        {// space and planet
            set_sprite_size(m_space, size);
            set_sprite_size(m_planet, { size.x / PLANET_SCALE_FACTOR_WIDTH, size.y / PLANET_SCALE_FACTOR_HEIGHT });
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
    else
    {
        {// space and planet
            set_sprite_size(m_space, size);
            set_sprite_size(m_planet, { size.x / PLANET_SCALE_FACTOR_WIDTH, size.y / PLANET_SCALE_FACTOR_HEIGHT });

            if(is_moving_planet_begin)
            {
                auto targetPositionX = size.x / PLANET_POSITION_FACTOR_X;
                auto currentPositionX = m_planet->getPosition().x / PLANET_POSITION_FACTOR_X;
                auto difference = currentPositionX - targetPositionX;
                m_planet->setPosition({ targetPositionX + difference, size.y / PLANET_POSITION_FACTOR_Y });
            }

            if(is_moving_planet_end)
            {
                m_planet->setPosition({ size.x / PLANET_POSITION_FACTOR_X, size.y / PLANET_POSITION_FACTOR_Y });
            }
        }
          
        {// text info
            const auto center = size * 0.5f;
            auto textBounds = m_info->getLocalBounds();
            m_info->setOrigin(textBounds.size * 0.5f);
            m_info->setPosition(center);

            const float width = size.x / TEXT_INFO_SCALE_FACTOR_WIDTH;
            const float height = size.y / TEXT_INFO_SCALE_FACTOR_HEIGHT;
            set_text_size(m_info, { width, height });
        }

        if(is_moving_planet_end)
        {
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
}


void TitleScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isLoaded)
    {
        states.blendMode = sf::BlendAlpha;
        target.draw(*m_space, states);

        if(m_isPresented)
        {
            target.draw(*m_planet, states);
            target.draw(*m_exit, states);
            target.draw(*m_settings, states);
            target.draw(*m_play, states);
        }
        else
        {
            target.draw(*m_info, states);

            if(is_show_planet)
                target.draw(*m_planet, states);

            if(is_intro_active_phase_end)
            {
                target.draw(*m_exit, states);
                target.draw(*m_settings, states);
                target.draw(*m_play, states);
            }
        }
    }
}