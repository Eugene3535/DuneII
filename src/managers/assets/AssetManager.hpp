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

    static AssetManager* instance() noexcept;

    sf::Texture* getTexture(const std::string& filename) noexcept;
    sf::Font*    getFont(const std::string& filename)    noexcept;

private:
    template<class T>
    T* tryLoadFromFile(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept;

private:
    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, sf::Font>    m_fonts;

private:
    static AssetManager* m_instance;
};

#include "managers/assets/AssetManager.inl"

#define Assets AssetManager::instance()

#endif