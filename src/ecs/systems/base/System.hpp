#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <cstdint>

#include "common/Defines.hpp"

BEGIN_NAMESPACE_ECS
class EntityManager;
END_NAMESPACE_ECS
class BaseSystem
{
public:
    BaseSystem(ecs::EntityManager& entityManager) noexcept;
    virtual ~BaseSystem() = default;
    virtual void execute() noexcept;
    static uint32_t getSystemCount() noexcept;

protected:
    template<class T> 
    static uint32_t generateComponentType() noexcept
    {
        uint32_t tmp = s_type;
        ++s_type;

        return tmp;
    }

protected:
    ecs::EntityManager& m_entityManager;

private:
    static uint32_t s_type;
};

template<class T>
class System : public BaseSystem
{
public:
    System(ecs::EntityManager& entityManager) noexcept:
        BaseSystem(entityManager)
    {

    }

public:
    static const uint32_t Type;
};

template<class T>
const uint32_t System<T>::Type = BaseSystem::generateComponentType<T>();

#endif // !SYSTEM_HPP