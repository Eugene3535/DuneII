#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <string>
#include <vector>
#include <utility>
#include <type_traits>

#include "managers/ecs/common/Components.hpp"
#include "managers/ecs/common/ComponentTable.hpp"

BEGIN_NAMESPACE_ECS

class EntityManager
{
    using PositionContainer  = std::vector<std::pair<int32_t, Position>>;
    using VelocityContainer  = std::vector<std::pair<int32_t, Velocity>>;
    using AnimationContainer = std::vector<std::pair<int32_t, Animation>>;
    using NameContainer      = std::vector<std::pair<int32_t, std::string>>;

public:
    EntityManager() noexcept;

    int32_t createEntity()                 noexcept;
    void     destroyEntity(int32_t entity) noexcept;

    template<class T, class... Args>
    T* addComponent(int32_t entity, Args&&... args) noexcept;

    template<class T>
    T* getComponent(int32_t entity) noexcept;

    template<class T>
    void removeComponent(int32_t entity) noexcept;

private:
    template<class T>
    void removeComponentFrom(int32_t i, T& container) noexcept;

public:
    std::vector<int32_t>        entities;
    std::vector<ComponentTable> tables;

public:
    PositionContainer  positions;
    VelocityContainer  velocities;
    AnimationContainer animations;
    NameContainer      names;
};

#include "managers/ecs/EntityManager.inl"

END_NAMESPACE_ECS

#endif // !ENTITY_MANAGER_HPP
