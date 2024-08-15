#include "managers/assets/AssetManager.hpp"
#include "scenes/mission/buildings/Builder.hpp"

Builder::Builder() noexcept:
    m_buildings(nullptr),
    m_tileMask(nullptr),
    m_mapWidthInTiles(0),
    m_mapHeightInTiles(0),
    m_tileWidth(0),
    m_tileHeight(0)
{

}

Builder::~Builder()
{

}

bool Builder::init(TileMap& tilemap) noexcept
{
    m_buildings        = &tilemap.m_buildings;
    m_tileMask         = &tilemap.m_tileMask;
    m_mapWidthInTiles  = tilemap.m_mapSizeInTiles.x;
    m_mapHeightInTiles = tilemap.m_mapSizeInTiles.y;
    m_tileWidth        = tilemap.m_tileSize.x;
    m_tileHeight       = tilemap.m_tileSize.y;

    for(const auto& building : tilemap.m_buildingsOnLoad)
        placeBuilding(building.type, building.cellX, building.cellY);

    auto& objects = tilemap.m_objects;

    auto get_area_of = [&objects](House houseName) -> sf::IntRect
    {
        for(auto& object : objects)
        {
            if(object.name == "Area")
            {
                auto found = std::find_if(object.properties.begin(), object.properties.end(), [houseName](const TileMap::Object::Property& property)
                {
                    return houseName == static_cast<House>(std::stoul(property.value));
                });

                if(found != object.properties.end())
                    return object.bounds;
            }
        }

        return sf::IntRect();
    };

    auto atreides_area  = get_area_of(House::Atreides);
    auto ordos_area     = get_area_of(House::Ordos);
    auto harkonnen_area = get_area_of(House::Harkonnen);

    for(auto& [id, building] : *m_buildings)
    {
        auto type = building.getType();
        const bool repairable = ((type!= Building::CONCRETE_SLAB) && (type != Building::WALL));

        if(repairable)
        {
            if(building.getBounds().intersects(atreides_area))
            {
                building.changeOwner(House::Atreides);
            }
            else if(building.getBounds().intersects(ordos_area))
            {
                building.changeOwner(House::Ordos);
            }
            else if(building.getBounds().intersects(harkonnen_area))
            {
                building.changeOwner(House::Harkonnen);
            }
        }
    }

    return true;
}

Building* Builder::placeBuilding(Building::Type type, int32_t cellX, int32_t cellY) noexcept
{
	int32_t coordX = cellX * m_tileWidth;
	int32_t coordY = cellY * m_tileHeight;
    auto bounds = getBoundsOf(type, coordX, coordY);

    {// out of bounds ?
        int32_t map_width  = m_mapWidthInTiles * m_tileWidth;
        int32_t map_height = m_mapHeightInTiles * m_tileHeight;

        if(bounds.left < 0) return nullptr;
        if(bounds.top < 0)  return nullptr;
        if(bounds.left + bounds.width > map_width)  return nullptr;
        if(bounds.top + bounds.height > map_height) return nullptr;
    }

    const int32_t origin = cellY * m_mapWidthInTiles + cellX;

    {// can be constructed ?
        sf::Vector2i size;

        switch (type)
        {
            case Building::CONCRETE_SLAB:      size = { 1, 1 }; break;
            case Building::CONSTRUCTION_YARD:  size = { 2, 2 }; break;
            case Building::SPICE_SILO:         size = { 2, 2 }; break;
            case Building::STARPORT:           size = { 3, 3 }; break;
            case Building::WIND_TRAP:          size = { 2, 2 }; break;
            case Building::SPICE_REFINERY:     size = { 3, 2 }; break;
            case Building::RADAR_OUTPOST:      size = { 2, 2 }; break;
            case Building::REPAIR_FACILITY:    size = { 3, 2 }; break;
            case Building::PALACE:             size = { 3, 3 }; break;
            case Building::HIGH_TECH_FACILITY: size = { 2, 2 }; break;
            case Building::BARRACKS:           size = { 2, 2 }; break;
            case Building::VEHICLE_FACTORY:    size = { 3, 2 }; break;
            case Building::WALL:               size = { 1, 1 }; break;
            case Building::TURRET:             size = { 1, 1 }; break;
            case Building::ROCKET_TURRET:      size = { 1, 1 }; break;

            default: break;
        }

        const char* mask = m_tileMask->data();
        int32_t offset = origin;

        for (int32_t i = 0; i < size.y; ++i)
        {
            for (int32_t j = 0; j < size.x; ++j)  
                if(mask[offset + j] != 'R')
                    return nullptr;
            
            offset += m_mapWidthInTiles;
        }
    }

	if(auto found = m_buildings->find(origin); found == m_buildings->end())
	{
		if(const auto texture = Assets->getTexture("Buildings.png"); texture != nullptr)
		{
			auto building = &m_buildings->emplace(origin, Building()).first->second;

			building->m_type = type;
			building->setTexture(texture);
			building->setTextureRect(getTexCoordsOf(type));
			building->setPosition(coordX, coordY);
			building->m_hitPoints = building->m_maxHitPoints = getHitPointsOf(type);
			building->m_bounds = bounds;

			auto setup_tiles_on_mask = [this, origin](int32_t width, int32_t height, char symbol = 'B') -> void
			{
                char* mask = m_tileMask->data();
                int32_t offset = origin;

				for (int32_t i = 0; i < height; ++i)
                {
					for (int32_t j = 0; j < width; ++j)  
                        mask[offset + j] = symbol;    
                    
                    offset += m_mapWidthInTiles;
                }
			};

			switch (type)
			{
				case Building::CONCRETE_SLAB:      setup_tiles_on_mask(1, 1, 'C'); break;
				case Building::CONSTRUCTION_YARD:  setup_tiles_on_mask(2, 2);      break;
				case Building::SPICE_SILO:         setup_tiles_on_mask(2, 2);      break;
				case Building::STARPORT:           setup_tiles_on_mask(3, 3);      break;
				case Building::WIND_TRAP:          setup_tiles_on_mask(2, 2);      break;
				case Building::SPICE_REFINERY:     setup_tiles_on_mask(3, 2);      break;
				case Building::RADAR_OUTPOST:      setup_tiles_on_mask(2, 2);      break;
				case Building::REPAIR_FACILITY:    setup_tiles_on_mask(3, 2);      break;
				case Building::PALACE:             setup_tiles_on_mask(3, 3);      break;
				case Building::HIGH_TECH_FACILITY: setup_tiles_on_mask(2, 2);      break;
				case Building::BARRACKS:           setup_tiles_on_mask(2, 2);      break;
				case Building::VEHICLE_FACTORY:    setup_tiles_on_mask(3, 2);      break;
				case Building::WALL:               setup_tiles_on_mask(1, 1, 'W'); break;
				case Building::TURRET:             setup_tiles_on_mask(1, 1);      break;
				case Building::ROCKET_TURRET:      setup_tiles_on_mask(1, 1);      break;

				default: break;
			}

            if(type == Building::WALL)
                updateWall(origin, 2);

            return building;
		}
	}

    return nullptr;
}

void Builder::updateWall(int32_t origin, int32_t level) noexcept
{
    if(level < 1) return;

    int32_t left   = origin - 1;
    int32_t top    = origin - m_mapWidthInTiles;
    int32_t right  = origin + 1;
    int32_t bottom = origin + m_mapWidthInTiles;

    const char* field = m_tileMask->data();

    bool a = (left >= 0)                   ? (field[left]   == 'W') : false;
    bool b = (top >= 0)                    ? (field[top]    == 'W') : false;
    bool c = (right < m_tileMask->size())  ? (field[right]  == 'W') : false;
    bool d = (bottom < m_tileMask->size()) ? (field[bottom] == 'W') : false;

    auto wall_type  = getWallType(a, b, c, d);
    auto tex_coords = getTexCoordsOf(wall_type);

    if(auto pair = m_buildings->find(origin); pair != m_buildings->end())
    {
        auto& sprite = pair->second;
        sprite.setTextureRect(tex_coords);
    }

    if(a) updateWall(left, level - 1);
    if(b) updateWall(top, level - 1);
    if(c) updateWall(right, level - 1);
    if(d) updateWall(bottom, level - 1);
}

Builder::WallCellType Builder::getWallType(bool left, bool top, bool right, bool bottom) noexcept
{
    if(!left && !top && !right && !bottom) return WallCellType::DOT;
    if((left || right) && !top && !bottom) return WallCellType::LEFT_RIGHT;
    if(!left && !right && (top || bottom)) return WallCellType::BOTTOM_TOP;
    if(top && right && !left && !bottom)   return WallCellType::TOP_RIGHT;
    if(!left && !top && right && bottom)   return WallCellType::RIGHT_BOTTOM;
    if(left && bottom && !top && !right)   return WallCellType::BOTTOM_LEFT;
    if(left && top && !right && !bottom)   return WallCellType::LEFT_TOP;
    if(!left && top && right && bottom)    return WallCellType::TOP_RIGHT_BOTTOM;
    if(left && right && bottom && !top)    return WallCellType::RIGHT_BOTTOM_LEFT;
    if(left && top && bottom && !right)    return WallCellType::BOTTOM_LEFT_TOP;
    if(left && top && right && !bottom)    return WallCellType::LEFT_TOP_RIGHT;

    return WallCellType::CROSS;
}

sf::IntRect Builder::getTexCoordsOf(Builder::WallCellType type) noexcept
{
    switch (type)
    {
        case WallCellType::DOT:               return { 0,   0, 32, 32 }; 
        case WallCellType::LEFT_RIGHT:        return { 32,  0, 32, 32 }; 
        case WallCellType::BOTTOM_TOP:        return { 64,  0, 32, 32 }; 
        case WallCellType::TOP_RIGHT:         return { 96,  0, 32, 32 }; 
        case WallCellType::RIGHT_BOTTOM:      return { 128, 0, 32, 32 };
        case WallCellType::BOTTOM_LEFT:       return { 160, 0, 32, 32 };
        case WallCellType::LEFT_TOP:          return { 192, 0, 32, 32 };
        case WallCellType::TOP_RIGHT_BOTTOM:  return { 224, 0, 32, 32 }; 
        case WallCellType::RIGHT_BOTTOM_LEFT: return { 256, 0, 32, 32 }; 
        case WallCellType::BOTTOM_LEFT_TOP:   return { 288, 0, 32, 32 };
        case WallCellType::LEFT_TOP_RIGHT:    return { 320, 0, 32, 32 };
        case WallCellType::CROSS:             return { 352, 0, 32, 32 };
    
        default: return { 0, 0, 32, 32 };
    }
}

sf::IntRect Builder::getTexCoordsOf(Building::Type type) const noexcept
{
    switch (type)
    {
        case Building::CONCRETE_SLAB:      return { 0, 160, 32, 32   };
        case Building::CONSTRUCTION_YARD:  return { 0, 32, 64, 64    };
        case Building::SPICE_SILO:         return { 192, 32, 64, 64  };
        case Building::STARPORT:           return { 0, 192, 96, 96   };
        case Building::WIND_TRAP:          return { 64, 32, 64, 64   };
        case Building::SPICE_REFINERY:     return { 416, 0, 96, 64   };
        case Building::RADAR_OUTPOST:      return { 128, 32, 64, 64  };
        case Building::REPAIR_FACILITY:    return { 224, 96, 96, 64  };
        case Building::PALACE:             return { 64, 96, 96, 96   };
        case Building::HIGH_TECH_FACILITY: return { 160, 96, 64, 64  };
        case Building::BARRACKS:           return { 0, 96, 64, 64    };
        case Building::VEHICLE_FACTORY:    return { 256, 32, 96, 64  };
        case Building::WALL:               return { 0, 0, 32, 32     };
        case Building::TURRET:             return { 192, 288, 32, 32 };
        case Building::ROCKET_TURRET:      return { 448, 288, 32, 32 };

        default: return sf::IntRect();
    }
}

sf::IntRect Builder::getBoundsOf(Building::Type type, int32_t coordX, int32_t coordY) const noexcept
{
	switch (type)
	{
		case Building::CONCRETE_SLAB:      return { coordX, coordY, 32, 32 };
		case Building::CONSTRUCTION_YARD:  return { coordX, coordY, 64, 64 };
		case Building::SPICE_SILO:         return { coordX, coordY, 64, 64 };
		case Building::STARPORT:           return { coordX, coordY, 96, 96 };
		case Building::WIND_TRAP:          return { coordX, coordY, 64, 64 };
		case Building::SPICE_REFINERY:     return { coordX, coordY, 96, 64 };
		case Building::RADAR_OUTPOST:      return { coordX, coordY, 64, 64 };
		case Building::REPAIR_FACILITY:    return { coordX, coordY, 96, 64 };
		case Building::PALACE:             return { coordX, coordY, 96, 96 };
		case Building::HIGH_TECH_FACILITY: return { coordX, coordY, 64, 64 };
		case Building::BARRACKS:           return { coordX, coordY, 64, 64 };
		case Building::VEHICLE_FACTORY:    return { coordX, coordY, 96, 64 };
		case Building::WALL:               return { coordX, coordY, 32, 32 };
		case Building::TURRET:             return { coordX, coordY, 32, 32 };
		case Building::ROCKET_TURRET:      return { coordX, coordY, 32, 32 };

		default: return sf::IntRect();
	}
}

int32_t Builder::getHitPointsOf(Building::Type type) const noexcept
{
    switch (type)
    {
        case Building::CONCRETE_SLAB:      return 40;
        case Building::CONSTRUCTION_YARD:  return 800;
        case Building::SPICE_SILO:         return 300;
        case Building::STARPORT:           return 1000;
        case Building::WIND_TRAP:          return 400;
        case Building::SPICE_REFINERY:     return 900;
        case Building::RADAR_OUTPOST:      return 1000;
        case Building::REPAIR_FACILITY:    return 1800;
        case Building::PALACE:             return 2000;
        case Building::HIGH_TECH_FACILITY: return 1000;
        case Building::BARRACKS:           return 600;
        case Building::VEHICLE_FACTORY:    return 800;
        case Building::WALL:               return 140;
        case Building::TURRET:             return 250;
        case Building::ROCKET_TURRET:      return 500;

        default: return 1;
    }
}