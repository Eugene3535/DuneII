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

        if(auto texture = tryLoadFromFile<sf::Texture>(filename, m_textures); texture != nullptr)
            return texture;
    }

    return nullptr;
}

sf::Font* AssetManager::getFont(const std::string& filename) noexcept
{
    if (m_instance)
    {
        if(auto found = m_fonts.find(filename); found != m_fonts.end())
            return &found->second;

        if(auto font = tryLoadFromFile<sf::Font>(filename, m_fonts); font != nullptr)
            return font;
    }
    
    return nullptr;
}

sf::SoundBuffer* AssetManager::getSoundBuffer(const std::string& filename) noexcept
{
    if (m_instance)
    {
        if(auto found = m_soundBuffers.find(filename); found != m_soundBuffers.end())
            return &found->second;

        if(auto soundBuffer = tryLoadFromFile<sf::SoundBuffer>(filename, m_soundBuffers); soundBuffer != nullptr)
            return soundBuffer;
    }
    
    return nullptr;
}

sf::Music* AssetManager::getMusic(const std::string& filename) noexcept
{
    if (m_instance)
    {
        if(auto found = m_musicBox.find(filename); found != m_musicBox.end())
            return &found->second;
        
        if(auto music = tryLoadFromFile<sf::Music>(filename, m_musicBox); music != nullptr)
            return music;
    }

    return nullptr;
}