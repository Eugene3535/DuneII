#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <unordered_map>
#include <type_traits>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "utils/FileProvider.hpp"

class AssetManager final:
    private sf::NonCopyable
{
public:
    AssetManager() noexcept;

    template<class T>
    static T* create(const std::string& filename) noexcept;

    template<class T>
    static T* get(const std::string& filename) noexcept;

    template<class T>
    static void remove(const std::string& filename) noexcept;

private:
    template<class T>
    T* tryLoadFromFile(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept;

    template<class T>
    T* tryCreate(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept;

private:
    std::unordered_map<std::string, sf::Texture>     m_textures;
    std::unordered_map<std::string, sf::Font>        m_fonts;
    std::unordered_map<std::string, sf::SoundBuffer> m_sounds;
    std::unordered_map<std::string, sf::Music>       m_musics;
    std::unordered_map<std::string, sf::Shader>      m_shaders;

private:
    static AssetManager* m_instance;
};

#include "managers/AssetManager.inl"

#endif