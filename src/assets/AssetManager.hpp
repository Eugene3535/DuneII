#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <unordered_map>
#include <type_traits>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

#include "common/FileProvider.hpp"

class AssetManager final:
    private sf::NonCopyable
{
public:
    AssetManager() noexcept;

    static AssetManager* instance() noexcept;

    template<class T>
    T* getResource(const std::string& filename) noexcept;

private:
    template<class T>
    T* tryLoadFromFile(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept;

private:
    std::unordered_map<std::string, sf::Texture>     m_textures;
    std::unordered_map<std::string, sf::Font>        m_fonts;
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Music>       m_musicBox;

private:
    static AssetManager* m_instance;
};

#include "assets/AssetManager.inl"

#define Assets AssetManager::instance()

#endif