#include "common/Enums.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/choosing_houses/Destiny.hpp"


#define ATREIDES_OUTLINE_POSITION_X 32.f
#define ORDOS_OUTLINE_POSITION_X 120.f
#define HARKONNEN_OUTLINE_POSITION_X 208.f
#define OUTLINE_POSITION_Y 136.f

#define DEFAULT_OUTLINE_SIZE_X 80.f
#define DEFAULT_OUTLINE_SIZE_Y 16.f


Destiny::Destiny(DuneII* game) noexcept:
    Scene(game)
{

}


bool Destiny::load(std::string_view info) noexcept
{
    if(!m_isLoaded)
    {

    }

    return m_isLoaded;
}


void Destiny::update(float dt) noexcept
{

}


void Destiny::draw() noexcept
{
    if(m_isLoaded)
    {

    }    
}


void Destiny::resize(int width, int height) noexcept
{

}