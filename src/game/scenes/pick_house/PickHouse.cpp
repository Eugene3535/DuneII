#include <SFML/Graphics/Sprite.hpp>

#include "common/Enums.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/pick_house/PickHouse.hpp"

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

    HouseType current_house = HouseType::Atreides;
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
                case HouseType::Atreides:
                    outlinePositionX = ATREIDES_OUTLINE_POSITION_X * dx;
                break;

                case HouseType::Ordos:
                    outlinePositionX = ORDOS_OUTLINE_POSITION_X * dx;
                break;

                case HouseType::Harkonnen:
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


PickHouse::PickHouse(DuneII* game) noexcept:
    Scene(game, Scene::PICK_HOUSE),
    m_outline({ DEFAULT_OUTLINE_SIZE_X, DEFAULT_OUTLINE_SIZE_Y }),
    m_theme(nullptr)
{

}


bool PickHouse::load(std::string_view data) noexcept
{
    if(!m_isLoaded)
    {
        auto& assets = m_game->assets;

        if(auto texture = assets.get<sf::Texture>(HOUSES_PNG))
        {
            m_sprite = std::make_unique<sf::Sprite>(*texture);        
            m_outline.setPosition({ ATREIDES_OUTLINE_POSITION_X, OUTLINE_POSITION_Y });
            m_outline.setFillColor(sf::Color::Transparent);
            m_outline.setOutlineColor(sf::Color::Red);
            m_outline.setOutlineThickness(1);
            
            if(m_theme = assets.get<sf::Music>(CHOSEN_DESTINY_FLAC))
            {
                m_theme->setLooping(true);
                m_theme->play();
            }

            resize(m_game->window.getSize());
            m_isLoaded = true;
        }
    }

    return m_isLoaded;
}


void PickHouse::update(const sf::Time dt) noexcept
{
    delay += dt;

    const bool isKeyLeftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    const bool isKeyRightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    bool outlineNeedUpdate = ( delay > sf::seconds(0.3f) && (isKeyLeftPressed || isKeyRightPressed) );

    switch (current_house)
    {
        case HouseType::Atreides:
        {
            if(isKeyRightPressed)
                current_house = HouseType::Ordos;      
        }
        break;

        case HouseType::Ordos:
        {
            if(isKeyLeftPressed)
                current_house = HouseType::Atreides;

            if(isKeyRightPressed)
                current_house = HouseType::Harkonnen;
        }
        break;

        case HouseType::Harkonnen:
        {
            if(isKeyLeftPressed)
                current_house = HouseType::Ordos;
        }
        break;
        
        default:
            break;
    }

    if(outlineNeedUpdate)
    {
        resize(m_game->window.getSize());
        delay = sf::Time::Zero;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
    {
        m_theme->stop();
        m_game->switchScene(this, Scene::MISSION);
    }
}


void PickHouse::resize(sf::Vector2u size) noexcept
{
    Scene::resize(size);
    set_sprite_size(m_sprite.get(), sf::Vector2f(size));
    set_outline_params(m_outline, m_sprite.get(), sf::Vector2f(size));
}


void PickHouse::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isLoaded)
    {
        target.setView(m_view);
        target.draw(*m_sprite, states);
        target.draw(m_outline, states);
    }    
}