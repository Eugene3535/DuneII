#ifndef HEAD_UP_DISPLAY_HPP
#define HEAD_UP_DISPLAY_HPP

#include <entt/entity/fwd.hpp>

#include "game/scenes/Scene.hpp"
#include "game/scenes/mission/HUD/selection/Cursor.hpp"
#include "game/scenes/mission/HUD/construction/ConstructionMenu.hpp"


class HeadUpDisplay:
    public Scene
{
public:
    HeadUpDisplay(class DuneII* game, class Tilemap& tilemap) noexcept;
    ~HeadUpDisplay();

    bool load(std::string_view data) noexcept override;
    void update(sf::Time dt)         noexcept override;
    void resize(sf::Vector2u size)   noexcept override;

    void hideMenu()        noexcept;
    void runSelection()    noexcept;
    void cancelSelection() noexcept;

    bool isMenuShown() const noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    const class Tilemap& m_tilemap;

    Cursor m_cursor;
    ConstructionMenu m_menu;

    sf::Time m_clickTimer;
    entt::entity m_lastSelectedEntity;
};

#endif // !HEAD_UP_DISPLAY_HPP