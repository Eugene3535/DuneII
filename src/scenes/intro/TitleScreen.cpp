#include "game/DuneII.hpp"
#include "scenes/intro/TitleScreen.hpp"


TitleScreen::TitleScreen(DuneII* game) noexcept:
    Scene(game)
{

}


bool TitleScreen::load(const std::string& info) noexcept
{
    return false;
}


void TitleScreen::update(const sf::Time dt) noexcept
{

}