#include "resources/files/FileProvider.hpp"
#include "game/scenes/mission/tilemap/Tilemap.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/mission/HUD/HeadUpDisplay.hpp"


constexpr sf::Time blink_loop_time = sf::milliseconds(250);


HeadUpDisplay::HeadUpDisplay(DuneII* game, Tilemap& tilemap) noexcept:
    Scene(game, Scene::HUD),
    m_tilemap(tilemap),
    m_menu(game, tilemap),
    m_clickTimer(),
    m_lastSelectedEntity(entt::null)
{

}


HeadUpDisplay::~HeadUpDisplay() = default;


bool HeadUpDisplay::load(std::string_view data) noexcept
{
    if (!m_cursor.load(m_game->animations, m_game->assets))
        return false;

    if (!m_menu.load(data))
        return false;

    m_game->window.setMouseCursorVisible(false);

    return true;
}


void HeadUpDisplay::update(sf::Time dt) noexcept
{
    if (!m_menu.isMenuShown())
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(m_game->window);
        const auto worldPosition = m_game->window.mapPixelToCoords(mousePosition);

        m_cursor.update(worldPosition, dt);
    }

    m_clickTimer += dt;
}


void HeadUpDisplay::resize(sf::Vector2u size) noexcept
{
    m_menu.resize(size);
}


void HeadUpDisplay::hideMenu() noexcept
{
    m_menu.hide();
}


void HeadUpDisplay::runSelection() noexcept
{
    if (m_menu.isMenuShown())
        return;

    const sf::Vector2i mousePosition = sf::Mouse::getPosition(m_game->window);
    const sf::Vector2i worldPosition = sf::Vector2i(m_game->window.mapPixelToCoords(mousePosition));

    const auto entity = m_tilemap.getEntityUnderCursor(worldPosition);

    if(entity == entt::null)
    {
        m_lastSelectedEntity = entt::null;

        return;
    }

    auto convert_building_type_to_preview = [](StructureInfo::Type type) -> PreviewType
    {
        switch (type)
        {
            case StructureInfo::SLAB_2x2:          return PreviewType::Slab_2x2;
            case StructureInfo::PALACE:            return PreviewType::Palace;
            case StructureInfo::VEHICLE:           return PreviewType::Light_Vehicle_Factory;
            case StructureInfo::HIGH_TECH:         return PreviewType::High_Tech;
            case StructureInfo::CONSTRUCTION_YARD: return PreviewType::Construction_Yard;
            case StructureInfo::WIND_TRAP:         return PreviewType::Wind_Trap;
            case StructureInfo::BARRACKS:          return PreviewType::Barracks;
            case StructureInfo::STARPORT:          return PreviewType::Starport;
            case StructureInfo::REFINERY:          return PreviewType::Refinery;
            case StructureInfo::REPAIR:            return PreviewType::Repair;
            case StructureInfo::TURRET:            return PreviewType::Turret;
            case StructureInfo::ROCKET_TURRET:     return PreviewType::Rocket_Turret;
            case StructureInfo::SILO:              return PreviewType::Spice_Silo;
            case StructureInfo::OUTPOST:           return PreviewType::Outpost;
        
            default: return PreviewType::Empty_Cell;
        }
    };

    auto& registry = m_tilemap.getRegistry();

    if (m_lastSelectedEntity != entity)
    {
        m_lastSelectedEntity = entity;
    }           
    else
    {
        if (m_clickTimer > blink_loop_time)
        {
            if (StructureInfo* info = registry.try_get<StructureInfo>(entity))
            {
                const auto mainPreview = convert_building_type_to_preview(info->type);

                if (mainPreview != PreviewType::Empty_Cell)
                {
                    const bool hasConstructionPreviews = ((info->type == StructureInfo::Type::VEHICLE)           ||
                                                          (info->type == StructureInfo::Type::HIGH_TECH)         || 
                                                          (info->type == StructureInfo::Type::CONSTRUCTION_YARD) || 
                                                          (info->type == StructureInfo::Type::BARRACKS)          ||
                                                          (info->type == StructureInfo::Type::STARPORT));

                    std::span<PreviewType> previews;

                    if (hasConstructionPreviews)
                    {
                        std::vector<PreviewType>* previewArray = registry.try_get<std::vector<PreviewType>>(entity);

                        if (previewArray)
                            previews = std::span(*previewArray);
                    }

                    m_menu.showEntityMenu(mainPreview, previews);
                }        
            }     
        }
            
        return;
    }

    m_clickTimer = sf::Time::Zero;;

    auto [info, bounds] = registry.try_get<StructureInfo, sf::IntRect>(entity);

    if (info)
    {
        bool isSelectable = ((info->type != StructureInfo::Type::SLAB_1x1) &&
                             (info->type != StructureInfo::Type::SLAB_2x2) && 
                             (info->type != StructureInfo::Type::WALL)     && 
                              info->type <  StructureInfo::Type::MAX);

        if (isSelectable)
        {
            const auto entityView = convert_building_type_to_preview(info->type);

            if(entityView != PreviewType::Empty_Cell)
                m_menu.showEntityView(entityView);

            if (bounds)
            {
                m_cursor.setVertexFrame(*bounds);
                m_cursor.select();
            }
        }
    }
}


void HeadUpDisplay::cancelSelection() noexcept
{
    m_lastSelectedEntity = entt::null;
    m_cursor.release();
}


bool HeadUpDisplay::isMenuShown() const noexcept
{
    return m_menu.isMenuShown();
}


void HeadUpDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_menu.isMenuShown())
        target.draw(m_cursor, states);

    if (m_menu.isMenuShown() || m_menu.isPreviewShown())
        target.draw(m_menu, states);
}