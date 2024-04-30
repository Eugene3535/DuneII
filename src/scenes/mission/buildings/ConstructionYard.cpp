#include "scenes/mission/buildings/ConstructionYard.hpp"

ConstructionYard::ConstructionYard() noexcept:
    Building()
{
    m_type = Building::CONSTRUCTION_YARD;
}

ConstructionYard::~ConstructionYard()
{
}

void ConstructionYard::update(std::int32_t dt) noexcept
{

}