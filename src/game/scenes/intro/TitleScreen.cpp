#include "resources/ogl/texture/Texture.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/intro/TitleScreen.hpp"


// Sprite scaling and position factors when changing window size
#define PLANET_SCALE_FACTOR_WIDTH 2.f
#define PLANET_SCALE_FACTOR_HEIGHT 1.5f
#define PLANET_POSITION_FACTOR_X 1.3f
#define PLANET_POSITION_FACTOR_Y 3.f

#define BUTTON_SCALE_FACTOR_WIDTH 10.f
#define BUTTON_SCALE_FACTOR_HEIGHT 15.f
#define BUTTON_POSITION_FACTOR_X 2.f
#define BUTTON_POSITION_FACTOR_Y 1.2f

#define TEXT_INFO_SCALE_FACTOR_WIDTH 3.f
#define TEXT_INFO_SCALE_FACTOR_HEIGHT 20.f

#define INTRO_TEXT_PRESENTATION "Какие-то компании представляют"


TitleScreen::TitleScreen(DuneII* game) noexcept:
    Scene(game),
    m_isPresented(false)
{
#ifdef DEBUG
    m_isPresented = true;
#endif
}


TitleScreen::~TitleScreen()
{
    auto glResources = m_game->glResourceHolder.get();

    glResources->destroy(m_textures, glDeleteTextures);
}


bool TitleScreen::load(std::string_view info) noexcept
{
    if(!m_isLoaded)
    {
        auto glResources = m_game->glResourceHolder.get();
        auto textureHandles = glResources->create(5, glGenTextures);

        if(textureHandles.empty())
            return false;

        std::array<Texture, 5> textures;

        for(size_t i = 0; i < textureHandles.size(); ++i)
        {
            textures[i].handle = textureHandles[i];
            m_textures.push_back(textureHandles[i]);
        }


        m_isLoaded = true;
    }

    return m_isLoaded;
}


void TitleScreen::update(float dt) noexcept
{

}


void TitleScreen::resize(const glm::ivec2& size) noexcept
{

}
