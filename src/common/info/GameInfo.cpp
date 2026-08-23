#include <array>
#include <unordered_set>
#include <cassert>
#include <algorithm>

#include "common/info/GameInfo.hpp"


namespace GameInfo {


StructureInfo::Type getStructureType(int tileNum) noexcept
{
	switch (tileNum) // start num of tile in grid
	{
		case 111:
		case 112:
		case 113:
		case 114:
		case 115:
		case 116:
		case 117:
		case 118:
		case 119:
		case 120:
		case 121:
		case 122: return StructureInfo::Type::Wall;
		case 124: return StructureInfo::Type::Refinery;
		case 127: return StructureInfo::Type::ConstructionYard;
		case 129: return StructureInfo::Type::WindTrap;
		case 131: return StructureInfo::Type::Outpost;
		case 133: return StructureInfo::Type::Silo;
		case 135: return StructureInfo::Type::Vehicle;
		case 159: return StructureInfo::Type::Barracks;
		case 161: return StructureInfo::Type::Palace;
		case 164: return StructureInfo::Type::HighTech;
		case 166: return StructureInfo::Type::Repair;
		case 191: return StructureInfo::Type::Slab_1x1;
		case 207: return StructureInfo::Type::Starport;
		case 261: return StructureInfo::Type::Turret;
		case 269: return StructureInfo::Type::RocketTurret;
	
		default: return StructureInfo::Type::Undefined;
	}
}


ivec2s getStructureSizeInTiles(StructureInfo::Type type) noexcept
{
	switch (type)
	{
		case StructureInfo::Type::Slab_1x1:         return { 1, 1 };
		case StructureInfo::Type::Palace:           return { 3, 3 };
		case StructureInfo::Type::Vehicle:          return { 3, 2 };
		case StructureInfo::Type::HighTech:         return { 2, 2 };
		case StructureInfo::Type::ConstructionYard: return { 2, 2 };
		case StructureInfo::Type::WindTrap:         return { 2, 2 };
		case StructureInfo::Type::Barracks:         return { 2, 2 };
		case StructureInfo::Type::Starport:         return { 3, 3 };
		case StructureInfo::Type::Refinery:         return { 3, 2 };
		case StructureInfo::Type::Repair:           return { 3, 2 };
		case StructureInfo::Type::Wall:             return { 1, 1 };
		case StructureInfo::Type::Turret:           return { 1, 1 };
		case StructureInfo::Type::RocketTurret:     return { 1, 1 };
		case StructureInfo::Type::Silo:             return { 2, 2 };
		case StructureInfo::Type::Outpost:          return { 2, 2 };

		default: return { 0, 0 };
	}
}


char convertTileNumToChar(int32_t index) noexcept
{
	#if defined(__GNUC__) || defined(__MINGW32__)

	switch (index)
	{
//      rocky soil
		case 1 ... 5:     return 'R';
		case 13 ... 16:   return 'R';
		case 25 ... 27:   return 'R';
		case 31 ... 34:   return 'R';
		case 40 ... 42:   return 'R';
		case 45:          return 'R';
		case 48:          return 'R';
		case 55:          return 'R';
		case 61:          return 'R'; 
		case 82:          return 'R';
		case 83:          return 'R';
		case 101:         return 'R';
//      sandy soil
		case 6 ... 12:    return 'S';
		case 17 ... 24:   return 'S';	
		case 28 ... 30:   return 'S';
		case 35 ... 39:   return 'S';
		case 43:          return 'S';
		case 44:          return 'S';
		case 46:          return 'S';
		case 47:          return 'S';
		case 49 ... 54:   return 'S';
		case 56 ... 60:   return 'S';
		case 62 ... 81:   return 'S';
		case 84 ... 100:  return 'S';
		case 102 ... 109: return 'S';
//      NOTE: Tiles occupied by buildings will be filled in later when loaded by the Builder class
//      Wall
		case 111 ... 122: return 'R';
//      Building
		case 124 ... 137: return 'R';
		case 140 ... 153: return 'R';
		case 159 ... 165: return 'R';
		case 175 ... 181: return 'R';
		case 192 ... 195: return 'R';
		case 207 ... 209: return 'R';
		case 223 ... 225: return 'R';
		case 239 ... 241: return 'R';	
		case 255 ... 286: return 'R';
//      Concrete slab
		case 191:         return 'R';
//      sandy soil by default
		default:          return 'S';
	}

#elif defined(_MSC_VER)

	switch (index)
	{
//  Rocky soil
		case 1:   return 'R';
		case 2:   return 'R';
		case 3:   return 'R';
		case 4:   return 'R';
		case 5:   return 'R';
		case 13:  return 'R';
		case 14:  return 'R';
		case 15:  return 'R';
		case 16:  return 'R';
		case 25:  return 'R';
		case 26:  return 'R';
		case 27:  return 'R';
		case 31:  return 'R';
		case 32:  return 'R';
		case 33:  return 'R';
		case 34:  return 'R';
		case 40:  return 'R';
		case 41:  return 'R';
		case 42:  return 'R';
		case 45:  return 'R';
		case 48:  return 'R';
		case 55:  return 'R';
		case 61:  return 'R'; 
		case 82:  return 'R';
		case 83:  return 'R';
		case 101: return 'R';
//  Sandy soil
		case 6:   return 'S';
		case 7:   return 'S';
		case 8:   return 'S';
		case 9:   return 'S';
		case 10:  return 'S';
		case 11:  return 'S';
		case 12:  return 'S';
		case 17:  return 'S';
		case 18:  return 'S';
		case 19:  return 'S';
		case 20:  return 'S';
		case 21:  return 'S';
		case 22:  return 'S';
		case 23:  return 'S';
		case 24:  return 'S';
		case 28:  return 'S';
		case 29:  return 'S';
		case 30:  return 'S';
		case 35:  return 'S';
		case 36:  return 'S';
		case 37:  return 'S';
		case 38:  return 'S';
		case 39:  return 'S';
		case 43:  return 'S';
		case 44:  return 'S';
		case 46:  return 'S';
		case 47:  return 'S';
		case 49:  return 'S';
		case 50:  return 'S';
		case 51:  return 'S';
		case 52:  return 'S';
		case 53:  return 'S';
		case 54:  return 'S';
		case 56:  return 'S';
		case 57:  return 'S';
		case 58:  return 'S';
		case 59:  return 'S';
		case 60:  return 'S';
		case 62:  return 'S';
		case 63:  return 'S';
		case 64:  return 'S';
		case 65:  return 'S';
		case 66:  return 'S';
		case 67:  return 'S';
		case 68:  return 'S';
		case 69:  return 'S';
		case 70:  return 'S';
		case 71:  return 'S';
		case 72:  return 'S';
		case 73:  return 'S';
		case 74:  return 'S';
		case 75:  return 'S';
		case 76:  return 'S';
		case 77:  return 'S';
		case 78:  return 'S';
		case 79:  return 'S';
		case 80:  return 'S';
		case 81:  return 'S';
		case 84:  return 'S';
		case 85:  return 'S';
		case 86:  return 'S';
		case 87:  return 'S';
		case 88:  return 'S';
		case 89:  return 'S';
		case 90:  return 'S';
		case 91:  return 'S';
		case 92:  return 'S';
		case 93:  return 'S';
		case 94:  return 'S';
		case 95:  return 'S';
		case 96:  return 'S';
		case 97:  return 'S';
		case 98:  return 'S';
		case 99:  return 'S';
		case 100: return 'S';
		case 102: return 'S';
		case 103: return 'S';
		case 104: return 'S';
		case 105: return 'S';
		case 106: return 'S';
		case 107: return 'S';
		case 108: return 'S';
		case 109: return 'S';
//      Wall
		case 111: return 'R';
		case 112: return 'R';
		case 113: return 'R';
		case 114: return 'R';
		case 115: return 'R';
		case 116: return 'R';
		case 117: return 'R';
		case 118: return 'R';
		case 119: return 'R';
		case 120: return 'R';
		case 121: return 'R';
		case 122: return 'R';
		case 124: return 'R';
		case 125: return 'R';
		case 126: return 'R';
		case 127: return 'R';
		case 128: return 'R';
		case 129: return 'R';
		case 130: return 'R';
		case 131: return 'R';
		case 132: return 'R';
		case 133: return 'R';
		case 134: return 'R';
		case 135: return 'R';
		case 136: return 'R';
		case 137: return 'R';
		case 140: return 'R';
		case 141: return 'R';
		case 142: return 'R';
		case 143: return 'R';
		case 144: return 'R';
		case 145: return 'R';
		case 146: return 'R';
		case 147: return 'R';
		case 148: return 'R';
		case 149: return 'R';
		case 150: return 'R';
		case 151: return 'R';
		case 152: return 'R';
		case 153: return 'R';
		case 159: return 'R';
		case 160: return 'R';
		case 161: return 'R';
		case 162: return 'R';
		case 163: return 'R';
		case 164: return 'R';
		case 165: return 'R';
		case 175: return 'R';
		case 176: return 'R';
		case 177: return 'R';
		case 178: return 'R';
		case 179: return 'R';
		case 180: return 'R';
		case 181: return 'R';
		case 192: return 'R';
		case 193: return 'R';
		case 194: return 'R';
		case 195: return 'R';
		case 207: return 'R';
		case 208: return 'R';
		case 209: return 'R';
		case 223: return 'R';
		case 224: return 'R';
		case 225: return 'R';
		case 239: return 'R';
		case 240: return 'R';
		case 241: return 'R';
		case 255: return 'R';
		case 256: return 'R';
		case 257: return 'R';
		case 258: return 'R';
		case 259: return 'R';
		case 260: return 'R';
		case 261: return 'R';
		case 262: return 'R';
		case 263: return 'R';
		case 264: return 'R';
		case 265: return 'R';
		case 266: return 'R';
		case 267: return 'R';
		case 268: return 'R';
		case 269: return 'R';
		case 270: return 'R';
		case 271: return 'R';
		case 272: return 'R';
		case 273: return 'R';
		case 274: return 'R';
		case 275: return 'R';
		case 276: return 'R';
		case 277: return 'R';
		case 278: return 'R';
		case 279: return 'R';
		case 280: return 'R';
		case 281: return 'R';
		case 282: return 'R';
		case 283: return 'R';
		case 284: return 'R';
		case 285: return 'R';
		case 286: return 'R';
//      Concrete slab
		case 191: return 'R';
//      sandy soil by default
		default:  return 'S';
	}

#else
    #error "Unsupported compiler"
#endif
}


std::vector<EntityIcon> GameInfo::getPreviewIconList(HouseType house, StructureInfo::Type building, uint32_t mission) noexcept
{
	assert(mission > 0);
	assert(mission < 10);

	// Filtering available entities by mission, excluding 8 and 9 (all entities are available)
	// No research is conducted in missions 8 and 9.

	std::vector<EntityIcon> previews;

	switch (mission)
	{
		case 1:                                                           // MISSION 1
			switch (building)
			{
				case StructureInfo::Type::Vehicle:
				break;

				case StructureInfo::Type::HighTech:
				break;

				case StructureInfo::Type::ConstructionYard: previews.insert(previews.end(), 
				{
					EntityIcon::Slab_2x2,
					EntityIcon::Wind_Trap,
					EntityIcon::Refinery
				});
				break;

				case StructureInfo::Type::Barracks:
				break;

				case StructureInfo::Type::Starport:
				break;

				default:
					break;
			}
		break;

		case 2:                                                           // MISSION 2
			switch (building)
			{
				case StructureInfo::Type::Vehicle: previews.insert(previews.end(), 
				{
					EntityIcon::Raider_Trike,
					EntityIcon::Trike
				});
				break;

				case StructureInfo::Type::HighTech:
				break;

				case StructureInfo::Type::ConstructionYard: previews.insert(previews.end(), 
				{
					EntityIcon::Slab_2x2,
					EntityIcon::Wind_Trap,
					EntityIcon::Refinery,
					EntityIcon::Outpost,
					EntityIcon::Spice_Silo,
					EntityIcon::Barracks,
					EntityIcon::Light_Vehicle_Factory
				});
				break;

				case StructureInfo::Type::Barracks: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry
				});
				break;

				case StructureInfo::Type::Starport:
				break;

				default:
					break;
			}
		break;

		case 3:                                                           // MISSION 3
			switch (building)
			{
				case StructureInfo::Type::Vehicle: previews.insert(previews.end(), 
				{
					EntityIcon::Raider_Trike,
					EntityIcon::Trike,
					EntityIcon::Quad
				});
				break;

				case StructureInfo::Type::HighTech:
				break;

				case StructureInfo::Type::ConstructionYard: previews.insert(previews.end(), 
				{
					EntityIcon::Slab_2x2,
					EntityIcon::Wind_Trap,
					EntityIcon::Refinery,
					EntityIcon::Outpost,
					EntityIcon::Spice_Silo,
					EntityIcon::Barracks,
					EntityIcon::Light_Vehicle_Factory
				});
				break;

				case StructureInfo::Type::Barracks: previews.insert(previews.end(), 
				{
					EntityIcon::Trooper,
					EntityIcon::Infantry
				});
				break;

				case StructureInfo::Type::Starport:
				break;

				default:
					break;
			}
		break;

		case 4:                                                           // MISSION 4
			switch (building)
			{
				case StructureInfo::Type::Vehicle: previews.insert(previews.end(), 
				{
					EntityIcon::Raider_Trike,
					EntityIcon::Trike,
					EntityIcon::Quad,
					EntityIcon::Harvester,
					EntityIcon::Tank,
					EntityIcon::MCV
				});
				break;

				case StructureInfo::Type::HighTech:
				break;

				case StructureInfo::Type::ConstructionYard: previews.insert(previews.end(), 
				{
					EntityIcon::Slab_2x2,
					EntityIcon::Wind_Trap,
					EntityIcon::Refinery,
					EntityIcon::Outpost,
					EntityIcon::Spice_Silo,
					EntityIcon::Barracks,
					EntityIcon::Light_Vehicle_Factory,
					EntityIcon::Wall
				});
				break;

				case StructureInfo::Type::Barracks: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry,
					EntityIcon::Trooper,
					EntityIcon::Troopers
				});
				break;

				case StructureInfo::Type::Starport:
				break;

				default:
					break;
			}
		break;

		case 5:                                                           // MISSION 5
			switch (building)
			{
				case StructureInfo::Type::Vehicle: previews.insert(previews.end(), 
				{
					EntityIcon::Raider_Trike,
					EntityIcon::Trike,
					EntityIcon::Quad,
					EntityIcon::Harvester,
					EntityIcon::Tank,
					EntityIcon::MCV,
					EntityIcon::Launcher
				});
				break;

				case StructureInfo::Type::HighTech: previews.insert(previews.end(), {
				                                                       EntityIcon::Carryall
				                                                    });
				break;

				case StructureInfo::Type::ConstructionYard: previews.insert(previews.end(), 
				{
					EntityIcon::Slab_2x2,
					EntityIcon::Wind_Trap,
					EntityIcon::Refinery,
					EntityIcon::Outpost,
					EntityIcon::Spice_Silo,
					EntityIcon::Barracks,
					EntityIcon::Light_Vehicle_Factory,
					EntityIcon::Turret,
					EntityIcon::Wall,
					EntityIcon::Repair,
					EntityIcon::High_Tech
				});
				break;

				case StructureInfo::Type::Barracks: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry,
					EntityIcon::Trooper,
					EntityIcon::Troopers
				});
				break;

				case StructureInfo::Type::Starport:
				break;

				default:
					break;
			}
		break;

		case 6:                                                           // MISSION 6
			switch (building)
			{
				case StructureInfo::Type::Vehicle: previews.insert(previews.end(), 
				{
					EntityIcon::Trike,
					EntityIcon::Raider_Trike,
					EntityIcon::Quad,
					EntityIcon::Harvester,
					EntityIcon::Tank,
					EntityIcon::MCV,
					EntityIcon::Launcher,
					EntityIcon::Siege_Tank
				});
				break;

				case StructureInfo::Type::HighTech: previews.insert(previews.end(), 
				{
					EntityIcon::Carryall
				});
				break;

				case StructureInfo::Type::ConstructionYard: previews.insert(previews.end(), 
				{
					EntityIcon::Slab_2x2,
					EntityIcon::Wind_Trap,
					EntityIcon::Refinery,
					EntityIcon::Outpost,
					EntityIcon::Spice_Silo,
					EntityIcon::Barracks,
					EntityIcon::Light_Vehicle_Factory,
					EntityIcon::Turret,
					EntityIcon::Wall,
					EntityIcon::Repair,
					EntityIcon::Rocket_Turret,
					EntityIcon::High_Tech,
					EntityIcon::Starport
				});
				break;

				case StructureInfo::Type::Barracks: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry,
					EntityIcon::Trooper,
					EntityIcon::Troopers
				});
				break;

				case StructureInfo::Type::Starport: previews.insert(previews.end(), 
				{
					EntityIcon::Trike,
					EntityIcon::Quad,
					EntityIcon::Harvester,
					EntityIcon::Tank,
					EntityIcon::MCV,
					EntityIcon::Launcher,
					EntityIcon::Siege_Tank
				});
				break;

				default:
					break;
			}
		break;

		case 7:                                                           // MISSION 7
			switch (building)
			{
				case StructureInfo::Type::Vehicle: previews.insert(previews.end(), 
				{
					EntityIcon::Trike,
					EntityIcon::Raider_Trike,
					EntityIcon::Quad,
					EntityIcon::Harvester,
					EntityIcon::Tank,
					EntityIcon::MCV,
					EntityIcon::Launcher,
					EntityIcon::Siege_Tank,
					EntityIcon::Sonic_Tank,
					EntityIcon::Devastator,
					EntityIcon::Deviator
				});
				break;

				case StructureInfo::Type::HighTech: previews.insert(previews.end(), 
				{
					EntityIcon::Carryall,
					EntityIcon::Ornithopter
				});
				break;

				case StructureInfo::Type::ConstructionYard: previews.insert(previews.end(), 
				{
					EntityIcon::Slab_2x2,
					EntityIcon::Wind_Trap,
					EntityIcon::Refinery,
					EntityIcon::Outpost,
					EntityIcon::Spice_Silo,
					EntityIcon::Barracks,
					EntityIcon::Light_Vehicle_Factory,
					EntityIcon::Turret,
					EntityIcon::Wall,
					EntityIcon::Repair,
					EntityIcon::Rocket_Turret,
					EntityIcon::High_Tech,
					EntityIcon::Starport
				});
				break;

				case StructureInfo::Type::Barracks: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry,
					EntityIcon::Trooper,
					EntityIcon::Troopers
				});
				break;

				case StructureInfo::Type::Starport: previews.insert(previews.end(), 
				{
					EntityIcon::Trike,
					EntityIcon::Quad,
					EntityIcon::Harvester,
					EntityIcon::Tank,
					EntityIcon::MCV,
					EntityIcon::Launcher,
					EntityIcon::Siege_Tank,
					EntityIcon::Ornithopter
				});
				break;

				default:
					break;
			}
		break;
		
		default:
			break;
	}

	if(mission > 7)
	{
		switch (building)
		{
			case StructureInfo::Type::Vehicle: previews.insert(previews.end(), 
			{
				EntityIcon::Trike,
				EntityIcon::Raider_Trike,
				EntityIcon::Quad,
				EntityIcon::Harvester,
				EntityIcon::Tank,
				EntityIcon::MCV,
				EntityIcon::Launcher,
				EntityIcon::Siege_Tank,
				EntityIcon::Sonic_Tank,
				EntityIcon::Deviator,
				EntityIcon::Devastator
			});
			break;

			case StructureInfo::Type::HighTech: previews.insert(previews.end(), 
			{
				EntityIcon::Carryall,
				EntityIcon::Ornithopter
			});
			break;

			case StructureInfo::Type::ConstructionYard: previews.insert(previews.end(), 
			{
				EntityIcon::Slab_2x2,
				EntityIcon::Wind_Trap,
				EntityIcon::Refinery,
				EntityIcon::Outpost,
				EntityIcon::Spice_Silo,
				EntityIcon::Barracks,
				EntityIcon::Light_Vehicle_Factory,
				EntityIcon::Turret,
				EntityIcon::Wall,
				EntityIcon::Repair,
				EntityIcon::Rocket_Turret,
				EntityIcon::High_Tech,
				EntityIcon::Starport,
				EntityIcon::Palace
			});
			break;

			case StructureInfo::Type::Barracks: previews.insert(previews.end(), 
			{
				EntityIcon::Infantry,
				EntityIcon::Trooper,
				EntityIcon::Troopers
			});
			break;

			case StructureInfo::Type::Starport: previews.insert(previews.end(), 
			{
				EntityIcon::Trike,
				EntityIcon::Quad,
				EntityIcon::Harvester,
				EntityIcon::Tank,
				EntityIcon::MCV,
				EntityIcon::Launcher,
				EntityIcon::Siege_Tank,
				EntityIcon::Ornithopter
			});
			break;

			default:
				break;
		}
	}

//  Filtering by house
	auto remove_previews = [](std::vector<EntityIcon>& vec, std::initializer_list<EntityIcon> values) -> void
	{
		std::unordered_set<EntityIcon> to_remove(values);

		vec.erase(std::remove_if(vec.begin(), vec.end(),
			[&to_remove](EntityIcon icon) { return to_remove.contains(icon); }), vec.end());
	};

	switch (house)
	{
		case HouseType::Atreides:
			switch (building)
			{
				case StructureInfo::Type::Vehicle:
					remove_previews(previews, 
					{
						EntityIcon::Raider_Trike,
						EntityIcon::Deviator,
						EntityIcon::Devastator
					});
				break;

				case StructureInfo::Type::HighTech:
				break;

				case StructureInfo::Type::ConstructionYard:
				break;

				case StructureInfo::Type::Barracks:
					remove_previews(previews, 
					{
						EntityIcon::Trooper,
						EntityIcon::Troopers
					});
				break;

				case StructureInfo::Type::Starport:
				break;

				default:
					break;
			}
		break;

		case HouseType::Ordos:
			switch (building)
			{
				case StructureInfo::Type::Vehicle:
					remove_previews(previews, 
					{
						EntityIcon::Trike,
						EntityIcon::Launcher,
						EntityIcon::Sonic_Tank,
						EntityIcon::Devastator
					});
				break;

				case StructureInfo::Type::HighTech:
				break;

				case StructureInfo::Type::ConstructionYard:
				break;

				case StructureInfo::Type::Barracks:
				break;

				case StructureInfo::Type::Starport:
				break;

				default:
					break;
			}
		break;

		case HouseType::Harkonnen:
			switch (building)
			{
				case StructureInfo::Type::Vehicle:
					remove_previews(previews, 
					{
						EntityIcon::Raider_Trike,
						EntityIcon::Trike,
						EntityIcon::Sonic_Tank,
						EntityIcon::Deviator
					});
				break;

				case StructureInfo::Type::HighTech:
					remove_previews(previews, 
					{
						EntityIcon::Ornithopter
					});
				break;

				case StructureInfo::Type::ConstructionYard:
				break;

				case StructureInfo::Type::Barracks:
					remove_previews(previews, 
					{
						EntityIcon::Infantry
					});
				break;

				case StructureInfo::Type::Starport:
				break;

				default:
					break;
			}
		break;

		case HouseType::Sardaukar:

		break;

		case HouseType::Fremen:

		break;

		case HouseType::Mercenary:

		break;
		
		default:
			break;
	}

	return previews;
}

} // !namespace GameInfo