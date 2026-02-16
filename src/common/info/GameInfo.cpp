#include <array>
#include <unordered_set>
#include <cassert>
#include <algorithm>

#include "common/info/GameInfo.hpp"


GameInfo::GameInfo() noexcept = default;


std::vector<PreviewType> GameInfo::getPreviewIconList(HouseType house, StructureInfo::Type building, uint32_t mission) const noexcept
{
	assert(mission > 0);
	assert(mission < 10);

	// Filtering available entities by mission, excluding 8 and 9 (all entities are available)
	// No research is conducted in missions 8 and 9.

	std::vector<PreviewType> previews;

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
					PreviewType::Slab_2x2,
					PreviewType::Wind_Trap,
					PreviewType::Refinery
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
					PreviewType::Raider_Trike,
					PreviewType::Trike
				});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					PreviewType::Slab_2x2,
					PreviewType::Wind_Trap,
					PreviewType::Refinery,
					PreviewType::Outpost,
					PreviewType::Spice_Silo,
					PreviewType::Barracks,
					PreviewType::Light_Vehicle_Factory
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					PreviewType::Infantry
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
					PreviewType::Raider_Trike,
					PreviewType::Trike,
					PreviewType::Quad
				});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					PreviewType::Slab_2x2,
					PreviewType::Wind_Trap,
					PreviewType::Refinery,
					PreviewType::Outpost,
					PreviewType::Spice_Silo,
					PreviewType::Barracks,
					PreviewType::Light_Vehicle_Factory
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					PreviewType::Trooper,
					PreviewType::Infantry
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
					PreviewType::Raider_Trike,
					PreviewType::Trike,
					PreviewType::Quad,
					PreviewType::Harvester,
					PreviewType::Tank,
					PreviewType::MCV
				});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					PreviewType::Slab_2x2,
					PreviewType::Wind_Trap,
					PreviewType::Refinery,
					PreviewType::Outpost,
					PreviewType::Spice_Silo,
					PreviewType::Barracks,
					PreviewType::Light_Vehicle_Factory,
					PreviewType::Wall
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					PreviewType::Infantry,
					PreviewType::Trooper,
					PreviewType::Troopers
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
					PreviewType::Raider_Trike,
					PreviewType::Trike,
					PreviewType::Quad,
					PreviewType::Harvester,
					PreviewType::Tank,
					PreviewType::MCV,
					PreviewType::Launcher
				});
				break;

				case StructureInfo::HIGH_TECH: previews.insert(previews.end(), {
				                                                       PreviewType::Carryall
				                                                    });
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					PreviewType::Slab_2x2,
					PreviewType::Wind_Trap,
					PreviewType::Refinery,
					PreviewType::Outpost,
					PreviewType::Spice_Silo,
					PreviewType::Barracks,
					PreviewType::Light_Vehicle_Factory,
					PreviewType::Turret,
					PreviewType::Wall,
					PreviewType::Repair,
					PreviewType::High_Tech
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					PreviewType::Infantry,
					PreviewType::Trooper,
					PreviewType::Troopers
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
					PreviewType::Trike,
					PreviewType::Raider_Trike,
					PreviewType::Quad,
					PreviewType::Harvester,
					PreviewType::Tank,
					PreviewType::MCV,
					PreviewType::Launcher,
					PreviewType::Siege_Tank
				});
				break;

				case StructureInfo::HIGH_TECH: previews.insert(previews.end(), 
				{
					PreviewType::Carryall
				});
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					PreviewType::Slab_2x2,
					PreviewType::Wind_Trap,
					PreviewType::Refinery,
					PreviewType::Outpost,
					PreviewType::Spice_Silo,
					PreviewType::Barracks,
					PreviewType::Light_Vehicle_Factory,
					PreviewType::Turret,
					PreviewType::Wall,
					PreviewType::Repair,
					PreviewType::Rocket_Turret,
					PreviewType::High_Tech,
					PreviewType::Starport
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					PreviewType::Infantry,
					PreviewType::Trooper,
					PreviewType::Troopers
				});
				break;

				case StructureInfo::STARPORT: previews.insert(previews.end(), 
				{
					PreviewType::Trike,
					PreviewType::Quad,
					PreviewType::Harvester,
					PreviewType::Tank,
					PreviewType::MCV,
					PreviewType::Launcher,
					PreviewType::Siege_Tank
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
					PreviewType::Trike,
					PreviewType::Raider_Trike,
					PreviewType::Quad,
					PreviewType::Harvester,
					PreviewType::Tank,
					PreviewType::MCV,
					PreviewType::Launcher,
					PreviewType::Siege_Tank,
					PreviewType::Sonic_Tank,
					PreviewType::Devastator,
					PreviewType::Deviator
				});
				break;

				case StructureInfo::HIGH_TECH: previews.insert(previews.end(), 
				{
					PreviewType::Carryall,
					PreviewType::Ornithopter
				});
				break;

				case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
				{
					PreviewType::Slab_2x2,
					PreviewType::Wind_Trap,
					PreviewType::Refinery,
					PreviewType::Outpost,
					PreviewType::Spice_Silo,
					PreviewType::Barracks,
					PreviewType::Light_Vehicle_Factory,
					PreviewType::Turret,
					PreviewType::Wall,
					PreviewType::Repair,
					PreviewType::Rocket_Turret,
					PreviewType::High_Tech,
					PreviewType::Starport
				});
				break;

				case StructureInfo::BARRACKS: previews.insert(previews.end(), 
				{
					PreviewType::Infantry,
					PreviewType::Trooper,
					PreviewType::Troopers
				});
				break;

				case StructureInfo::STARPORT: previews.insert(previews.end(), 
				{
					PreviewType::Trike,
					PreviewType::Quad,
					PreviewType::Harvester,
					PreviewType::Tank,
					PreviewType::MCV,
					PreviewType::Launcher,
					PreviewType::Siege_Tank,
					PreviewType::Ornithopter
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
				PreviewType::Trike,
				PreviewType::Raider_Trike,
				PreviewType::Quad,
				PreviewType::Harvester,
				PreviewType::Tank,
				PreviewType::MCV,
				PreviewType::Launcher,
				PreviewType::Siege_Tank,
				PreviewType::Sonic_Tank,
				PreviewType::Deviator,
				PreviewType::Devastator
			});
			break;

			case StructureInfo::HIGH_TECH: previews.insert(previews.end(), 
			{
				PreviewType::Carryall,
				PreviewType::Ornithopter
			});
			break;

			case StructureInfo::CONSTRUCTION_YARD: previews.insert(previews.end(), 
			{
				PreviewType::Slab_2x2,
				PreviewType::Wind_Trap,
				PreviewType::Refinery,
				PreviewType::Outpost,
				PreviewType::Spice_Silo,
				PreviewType::Barracks,
				PreviewType::Light_Vehicle_Factory,
				PreviewType::Turret,
				PreviewType::Wall,
				PreviewType::Repair,
				PreviewType::Rocket_Turret,
				PreviewType::High_Tech,
				PreviewType::Starport,
				PreviewType::Palace
			});
			break;

			case StructureInfo::BARRACKS: previews.insert(previews.end(), 
			{
				PreviewType::Infantry,
				PreviewType::Trooper,
				PreviewType::Troopers
			});
			break;

			case StructureInfo::STARPORT: previews.insert(previews.end(), 
			{
				PreviewType::Trike,
				PreviewType::Quad,
				PreviewType::Harvester,
				PreviewType::Tank,
				PreviewType::MCV,
				PreviewType::Launcher,
				PreviewType::Siege_Tank,
				PreviewType::Ornithopter
			});
			break;

			default:
				break;
		}
	}

//  Filtering by house
	auto remove_previews = [](std::vector<PreviewType>& vec, std::initializer_list<PreviewType> values) -> void
	{
		std::unordered_set<PreviewType> to_remove(values);

		vec.erase(std::remove_if(vec.begin(), vec.end(),
			[&to_remove](PreviewType type) { return to_remove.contains(type); }), vec.end());
	};

	switch (house)
	{
		case HouseType::Atreides:
			switch (building)
			{
				case StructureInfo::VEHICLE:
					remove_previews(previews, 
					{
						PreviewType::Raider_Trike,
						PreviewType::Deviator,
						PreviewType::Devastator
					});
				break;

				case StructureInfo::HIGH_TECH:
				break;

				case StructureInfo::CONSTRUCTION_YARD:
				break;

				case StructureInfo::BARRACKS:
					remove_previews(previews, 
					{
						PreviewType::Trooper,
						PreviewType::Troopers
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
						PreviewType::Trike,
						PreviewType::Launcher,
						PreviewType::Sonic_Tank,
						PreviewType::Devastator
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
						PreviewType::Raider_Trike,
						PreviewType::Trike,
						PreviewType::Sonic_Tank,
						PreviewType::Deviator
					});
				break;

				case StructureInfo::HIGH_TECH:
					remove_previews(previews, 
					{
						PreviewType::Ornithopter
					});
				break;

				case StructureInfo::CONSTRUCTION_YARD:
				break;

				case StructureInfo::BARRACKS:
					remove_previews(previews, 
					{
						PreviewType::Infantry
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