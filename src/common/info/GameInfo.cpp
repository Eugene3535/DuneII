#include <array>
#include <unordered_set>
#include <cassert>
#include <algorithm>

#include "common/info/GameInfo.hpp"


GameInfo::GameInfo() noexcept = default;


std::vector<EntityPreview::Icon> GameInfo::getPreviewIconList(HouseType house, StructureInfo::Type building, uint32_t mission) const noexcept
{
	assert(mission > 0);
	assert(mission < 10);

	// Filtering available entities by mission, excluding 8 and 9 (all entities are available)
	// No research is conducted in missions 8 and 9.

	std::vector<EntityPreview::Icon> previews;

	switch (mission)
	{
		case 1:                                                           // MISSION 1
			switch (building)
			{
				case StructureInfo::VEHICLE:
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Slab_2x2,
					EntityPreview::Icon::Wind_Trap,
					EntityPreview::Icon::Refinery
				});
				break;

				case StructureInfo::BARRACKS:
				break;

				case StructureInfo::STARPORT:
				break;

				default:
					break;
			}
		break;

		case 2:                                                           // MISSION 2
			switch (building)
			{
				case StructureInfo::VEHICLE: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Raider_Trike,
					EntityPreview::Icon::Trike
				});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Slab_2x2,
					EntityPreview::Icon::Wind_Trap,
					EntityPreview::Icon::Refinery,
					EntityPreview::Icon::Outpost,
					EntityPreview::Icon::Spice_Silo,
					EntityPreview::Icon::Barracks,
					EntityPreview::Icon::Light_Vehicle_Factory
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Infantry
				});
				break;

				case StructureInfo::STARPORT:
				break;

				default:
					break;
			}
		break;

		case 3:                                                           // MISSION 3
			switch (building)
			{
				case StructureInfo::VEHICLE: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Raider_Trike,
					EntityPreview::Icon::Trike,
					EntityPreview::Icon::Quad
				});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Slab_2x2,
					EntityPreview::Icon::Wind_Trap,
					EntityPreview::Icon::Refinery,
					EntityPreview::Icon::Outpost,
					EntityPreview::Icon::Spice_Silo,
					EntityPreview::Icon::Barracks,
					EntityPreview::Icon::Light_Vehicle_Factory
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Trooper,
					EntityPreview::Icon::Infantry
				});
				break;

				case StructureInfo::STARPORT:
				break;

				default:
					break;
			}
		break;

		case 4:                                                           // MISSION 4
			switch (building)
			{
				case StructureInfo::VEHICLE: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Raider_Trike,
					EntityPreview::Icon::Trike,
					EntityPreview::Icon::Quad,
					EntityPreview::Icon::Harvester,
					EntityPreview::Icon::Tank,
					EntityPreview::Icon::MCV
				});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Slab_2x2,
					EntityPreview::Icon::Wind_Trap,
					EntityPreview::Icon::Refinery,
					EntityPreview::Icon::Outpost,
					EntityPreview::Icon::Spice_Silo,
					EntityPreview::Icon::Barracks,
					EntityPreview::Icon::Light_Vehicle_Factory,
					EntityPreview::Icon::Wall
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Infantry,
					EntityPreview::Icon::Trooper,
					EntityPreview::Icon::Troopers
				});
				break;

				case StructureInfo::STARPORT:
				break;

				default:
					break;
			}
		break;

		case 5:                                                           // MISSION 5
			switch (building)
			{
				case StructureInfo::VEHICLE: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Raider_Trike,
					EntityPreview::Icon::Trike,
					EntityPreview::Icon::Quad,
					EntityPreview::Icon::Harvester,
					EntityPreview::Icon::Tank,
					EntityPreview::Icon::MCV,
					EntityPreview::Icon::Launcher
				});
				break;

				case StructureInfo::HIGH_TECH: previews.insert(previews.end(), {
				                                                       EntityPreview::Icon::Carryall
				                                                    });
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Slab_2x2,
					EntityPreview::Icon::Wind_Trap,
					EntityPreview::Icon::Refinery,
					EntityPreview::Icon::Outpost,
					EntityPreview::Icon::Spice_Silo,
					EntityPreview::Icon::Barracks,
					EntityPreview::Icon::Light_Vehicle_Factory,
					EntityPreview::Icon::Turret,
					EntityPreview::Icon::Wall,
					EntityPreview::Icon::Repair,
					EntityPreview::Icon::High_Tech
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Infantry,
					EntityPreview::Icon::Trooper,
					EntityPreview::Icon::Troopers
				});
				break;

				case StructureInfo::STARPORT:
				break;

				default:
					break;
			}
		break;

		case 6:                                                           // MISSION 6
			switch (building)
			{
				case StructureInfo::VEHICLE: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Trike,
					EntityPreview::Icon::Raider_Trike,
					EntityPreview::Icon::Quad,
					EntityPreview::Icon::Harvester,
					EntityPreview::Icon::Tank,
					EntityPreview::Icon::MCV,
					EntityPreview::Icon::Launcher,
					EntityPreview::Icon::Siege_Tank
				});
				break;

				case StructureInfo::HIGH_TECH: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Carryall
				});
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Slab_2x2,
					EntityPreview::Icon::Wind_Trap,
					EntityPreview::Icon::Refinery,
					EntityPreview::Icon::Outpost,
					EntityPreview::Icon::Spice_Silo,
					EntityPreview::Icon::Barracks,
					EntityPreview::Icon::Light_Vehicle_Factory,
					EntityPreview::Icon::Turret,
					EntityPreview::Icon::Wall,
					EntityPreview::Icon::Repair,
					EntityPreview::Icon::Rocket_Turret,
					EntityPreview::Icon::High_Tech,
					EntityPreview::Icon::Starport
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Infantry,
					EntityPreview::Icon::Trooper,
					EntityPreview::Icon::Troopers
				});
				break;

				case StructureInfo::STARPORT: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Trike,
					EntityPreview::Icon::Quad,
					EntityPreview::Icon::Harvester,
					EntityPreview::Icon::Tank,
					EntityPreview::Icon::MCV,
					EntityPreview::Icon::Launcher,
					EntityPreview::Icon::Siege_Tank
				});
				break;

				default:
					break;
			}
		break;

		case 7:                                                           // MISSION 7
			switch (building)
			{
				case StructureInfo::VEHICLE: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Trike,
					EntityPreview::Icon::Raider_Trike,
					EntityPreview::Icon::Quad,
					EntityPreview::Icon::Harvester,
					EntityPreview::Icon::Tank,
					EntityPreview::Icon::MCV,
					EntityPreview::Icon::Launcher,
					EntityPreview::Icon::Siege_Tank,
					EntityPreview::Icon::Sonic_Tank,
					EntityPreview::Icon::Devastator,
					EntityPreview::Icon::Deviator
				});
				break;

				case StructureInfo::HIGH_TECH: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Carryall,
					EntityPreview::Icon::Ornithopter
				});
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Slab_2x2,
					EntityPreview::Icon::Wind_Trap,
					EntityPreview::Icon::Refinery,
					EntityPreview::Icon::Outpost,
					EntityPreview::Icon::Spice_Silo,
					EntityPreview::Icon::Barracks,
					EntityPreview::Icon::Light_Vehicle_Factory,
					EntityPreview::Icon::Turret,
					EntityPreview::Icon::Wall,
					EntityPreview::Icon::Repair,
					EntityPreview::Icon::Rocket_Turret,
					EntityPreview::Icon::High_Tech,
					EntityPreview::Icon::Starport
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Infantry,
					EntityPreview::Icon::Trooper,
					EntityPreview::Icon::Troopers
				});
				break;

				case StructureInfo::STARPORT: previews.insert(previews.end(), 
				{
					EntityPreview::Icon::Trike,
					EntityPreview::Icon::Quad,
					EntityPreview::Icon::Harvester,
					EntityPreview::Icon::Tank,
					EntityPreview::Icon::MCV,
					EntityPreview::Icon::Launcher,
					EntityPreview::Icon::Siege_Tank,
					EntityPreview::Icon::Ornithopter
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
			case StructureInfo::VEHICLE: previews.insert(previews.end(), 
			{
				EntityPreview::Icon::Trike,
				EntityPreview::Icon::Raider_Trike,
				EntityPreview::Icon::Quad,
				EntityPreview::Icon::Harvester,
				EntityPreview::Icon::Tank,
				EntityPreview::Icon::MCV,
				EntityPreview::Icon::Launcher,
				EntityPreview::Icon::Siege_Tank,
				EntityPreview::Icon::Sonic_Tank,
				EntityPreview::Icon::Deviator,
				EntityPreview::Icon::Devastator
			});
			break;

			case StructureInfo::HIGH_TECH: previews.insert(previews.end(), 
			{
				EntityPreview::Icon::Carryall,
				EntityPreview::Icon::Ornithopter
			});
			break;

			case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
			{
				EntityPreview::Icon::Slab_2x2,
				EntityPreview::Icon::Wind_Trap,
				EntityPreview::Icon::Refinery,
				EntityPreview::Icon::Outpost,
				EntityPreview::Icon::Spice_Silo,
				EntityPreview::Icon::Barracks,
				EntityPreview::Icon::Light_Vehicle_Factory,
				EntityPreview::Icon::Turret,
				EntityPreview::Icon::Wall,
				EntityPreview::Icon::Repair,
				EntityPreview::Icon::Rocket_Turret,
				EntityPreview::Icon::High_Tech,
				EntityPreview::Icon::Starport,
				EntityPreview::Icon::Palace
			});
			break;

			case StructureInfo::BARRACKS: previews.insert(previews.end(), 
			{
				EntityPreview::Icon::Infantry,
				EntityPreview::Icon::Trooper,
				EntityPreview::Icon::Troopers
			});
			break;

			case StructureInfo::STARPORT: previews.insert(previews.end(), 
			{
				EntityPreview::Icon::Trike,
				EntityPreview::Icon::Quad,
				EntityPreview::Icon::Harvester,
				EntityPreview::Icon::Tank,
				EntityPreview::Icon::MCV,
				EntityPreview::Icon::Launcher,
				EntityPreview::Icon::Siege_Tank,
				EntityPreview::Icon::Ornithopter
			});
			break;

			default:
				break;
		}
	}

//  Filtering by house
	auto remove_previews = [](std::vector<EntityPreview::Icon>& vec, std::initializer_list<EntityPreview::Icon> values) -> void
	{
		std::unordered_set<EntityPreview::Icon> to_remove(values);

		vec.erase(std::remove_if(vec.begin(), vec.end(),
			[&to_remove](EntityPreview::Icon icon) { return to_remove.contains(icon); }), vec.end());
	};

	switch (house)
	{
		case HouseType::Atreides:
			switch (building)
			{
				case StructureInfo::VEHICLE:
					remove_previews(previews, 
					{
						EntityPreview::Icon::Raider_Trike,
						EntityPreview::Icon::Deviator,
						EntityPreview::Icon::Devastator
					});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD:
				break;

				case StructureInfo::BARRACKS:
					remove_previews(previews, 
					{
						EntityPreview::Icon::Trooper,
						EntityPreview::Icon::Troopers
					});
				break;

				case StructureInfo::STARPORT:
				break;

				default:
					break;
			}
		break;

		case HouseType::Ordos:
			switch (building)
			{
				case StructureInfo::VEHICLE:
					remove_previews(previews, 
					{
						EntityPreview::Icon::Trike,
						EntityPreview::Icon::Launcher,
						EntityPreview::Icon::Sonic_Tank,
						EntityPreview::Icon::Devastator
					});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD:
				break;

				case StructureInfo::BARRACKS:
				break;

				case StructureInfo::STARPORT:
				break;

				default:
					break;
			}
		break;

		case HouseType::Harkonnen:
			switch (building)
			{
				case StructureInfo::VEHICLE:
					remove_previews(previews, 
					{
						EntityPreview::Icon::Raider_Trike,
						EntityPreview::Icon::Trike,
						EntityPreview::Icon::Sonic_Tank,
						EntityPreview::Icon::Deviator
					});
				break;

				case StructureInfo::HIGH_TECH:
					remove_previews(previews, 
					{
						EntityPreview::Icon::Ornithopter
					});
				break;

				case StructureInfo::CONSTRUCTION_YARD:
				break;

				case StructureInfo::BARRACKS:
					remove_previews(previews, 
					{
						EntityPreview::Icon::Infantry
					});
				break;

				case StructureInfo::STARPORT:
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