#include <algorithm>

#include "managers/ecs/EntityManager.hpp"

BEGIN_NAMESPACE_ECS

int32_t EntityManager::createEntity() noexcept
{
    int32_t entity = static_cast<int32_t>(entities.size());
    entities.push_back(entity);
    tables.emplace_back();

    return entity;
}

void EntityManager::destroyEntity(int32_t entity) noexcept
{
    removeComponent<Position>(entity);
    removeComponent<Velocity>(entity);
    removeComponent<Animation>(entity);
    removeComponent<std::string>(entity);
    
    entities[static_cast<size_t>(entity)] = entities.back();
    std::swap(tables[static_cast<size_t>(entity)], tables.back());

    entities.pop_back();
    tables.pop_back();
}

END_NAMESPACE_ECS