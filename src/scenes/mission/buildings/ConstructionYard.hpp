#ifndef CONSTRUCTION_YARD_HPP
#define CONSTRUCTION_YARD_HPP

#include "scenes/mission/buildings/base/Building.hpp"

/// \brief 
/// Build cost : Deploy an MCV which costs 900
/// Builds all other buildings.

class ConstructionYard:
    public Building
{
public:
    ConstructionYard() noexcept;
    ~ConstructionYard();

    void update(std::int32_t dt) noexcept override;

private:

};

#endif // !CONSTRUCTION_YARD_HPP