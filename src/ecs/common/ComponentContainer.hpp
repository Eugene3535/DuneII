#ifndef COMPONENT_CONTAINER_HPP
#define COMPONENT_CONTAINER_HPP

#include "ecs/common/SparseSet.hpp"

BEGIN_NAMESPACE_ECS

template<class T>
using ComponentSparseSet = SparseSet<component_id_t, T>;

class BaseComponentContainer
{
public:
    virtual ~BaseComponentContainer() = default;
    virtual class BaseComponent* get(component_id_t id) noexcept { return nullptr; }
    virtual void remove(component_id_t id) noexcept {}
};

template<class T>
class ComponentContainer : public BaseComponentContainer
{
public:
    class BaseComponent* get(component_id_t id) noexcept override
    {
        return &components[id];
    }

    void remove(component_id_t id) noexcept override
    {
        components.erase(id);
    }

public:
    ComponentSparseSet<T> components;
};

END_NAMESPACE_ECS

#endif // !COMPONENT_CONTAINER_HPP