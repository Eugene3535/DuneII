#ifndef COMPONENT_CONTAINER_HPP
#define COMPONENT_CONTAINER_HPP

#include "ecs/common/SparseSet.hpp"

BEGIN_NAMESPACE_ECS

template<class T>
using ComponentSparseSet = SparseSet<ComponentId, T>;

class BaseComponentContainer
{
public:
    virtual ~BaseComponentContainer() = default;
    virtual class BaseComponent* get(ComponentId id) noexcept { return nullptr; }
    virtual void remove(ComponentId id) noexcept {}
};

template<class T>
class ComponentContainer : public BaseComponentContainer
{
public:
    class BaseComponent* get(ComponentId id) noexcept override
    {
        return &components[id];
    }

    void remove(ComponentId id) noexcept override
    {
        components.erase(id);
    }

public:
    ComponentSparseSet<T> components;
};

END_NAMESPACE_ECS

#endif // !COMPONENT_CONTAINER_HPP