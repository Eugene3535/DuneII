#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <entt/entt.hpp>

class BaseSystem
{
public:
    BaseSystem(entt::registry& registry) noexcept;
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
    entt::registry& m_registry;

private:
    static uint32_t s_type;
};

template<class T>
class System : public BaseSystem
{
public:
    System(entt::registry& registry) noexcept:
        BaseSystem(registry)
    {

    }

public:
    static const uint32_t Type;
};

template<class T>
const uint32_t System<T>::Type = BaseSystem::generateComponentType<T>();

#endif // !SYSTEM_HPP