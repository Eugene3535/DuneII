#include <SFML/Graphics/Sprite.hpp>

#include "game/DuneII.hpp"
#include "scenes/choosing_houses/Destiny.hpp"


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
}


Destiny::Destiny(DuneII* game) noexcept:
    Scene(game),
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
            auto size = sf::Vector2f(m_game->window.getSize());
            set_sprite_size(m_sprite.get(), size);
            
            if(m_theme = m_game->getAssets().get<sf::Music>(CHOSEN_DESTINY_FLAC))
            {
                m_theme->setLooping(true);
                m_theme->play();
            }

            m_isLoaded = true;
        }
    }

    return m_isLoaded;
}


void Destiny::update(const sf::Time dt) noexcept
{
    static sf::Time delay;
    delay += dt;

    if(delay > sf::seconds(3))
    {
        m_theme->stop();
        m_status = { Scene::Type::MISSION, true };
    }
}


void Destiny::resize(const sf::Vector2f& size) noexcept
{
    Scene::resize(size);
    set_sprite_size(m_sprite.get(), size);
}


void Destiny::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isLoaded)
        target.draw(*m_sprite, states);
}