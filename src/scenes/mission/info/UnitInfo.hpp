#pragma once

#include <cglm/struct/ivec2.h>

struct UnitInfo
{
    enum class Type : int
    {
        Carryall = 0,
        Ornithopter,
        Infantry,
        Troopers,
        Soldier,
        Trooper,
        Saboteur,
        Launcher,
        Deviator,
        Tank,
        SiegeTank,
        Devastator,
        SonicTank,
        Trike,
        RaiderTrike,
        Quad,
        Harvester,
        Mcv,
        MissileHouse,
        MissileRocket,
        MissileTurret,
        MissileDeviator,
        MissileTrooper,
        Bullet,
        Sonic_Blast,
        Sandworm,
        Max,
        Invalid = 0xff
    } type;

    ivec2s cell;
    vec2s velocity;
    const float coef = 0.707f; // Diagonal movement speed coefficient (for left, up, right, down is equal 1.0f)
};