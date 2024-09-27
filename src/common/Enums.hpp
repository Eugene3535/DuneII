#ifndef ENUMS_HPP
#define ENUMS_HPP

enum class HouseType 
{
	HARKONNEN = 0,
	ATREIDES,
	ORDOS,
	FREMEN,
	SARDAUKAR,
	MERCENARY,
	MAX,
	INVALID = 0xFF
};

enum class StructureType 
{
	SLAB_1x1 = 0,
	SLAB_2x2,
	PALACE,
	VEHICLE,
	HIGH_TECH,
	CONSTRUCTION_YARD,
	WIND_TRAP,
	BARRACKS,
	STARPORT,
	REFINERY,
	REPAIR,
	WALL,
	TURRET,
	ROCKET_TURRET,
	SILO,
	OUTPOST,
	MAX,
	INVALID = 0xFF
};

enum class UnitType
{
    CARRYALL = 0,
    ORNITHOPTER,
    INFANTRY,
    TROOPERS,
    SOLDIER,
    TROOPER,
    SABOTEUR,
    LAUNCHER,
    DEVIATOR,
    TANK,
    SIEGE_TANK,
    DEVASTATOR,
    SONIC_TANK,
    TRIKE,
    RAIDER_TRIKE,
    QUAD,
    HARVESTER,
    MCV,
    MISSILE_HOUSE,
    MISSILE_ROCKET,
    MISSILE_TURRET,
    MISSILE_DEVIATOR,
    MISSILE_TROOPER,
    BULLET,
    SONIC_BLAST,
    SANDWORM,
    MAX,
    INVALID = 0xFF
};

enum class House : unsigned int
{
    Atreides = 1 << 1,
    Ordos = 1 << 2,
    Harkonnen = 1 << 3,
    Fremen = 1 << 4
};

#endif // !ENUMS_HPP