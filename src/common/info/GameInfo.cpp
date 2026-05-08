#include <array>
#include <unordered_set>
#include <cassert>
#include <algorithm>

#include "common/info/GameInfo.hpp"


GameInfo::GameInfo() noexcept = default;


std::vector<EntityIcon> GameInfo::getPreviewIconList(HouseType house, StructureInfo::Type building, uint32_t mission) const noexcept
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
				case StructureInfo::VEHICLE:
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					EntityIcon::Slab_2x2,
					EntityIcon::Wind_Trap,
					EntityIcon::Refinery
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
					EntityIcon::Raider_Trike,
					EntityIcon::Trike
				});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
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

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry
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
					EntityIcon::Raider_Trike,
					EntityIcon::Trike,
					EntityIcon::Quad
				});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
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

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityIcon::Trooper,
					EntityIcon::Infantry
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
					EntityIcon::Raider_Trike,
					EntityIcon::Trike,
					EntityIcon::Quad,
					EntityIcon::Harvester,
					EntityIcon::Tank,
					EntityIcon::MCV
				});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
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

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry,
					EntityIcon::Trooper,
					EntityIcon::Troopers
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
					EntityIcon::Raider_Trike,
					EntityIcon::Trike,
					EntityIcon::Quad,
					EntityIcon::Harvester,
					EntityIcon::Tank,
					EntityIcon::MCV,
					EntityIcon::Launcher
				});
				break;

				case StructureInfo::HIGH_TECH: previews.insert(previews.end(), {
				                                                       EntityIcon::Carryall
				                                                    });
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
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

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry,
					EntityIcon::Trooper,
					EntityIcon::Troopers
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

				case StructureInfo::HIGH_TECH: previews.insert(previews.end(), 
				{
					EntityIcon::Carryall
				});
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
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

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry,
					EntityIcon::Trooper,
					EntityIcon::Troopers
				});
				break;

				case StructureInfo::STARPORT: previews.insert(previews.end(), 
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
				case StructureInfo::VEHICLE: previews.insert(previews.end(), 
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

				case StructureInfo::HIGH_TECH: previews.insert(previews.end(), 
				{
					EntityIcon::Carryall,
					EntityIcon::Ornithopter
				});
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
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

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					EntityIcon::Infantry,
					EntityIcon::Trooper,
					EntityIcon::Troopers
				});
				break;

				case StructureInfo::STARPORT: previews.insert(previews.end(), 
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
			case StructureInfo::VEHICLE: previews.insert(previews.end(), 
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

			case StructureInfo::HIGH_TECH: previews.insert(previews.end(), 
			{
				EntityIcon::Carryall,
				EntityIcon::Ornithopter
			});
			break;

			case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
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

			case StructureInfo::BARRACKS: previews.insert(previews.end(), 
			{
				EntityIcon::Infantry,
				EntityIcon::Trooper,
				EntityIcon::Troopers
			});
			break;

			case StructureInfo::STARPORT: previews.insert(previews.end(), 
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
				case StructureInfo::VEHICLE:
					remove_previews(previews, 
					{
						EntityIcon::Raider_Trike,
						EntityIcon::Deviator,
						EntityIcon::Devastator
					});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD:
				break;

				case StructureInfo::BARRACKS:
					remove_previews(previews, 
					{
						EntityIcon::Trooper,
						EntityIcon::Troopers
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
						EntityIcon::Trike,
						EntityIcon::Launcher,
						EntityIcon::Sonic_Tank,
						EntityIcon::Devastator
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
						EntityIcon::Raider_Trike,
						EntityIcon::Trike,
						EntityIcon::Sonic_Tank,
						EntityIcon::Deviator
					});
				break;

				case StructureInfo::HIGH_TECH:
					remove_previews(previews, 
					{
						EntityIcon::Ornithopter
					});
				break;

				case StructureInfo::CONSTRUCTION_YARD:
				break;

				case StructureInfo::BARRACKS:
					remove_previews(previews, 
					{
						EntityIcon::Infantry
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