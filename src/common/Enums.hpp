#pragma once

#include <cstdint>

enum class HouseType : uint32_t
{
    Atreides = 0,
    Ordos,
	Harkonnen,
    Sardaukar,
	Fremen,
	Mercenary,
	MAX,
	INVALID = 0xff
};


enum class EntityIcon : uint32_t
{
	WOR = 0,
	Wind_Trap,
	Wall,
	Death_Hand,
	Quad,
	Harvester,
	Light_Vehicle_Factory,
	Starport,
	Turret,
	Slab_2x2,
	Raider_Trike,
	MCV,
	House_of_IX,
	Spice_Silo,
	Rocket_Turret,
	Construction_Yard,
	Launcher,
	Carryall,
	High_Tech,
	Repair,
	Refinery,
	Barracks,
	Siege_Tank,
	Starport_Intro,
	Heavy_Vehicle_Factory,
	Palace,
	Outpost,
	Fremen,
	Sonic_Tank,
	Devastator,
	Saboteur,
	Sandworm,
	Trooper,
	Troopers,
	Trike,
	Deviator,
	Ornithopter,
	Infantry,
	Tank,
	Empty_Cell,
	MAX,
	INVALID = 0xFF
};