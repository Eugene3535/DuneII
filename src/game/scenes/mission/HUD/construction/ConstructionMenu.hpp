#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <array>
#include <span>

#include <SFML/Graphics/RectangleShape.hpp>

#include "common/Enums.hpp"
#include "game/scenes/Scene.hpp"


class ConstructionMenu:
    public Scene
{
public:
    ConstructionMenu(class DuneII* game, class Tilemap& tilemap) noexcept;
    ~ConstructionMenu();

    bool load(std::string_view data)         noexcept override;
    void showEntityView(PreviewType preview) noexcept;
    void showEntityMenu(PreviewType mainPreview, std::span<PreviewType> menu) noexcept;
    void hide()                    noexcept;
    void resize(sf::Vector2u size) noexcept override;

    bool isPreviewShown() const noexcept;
    bool isMenuShown() const noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void createFrames()   noexcept;
    bool createPreviews() noexcept;
    bool createButtons()  noexcept;

    class Tilemap& m_tilemap;

    sf::RectangleShape m_rootShape;
    sf::RectangleShape m_entityShape;
    sf::RectangleShape m_entityShapeLabel;
    sf::RectangleShape m_entityShapeParams[3];

    std::array<sf::RectangleShape, 18> m_previews;
    sf::RectangleShape m_mainPreview;
    sf::RectangleShape m_sideBarEntityPreview;

    sf::RectangleShape m_buttonExit;
    sf::RectangleShape m_buttonRepair;
    sf::RectangleShape m_buttonStop;
    
    std::span<const sf::IntRect> m_textureGrid;
    bool m_isOnlyEntityView;
    bool m_isShown;
};

#endif // !CONSTRUCTION_MENU_HPP