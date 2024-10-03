#include "ecs/systems/MovementController.hpp"

MovementController::MovementController(entt::registry& registry) noexcept :
    System(registry)
{
    
}

MovementController::~MovementController() = default;

void MovementController::execute(sf::Time dt) noexcept
{
    
}