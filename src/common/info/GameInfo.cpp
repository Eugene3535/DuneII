#include <array>

#include "common/info/GameInfo.hpp"


// PreviewType::WOR,
// PreviewType::Wind_Trap,
// PreviewType::Wall,
// PreviewType::Death_Hand,
// PreviewType::Quad,
// PreviewType::Harvester,
// PreviewType::Light_Vehicle_Factory,
// PreviewType::Starport,
// PreviewType::Turret,
// PreviewType::Slab_2x2,
// PreviewType::Raider_Trike,
// PreviewType::MCV,
// PreviewType::House_of_IX,
// PreviewType::Spice_Silo,
// PreviewType::Rocket_Turret,
// PreviewType::Construction_Yard,
// PreviewType::Launcher,
// PreviewType::Carryall,
// PreviewType::High_Tech,
// PreviewType::Repair,
// PreviewType::Refinery,
// PreviewType::Barracks,
// PreviewType::Siege_Tank,
// PreviewType::Starport_Intro,
// PreviewType::Heavy_Vehicle_Factory,
// PreviewType::Palace,
// PreviewType::Outpost,
// PreviewType::Fremen,
// PreviewType::Sonic_Tank,
// PreviewType::Devastator,
// PreviewType::Saboteur,
// PreviewType::Sandworm,
// PreviewType::Trooper,
// PreviewType::Troopers,
// PreviewType::Trike,
// PreviewType::Deviator,
// PreviewType::Ornithopter,
// PreviewType::Infantry,
// PreviewType::Tank,
// PreviewType::Empty_Cell


static constexpr std::array<PreviewType, 14> get_construction_yard_previews() noexcept
{
	return 
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
	};
}


static constexpr std::array<PreviewType, 2> get_hi_tech_previews() noexcept
{
	return 
	{
		PreviewType::Carryall,
		PreviewType::Ornithopter
	};
}


static constexpr std::array<PreviewType, 11> get_vehicle_previews() noexcept
{
	return 
	{
		PreviewType::Trike,
		PreviewType::Raider_Trike,
		PreviewType::Quad,
		PreviewType::Tank,
		PreviewType::Harvester,
		PreviewType::MCV,
		PreviewType::Launcher,
		PreviewType::Siege_Tank,
		PreviewType::Sonic_Tank,
		PreviewType::Devastator,
		PreviewType::Deviator
	};
}


static constexpr std::array<PreviewType, 3> get_barracks_previews() noexcept
{
	return 
	{
		PreviewType::Trooper,
		PreviewType::Troopers,
		PreviewType::Infantry
	};
}


static constexpr std::array<PreviewType, 7> get_starport_previews() noexcept
{
	return 
	{
		PreviewType::Trike,
		PreviewType::Quad,
		PreviewType::Tank,
		PreviewType::Harvester,
		PreviewType::MCV,
		PreviewType::Launcher,
		PreviewType::Siege_Tank
	};
}



GameInfo::GameInfo() noexcept = default;


std::vector<PreviewType> GameInfo::getPreviewIconList(HouseType house, StructureInfo::Type building, uint32_t mission) const noexcept
{
    std::vector<PreviewType> previews;

	switch (building)
	{
		case StructureInfo::VEHICLE:
			auto vehicle = get_vehicle_previews();
			previews.insert(previews.end(), vehicle.begin(), vehicle.end());
		break;

		case StructureInfo::HIGH_TECH:
			auto hi_tech = get_hi_tech_previews();
			previews.insert(previews.end(), hi_tech.begin(), hi_tech.end());
		break;

		case StructureInfo::CONSTRUCTION_YARD:
			auto buildings = get_construction_yard_previews();
			previews.insert(previews.end(), buildings.begin(), buildings.end());
		break;

		case StructureInfo::BARRACKS:
			auto cannon_fodder = get_barracks_previews();
			previews.insert(previews.end(), cannon_fodder.begin(), cannon_fodder.end());
		break;

		case StructureInfo::STARPORT:
			auto delivery = get_starport_previews();
			previews.insert(previews.end(), delivery.begin(), delivery.end());
		break;

		default:
			break;
	}

    return previews;
}