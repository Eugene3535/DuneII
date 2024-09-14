#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "common/Defines.hpp"

BEGIN_NAMESPACE_ECS

class BaseSystem
{
public:
    BaseSystem(class EntityManager& entityManager) noexcept:
        m_entityManager(entityManager)
    {

    }

    virtual ~BaseSystem() = default;

    virtual void execute() noexcept {}

    static uint32_t getSystemCount() noexcept
    {
        return s_type; 
    }

protected:
    template<class T> 
    static uint32_t generateComponentType() noexcept
    {
        uint32_t tmp = s_type;
        ++s_type;

        return tmp;
    }

protected:
    class EntityManager& m_entityManager;

private:
    static uint32_t s_type;
};

uint32_t BaseSystem::s_type = 0;

template<class T>
class System : public BaseSystem
{
public:
    System(class EntityManager& entityManager) noexcept:
        BaseSystem(entityManager)
    {

    }

public:
    static const uint32_t Type;
};

template<class T>
const uint32_t System<T>::Type = BaseSystem::generateComponentType<T>();

END_NAMESPACE_ECS

#endif // !SYSTEM_HPP