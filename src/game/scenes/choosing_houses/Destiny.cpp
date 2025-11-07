#include <SFML/Graphics/Sprite.hpp>

#include "game/DuneII.hpp"
#include "common/Enums.hpp"
#include "scenes/choosing_houses/Destiny.hpp"

#define ATREIDES_OUTLINE_POSITION_X 32.f
#define ORDOS_OUTLINE_POSITION_X 120.f
#define HARKONNEN_OUTLINE_POSITION_X 208.f
#define OUTLINE_POSITION_Y 136.f

#define DEFAULT_OUTLINE_SIZE_X 80.f
#define DEFAULT_OUTLINE_SIZE_Y 16.f

namespace
{// set scale in pixels
    void set_sprite_size(sf::Sprite* sprite, const sf::Vector2f& newSize) noexcept
    {
        if (const auto& rect = sprite->getTextureRect(); rect.size.x > 0 && rect.size.y > 0)
        {
            float dx = newSize.x / rect.size.x;
            float dy = newSize.y / rect.size.y;
            sprite->setScale({dx, dy});
        }
    }

    HouseType current_house = HouseType::ATREIDES;
    sf::Time delay;

    void set_outline_params(sf::RectangleShape& shape, const sf::Sprite* sprite, const sf::Vector2f& newSize) noexcept
    {
        if (const auto& rect = sprite->getTextureRect(); rect.size.x > 0 && rect.size.y > 0)
        {
            float dx = newSize.x / rect.size.x;
            float dy = newSize.y / rect.size.y;

            float outlinePositionX = 0;

            switch (current_house)
            {
                case HouseType::ATREIDES:
                    outlinePositionX = ATREIDES_OUTLINE_POSITION_X * dx;
                break;

                case HouseType::ORDOS:
                    outlinePositionX = ORDOS_OUTLINE_POSITION_X * dx;
                break;

                case HouseType::HARKONNEN:
                    outlinePositionX = HARKONNEN_OUTLINE_POSITION_X * dx;
                break;
                
                default:
                    break;
            }

            shape.setPosition({ outlinePositionX, OUTLINE_POSITION_Y * dy });
            shape.setScale({dx, dy});
        }
    }
}


Destiny::Destiny(DuneII* game) noexcept:
    Scene(game),
    m_outline(),
    m_theme(nullptr)
{

}


bool Destiny::load(const std::string& info) noexcept
{
    if(!m_isLoaded)
    {
        auto& assets = m_game->getAssets();

        if(auto texture = assets.get<sf::Texture>(HOUSES_PNG))
        {
            m_sprite = std::make_unique<sf::Sprite>(*texture);        

            m_outline.setSize({ DEFAULT_OUTLINE_SIZE_X, DEFAULT_OUTLINE_SIZE_Y });
            m_outline.setPosition({ ATREIDES_OUTLINE_POSITION_X, OUTLINE_POSITION_Y });
            m_outline.setFillColor(sf::Color::Transparent);
            m_outline.setOutlineColor(sf::Color::Red);
            m_outline.setOutlineThickness(1);
            
            if(m_theme = m_game->getAssets().get<sf::Music>(CHOSEN_DESTINY_FLAC))
            {
                m_theme->setLooping(true);
                m_theme->play();
            }

            const auto size = sf::Vector2f(m_game->window.getSize());
            resize(size);

            m_isLoaded = true;
        }
    }

    return m_isLoaded;
}


void Destiny::update(const sf::Time dt) noexcept
{
    delay += dt;

    const bool isKeyLeftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    const bool isKeyRightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    bool outlineNeedUpdate = ( delay > sf::seconds(0.3f) && (isKeyLeftPressed || isKeyRightPressed) );

    switch (current_house)
    {
        case HouseType::ATREIDES:

            if(isKeyRightPressed)
                current_house = HouseType::ORDOS;      

        break;

        case HouseType::ORDOS:

            if(isKeyLeftPressed)
                current_house = HouseType::ATREIDES;

            if(isKeyRightPressed)
                current_house = HouseType::HARKONNEN;

        break;

        case HouseType::HARKONNEN:
            if(isKeyLeftPressed)
                current_house = HouseType::ORDOS;

        break;
        
        default:
            break;
    }

    if(outlineNeedUpdate)
    {
        const auto size = sf::Vector2f(m_game->window.getSize());
        resize(size);
        delay = sf::Time::Zero;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
    {
        m_theme->stop();
        m_status = { Scene::Type::MISSION, true };
    }
}


void Destiny::resize(const sf::Vector2f& size) noexcept
{
    Scene::resize(size);

    set_sprite_size(m_sprite.get(), size);
    set_outline_params(m_outline, m_sprite.get(), size);
}


void Destiny::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isLoaded)
    {
        target.draw(*m_sprite, states);
        target.draw(m_outline, states);
    }    
}