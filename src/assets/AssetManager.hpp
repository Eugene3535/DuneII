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

// texture files
#define STRUCTURES_TEXTURE_FILE_NAME "Structures.png"
#define FLAGS_TEXTURE_FILE_NAME      "Flags.png"

// music files
#define OPENING_MUSIC_FILE_NAME         "01-Opening.flac"
#define CYRILS_COUNCIL_MUSIC_FILE_NAME  "02-Cyril's-Council.flac"
#define AMMONS_ADVICE_MUSIC_FILE_NAME   "03-Ammon's-Advice.flac"
#define RADNORS_SCHEME_MUSIC_FILE_NAME  "04-Radnor's-Scheme.flac"
#define LEGO_TUNE_MUSIC_FILE_NAME       "05-The-Lego-Tune.flac"
#define TURBULENCE_MUSIC_FILE_NAME      "06-Turbulence.flac"
#define SPICE_TRIP_MUSIC_FILE_NAME      "07-Spice-Trip.flac"
#define COMMAND_POST_MUSIC_FILE_NAME    "08-Command-Post.flac"
#define TRENCHING_MUSIC_FILE_NAME       "09-Trenching.flac"
#define STARPORT_MUSIC_FILE_NAME        "10-Starport.flac"
#define EVASIVE_ACTION_MUSIC_FILE_NAME  "11-Evasive-Action.flac"
#define CHOSEN_DESTINY_MUSIC_FILE_NAME  "12-Chosen-Destiny.flac"
#define CONQUEST_MUSIC_FILE_NAME        "13-Conquest.flac"
#define SLITHERIN_MUSIC_FILE_NAME       "14-Slitherin.flac"
#define HARKONNEN_RULES_MUSIC_FILE_NAME "15-Harkonnen-Rules.flac"
#define ATREDIES_DIRGE_MUSIC_FILE_NAME  "16-Atredies-Dirge.flac"
#define ORDOS_DIRGE_MUSIC_FILE_NAME     "17-Ordos-Dirge.flac"
#define FINALE_MUSIC_FILE_NAME          "19-Finale.flac"
#define CREDIT_ROLL_MUSIC_FILE_NAME     "20-Credit-Roll.flac"


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