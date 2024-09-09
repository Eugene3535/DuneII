#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "ecs/common/EntityManager.hpp"

BEGIN_NAMESPACE_ECS

class Entity final
{
public:
    Entity() noexcept;
    ~Entity() = default;

    bool create() noexcept;
    bool exist()   const noexcept;
    void destroy() noexcept;
    entity_t getId() const noexcept;

//  Components
    template<class T, class ...Args>
    T& addComponent(Args&&... args) noexcept;

    template<class T>
    void removeComponent() noexcept;
    
    template<class T>
    bool hasComponent() const noexcept;

    template<class ...Ts>
    bool hasComponents() const noexcept;

    template<class T>
    T& getComponent() noexcept;

    template<class T>
    const T& getComponent() const noexcept;

    template<class ...Ts>
    std::tuple<Ts&...> getComponents() noexcept;

    template<class ...Ts>
    std::tuple<const Ts&...> getComponents() const noexcept;

private:
    static EntityManager* s_entityManager;

private:
    entity_t m_id;
};

#include "ecs/entity/Entity.inl"

END_NAMESPACE_ECS

#endif // !ENTITY_HPP