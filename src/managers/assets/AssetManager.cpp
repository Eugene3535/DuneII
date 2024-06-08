#include "managers/assets/AssetManager.hpp"

AssetManager* AssetManager::m_instance;

AssetManager::AssetManager() noexcept
{
    if (m_instance == nullptr)
        m_instance = this;
}

AssetManager* AssetManager::instance() noexcept
{
    return m_instance;
}

sf::Texture* AssetManager::getTexture(const std::string& filename) noexcept
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

sf::Font* AssetManager::getFont(const std::string& filename) noexcept
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