#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <memory>

#include "ecs/common/ComponentContainer.hpp"

BEGIN_NAMESPACE_ECS

class BaseComponent
{
public:
    static size_t getComponentCount() noexcept
    {
        return s_factories.size();
    }

    static std::unique_ptr<BaseComponentContainer> createComponentContainer(size_t type) noexcept
    {
        return s_factories[type]();
    }

protected:
    template<class T> 
    static ComponentType generateComponentType() noexcept
    {
        s_factories.push_back([]() -> std::unique_ptr<BaseComponentContainer>
        {
            return std::make_unique<ComponentContainer<T>>();
        });

        return static_cast<ComponentType>(s_factories.size() - 1);
    }

private:
    using ComponentContainerFactory = std::unique_ptr<BaseComponentContainer>(*)();

    static std::vector<ComponentContainerFactory> s_factories;
};

inline std::vector<BaseComponent::ComponentContainerFactory> BaseComponent::s_factories;

template<class T>
class Component : public BaseComponent
{
public:
    static const ComponentType Type;
};

template<class T>
const ComponentType Component<T>::Type = BaseComponent::generateComponentType<T>();

template<class T>
constexpr void checkComponentType()
{
    static_assert(std::is_base_of_v<Component<T>, T>, "A component of type T must inherit Component<T>");
}

template<class ...Ts>
constexpr void checkComponentTypes()
{
    (checkComponentType<Ts>(), ...);
}

END_NAMESPACE_ECS

#endif // !COMPONENT_HPP