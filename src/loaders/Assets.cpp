#include "loaders/Assets.hpp"

Assets* Assets::m_instance;

Assets::Assets() noexcept
{
    if (m_instance == nullptr)
        m_instance = this;
}

Assets* Assets::instance() noexcept
{
    return m_instance;
}

sf::Texture* Assets::getTexture(const std::string& filename) noexcept
{
    if (m_instance)
    {
        if(auto found = m_textures.find(filename); found != m_textures.end())
            return &found->second;

        if(auto new_texture = tryLoadFromFile<sf::Texture>(filename, m_textures); new_texture != nullptr)
            return new_texture;
    }

    return nullptr;
}

sf::Font* Assets::getFont(const std::string& filename) noexcept
{
    if (m_instance)
    {
        if(auto found = m_fonts.find(filename); found != m_fonts.end())
            return &found->second;

        if(auto new_font = tryLoadFromFile<sf::Font>(filename, m_fonts); new_font != nullptr)
            return new_font;
    }
    
    return nullptr;
}