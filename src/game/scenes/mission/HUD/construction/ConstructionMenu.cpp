#include <cstring>

#include "resources/files/FileProvider.hpp"
#include "game/scenes/mission/tilemap/Tilemap.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/mission/HUD/construction/ConstructionMenu.hpp"


#define PREVIEW_ICON_COLUMNS 3
#define PREVIEW_ICON_ROWS 6

#define DEFAULT_MENU_WIDTH  920.f
#define DEFAULT_MENU_HEIGHT 800.f
#define MENU_SCALE_FACTOR 0.7f


constexpr static sf::Color background_color      = sf::Color(155, 160, 163, 255);
constexpr static sf::Color outline_color         = sf::Color(170, 199, 207, 255);
constexpr static sf::Color cell_background_color = sf::Color(116, 120, 121, 255);
constexpr static sf::Color selection_frame_color = sf::Color::Red;


ConstructionMenu::ConstructionMenu(DuneII* game, Tilemap& tilemap) noexcept:
    Scene(game, Scene::CONSTRUCTION_MENU),
    m_tilemap(tilemap),
    m_isOnlyEntityView(false),
    m_isShown(false)
{

}


ConstructionMenu::~ConstructionMenu() = default;


bool ConstructionMenu::load(std::string_view data) noexcept
{
    const auto windowSize = sf::Vector2f(m_game->window.getSize());
    m_view.setCenter(windowSize * 0.5f);

    createFrames();

    if (!createPreviews())
        return false;
        
    if (!createButtons())
        return false;

    return true;
}


void ConstructionMenu::showEntityView(PreviewType preview) noexcept
{
    if(preview >= PreviewType::MAX)
        return;

    const size_t index = static_cast<size_t>(preview);
    const auto& texCoords = m_textureGrid[index];
    m_sideBarEntityPreview.setTextureRect(texCoords);
    m_isOnlyEntityView = true;
}


void ConstructionMenu::showEntityMenu(PreviewType mainPreview, std::span<PreviewType> menu) noexcept
{
    if(mainPreview >= PreviewType::MAX)
        return;

    m_isOnlyEntityView = false;

    auto setup_tex_coords = [this](sf::RectangleShape& shape, PreviewType preview) -> void
    {
        const size_t index = static_cast<size_t>(preview);
        const auto& texCoords = m_textureGrid[index];
        shape.setTextureRect(texCoords);
    };

    constexpr size_t previewCount = PREVIEW_ICON_COLUMNS * PREVIEW_ICON_ROWS;

//  Main preview
    setup_tex_coords(m_mainPreview, mainPreview);

//  Others previews (if exists)
    if(!menu.empty())
    {
        for (size_t i = 0; i < previewCount; ++i)
        {
            if(i < menu.size())
            {
                setup_tex_coords(m_previews[i], menu[i]);
            }
            else
            {
                setup_tex_coords(m_previews[i], PreviewType::Empty_Cell);
            }
        }
    }
    else
    {
        for (auto& preview : m_previews)
            setup_tex_coords(preview, PreviewType::Empty_Cell);
    }

    m_isShown = true;
}


void ConstructionMenu::hide() noexcept
{
    m_isShown = false;
}


void ConstructionMenu::resize(sf::Vector2u size) noexcept
{
    float dx = size.x * MENU_SCALE_FACTOR / DEFAULT_MENU_WIDTH;
    float dy = size.y * MENU_SCALE_FACTOR / DEFAULT_MENU_HEIGHT;
    // m_transform.setScale(dx, dy);
    // m_transform.setPosition(width * 0.5f, height * 0.5f);

    Scene::resize(size);

   //m_view.setCenter(sf::Vector2f(dx, dy) * 0.5f);
   //m_view.setSize({ dx, dy });
}


bool ConstructionMenu::isPreviewShown() const noexcept
{
    return m_isOnlyEntityView;
}


bool ConstructionMenu::isMenuShown() const noexcept
{
    return m_isShown;
}


void ConstructionMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.setView(m_view);

    if(m_isOnlyEntityView)
    {
        target.draw(m_sideBarEntityPreview, states);
    }    
    else
    {
//      Frames
        target.draw(m_rootShape, states);
        target.draw(m_entityShape, states);
        target.draw(m_entityShapeLabel, states);

        target.draw(m_entityShapeParams[0], states);
        target.draw(m_entityShapeParams[1], states);
        target.draw(m_entityShapeParams[2], states);

//      Previews
        for (const auto& preview : m_previews)
            target.draw(preview, states);

        target.draw(m_mainPreview, states);
        
//      Buttons
        target.draw(m_buttonExit, states);
        target.draw(m_buttonRepair, states);
        target.draw(m_buttonStop, states);
    }
}


void ConstructionMenu::createFrames() noexcept
{ 
    auto createRectangle = [](sf::RectangleShape& shape, sf::Vector2f size, sf::Vector2f position = sf::Vector2f()) -> void
    {
        const float thickness = 5.f;

        shape.setSize(size);
        shape.setFillColor(background_color);
        shape.setOutlineThickness(thickness);
        shape.setOutlineColor(outline_color);
        shape.setPosition(position);
    };

//  Root frame
    createRectangle(m_rootShape, { DEFAULT_MENU_WIDTH, DEFAULT_MENU_HEIGHT });

//  Entity presentation widget
    createRectangle(m_entityShape, { 300.f, 200.f }, { 580.f, 100.f });

//  Entity widget label
    createRectangle(m_entityShapeLabel, { 300.f, 50.f }, { 580.f, 315.f });

//  Entity property labels
    createRectangle(m_entityShapeParams[0], { 300.f, 50.f }, { 580.f, 450.f });
    createRectangle(m_entityShapeParams[1], { 300.f, 50.f }, { 580.f, 550.f });
    createRectangle(m_entityShapeParams[2], { 300.f, 50.f }, { 580.f, 650.f });
}


bool ConstructionMenu::createPreviews() noexcept
{
    const sf::Texture* texture = m_game->assets.get<sf::Texture>(PREVIEWS_PNG);

    if (!texture)
        return false;

    const uint32_t columns = 6; // The number of tiles in the texture horizontally
    const uint32_t rows = 7;    // and vertically
    m_game->animations.createGridAnimaton("PreviewGrid", texture->getSize(), columns, rows);
    m_textureGrid = m_game->animations.getFrames("PreviewGrid");

    if (m_textureGrid.empty())
        return false;

    const sf::Vector2f cellSize(150.f, 100.f);
    const sf::Vector2f startPos(50.f, 100.f);
    const float indent = 10.f;

    const size_t index = static_cast<size_t>(PreviewType::Empty_Cell);
    const sf::IntRect& texCoords = m_textureGrid[index]; // offset to gray color cell (empty preview)

    for (int32_t y = 0; y < PREVIEW_ICON_ROWS; ++y)
    {
        for (int32_t x = 0; x < PREVIEW_ICON_COLUMNS; ++x)
        {
            const size_t idx = y * PREVIEW_ICON_COLUMNS + x;
            auto& previewShape = m_previews[idx];

            const float left = startPos.x + cellSize.x * x + indent * x;
            const float top  = startPos.y + cellSize.y * y + indent * y;

            previewShape.setSize(cellSize);
            previewShape.setPosition({left, top});
            previewShape.setTexture(texture);
            previewShape.setTextureRect(texCoords);
        }
    }

//  Main preview
    m_mainPreview.setSize({ 280, 180.f });
    m_mainPreview.setPosition({ 590.f, 110.f });
    m_mainPreview.setTexture(texture);
    m_mainPreview.setTextureRect(texCoords);
    
//  Side bar entity preview
    m_sideBarEntityPreview.setSize({ 150, 100.f });
    m_sideBarEntityPreview.setPosition({ 1020.f, 100.f });
    m_sideBarEntityPreview.setTexture(texture);
    m_sideBarEntityPreview.setTextureRect(texCoords);

    return true;
}


bool ConstructionMenu::createButtons() noexcept
{
    std::array<sf::Texture*, 3> textures;
    uint32_t currentTexture = 0;

    for (const auto textureName : { BUTTON_EXIT_RU_PNG, BUTTON_REPAIR_RU_PNG, BUTTON_STOP_RU_PNG })
    {
        sf::Texture* texture = m_game->assets.get<sf::Texture>(textureName);

        if (!texture)
            return false;

        textures[currentTexture++] = texture;
    }

    const sf::Vector2f posFrame(50.f, 30.f);
    const float offset = 160.f;

    for (size_t i = 0; i < 3; ++i)
    {
        sf::RectangleShape* button = &m_buttonExit;
        if(i == 1) button = &m_buttonRepair;
        if(i == 2) button = &m_buttonStop;

        button->setSize({150.f, 50.f});
        button->setTexture(textures[i]);
        button->setPosition({posFrame.x + i * offset, posFrame.y});
    }

    return true;
}