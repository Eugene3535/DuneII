#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include <vector>

#include "game/scenes/Scene.hpp"


class TitleScreen:
    public Scene
{
public:
    explicit TitleScreen(class DuneII* game) noexcept;
    ~TitleScreen();

    bool load(std::string_view info)    noexcept override;
    void update(float dt)               noexcept override;
    void resize(const glm::ivec2& size) noexcept override;

private:
    std::vector<uint32_t> m_textures;

    bool m_isPresented;
};

#endif // !TITLE_SCREEN_HPP