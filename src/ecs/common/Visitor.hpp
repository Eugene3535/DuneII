#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <functional>

#include "Component.hpp"

BEGIN_NAMESPACE_ECS

class Visitor
{
public:
    Visitor() noexcept
    {
        m_handlers.resize(BaseComponent::getComponentCount());
    }

    template<class T, class Callable>
    void setHandler(Callable&& callable) noexcept
    {
        checkComponentType<T>();
        
        m_handlers[T::Type] = [callable = std::forward<Callable>(callable)](BaseComponent& component)
        {
            callable(static_cast<T&>(component));
        };
    }

    void handle(component_t componentType, BaseComponent* component) const noexcept
    {
        if (m_handlers[componentType])
            m_handlers[componentType](*component);
    }

private:
    std::vector<std::function<void(BaseComponent&)>> m_handlers;
};

END_NAMESPACE_ECS

#endif // !VISITOR_HPP
