#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <unordered_map>
#include <type_traits>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

#include "common/FileProvider.hpp"

// Fonts
#define AVANTENRBOOK_TTF "AvanteNrBook.ttf"

// Images
#define STRUCTURES_PNG           "Structures.png"
#define FLAGS_PNG                "Flags.png"
#define CROSSHAIRS_TILESHEET_PNG "crosshairs_tilesheet.png"
#define SPACE_JPG                "space.jpg"
#define DINE_PNG                 "dune.png"
#define BUTTON_PLAY_PNG          "button_play.png"
#define BUTTON_EXIT_RU_PNG       "button_exit_ru.png"
#define BUTTON_SETTINGS_RU_PNG   "button_settings_ru.png"
#define HOUSES_PNG               "Houses.png"

// Animation xml
#define CURSOR_FRAME_XML "CursorFrames.xml"

// Music files
#define OPENING_FLAC         "01-Opening.flac"
#define CYRILS_COUNCIL_FLAC  "02-Cyril's-Council.flac"
#define AMMONS_ADVICE_FLAC   "03-Ammon's-Advice.flac"
#define RADNORS_SCHEME_FLAC  "04-Radnor's-Scheme.flac"
#define LEGO_TUNE_FLAC       "05-The-Lego-Tune.flac"
#define TURBULENCE_FLAC      "06-Turbulence.flac"
#define SPICE_TRIP_FLAC      "07-Spice-Trip.flac"
#define COMMAND_POST_FLAC    "08-Command-Post.flac"
#define TRENCHING_FLAC       "09-Trenching.flac"
#define STARPORT_FLAC        "10-Starport.flac"
#define EVASIVE_ACTION_FLAC  "11-Evasive-Action.flac"
#define CHOSEN_DESTINY_FLAC  "12-Chosen-Destiny.flac"
#define CONQUEST_FLAC        "13-Conquest.flac"
#define SLITHERIN_FLAC       "14-Slitherin.flac"
#define HARKONNEN_RULES_FLAC "15-Harkonnen-Rules.flac"
#define ATREDIES_DIRGE_FLAC  "16-Atredies-Dirge.flac"
#define ORDOS_DIRGE_FLAC     "17-Ordos-Dirge.flac"
#define FINALE_FLAC          "19-Finale.flac"
#define CREDIT_ROLL_FLAC     "20-Credit-Roll.flac"


class AssetManager final
{
public:
    template<class T>
    T* get(const std::string& filename) noexcept;

private:
    template<class T>
    T* tryLoadFromFile(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept;

private:
    std::unordered_map<std::string, sf::Texture>     m_textures;
    std::unordered_map<std::string, sf::Font>        m_fonts;
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Music>       m_musicBox;
};

#include "assets/AssetManager.inl"

#endif // !ASSET_MANAGER_HPP