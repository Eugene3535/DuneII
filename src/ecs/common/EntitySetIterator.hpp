#ifndef ENTITY_SET_ITERATOR_HPP
#define ENTITY_SET_ITERATOR_HPP

#include <array>
#include <tuple>
#include <utility>

#include "ecs/common/SparseSet.hpp"

BEGIN_NAMESPACE_ECS

template<class Iterator, class ...Ts>
class EntitySetIterator
{
    using ComponentContainers = std::tuple<ComponentSparseSet<std::remove_const_t<Ts>>&...>;

public:
    explicit EntitySetIterator(Iterator it, const ComponentContainers& componentContainers) noexcept :
        m_it(it), 
        m_componentContainers(componentContainers)
    {

    }

    bool operator !=(const EntitySetIterator<Iterator, Ts...>& it) noexcept
    {
        return m_it != it.m_it;
    }

    std::pair<entity_t, std::tuple<Ts&...>> operator *() noexcept
    {
        return std::pair(m_it->first, getComponentsByIds(m_it->second, std::index_sequence_for<Ts...>{}));
    }

    EntitySetIterator<Iterator, Ts...>& operator ++() noexcept
    {
        ++m_it;

        return *this;
    }

private:
    template<std::size_t ...Is>
    std::tuple<Ts&...> getComponentsByIds(const std::array<component_id_t, sizeof...(Ts)>& componentIds, std::index_sequence<Is...>) noexcept
    {
        return std::tie(std::get<Is>(m_componentContainers)[componentIds[Is]]...);
    }

    template<std::size_t ...Is>
    std::tuple<const Ts&...> getComponentsByIds(const std::array<component_id_t, sizeof...(Ts)>& componentIds, std::index_sequence<Is...>) const noexcept
    {
        return std::tie(std::as_const(std::get<Is>(m_componentContainers).get(componentIds[Is]))...);
    }

private:
    Iterator m_it;
    const ComponentContainers& m_componentContainers; // MAYBE: just copy the references
};

END_NAMESPACE_ECS

#endif // !ENTITY_SET_ITERATOR_HPP