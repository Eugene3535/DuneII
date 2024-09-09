template<class ...Ts>
entity_set_t BaseEntitySet::generateEntitySetType() noexcept
{
    s_factories.push_back([](EntityContainer& entities,
        const std::vector<std::unique_ptr<BaseComponentContainer>>& componentContainers,
        std::vector<std::vector<BaseEntitySet*>>& componentToEntitySets)
        -> std::unique_ptr<BaseEntitySet>
    {
        auto entitySet = std::make_unique<EntitySet<Ts...>>(entities,
            std::tie(static_cast<ComponentContainer<Ts>*>(componentContainers[Ts::Type].get())->components...));
            
        (componentToEntitySets[Ts::Type].push_back(entitySet.get()), ...);

        return std::move(entitySet);
    });
    
    return s_factories.size() - 1;
}

template<class ...Ts>
EntitySet<Ts ...>::EntitySet(EntityContainer& entities, const ComponentContainers& componentContainers) noexcept :
    m_entities(entities), 
    m_componentContainers(componentContainers)
{

}

template<class ...Ts>
size_t EntitySet<Ts ...>::getSize() const noexcept
{
    return m_managedEntities.size();
}

template<class ...Ts>
typename EntitySet<Ts ...>::Iterator EntitySet<Ts ...>::begin() noexcept
{
    return Iterator(m_managedEntities.begin(), m_componentContainers);
}

template<class ...Ts>
typename EntitySet<Ts ...>::ConstIterator EntitySet<Ts ...>::begin() const noexcept
{
    return ConstIterator(m_managedEntities.begin(), m_componentContainers);
}

template<class ...Ts>
typename EntitySet<Ts ...>::Iterator EntitySet<Ts ...>::end() noexcept
{
    return Iterator(m_managedEntities.end(), m_componentContainers);
}

template<class ...Ts>
typename EntitySet<Ts ...>::ConstIterator EntitySet<Ts ...>::end() const noexcept
{
    return ConstIterator(m_managedEntities.end(), m_componentContainers);
}

// Listeners
template<class ...Ts>
typename EntitySet<Ts ...>::ListenerId EntitySet<Ts ...>::addEntityAddedListener(EntityAddedListener listener) noexcept
{
    return m_entityAddedListeners.emplace(std::move(listener)).first;
}

template<class ...Ts>
void EntitySet<Ts ...>::removeEntityAddedListener(ListenerId id) noexcept
{
    m_entityAddedListeners.erase(id);
}

template<class ...Ts>
typename EntitySet<Ts ...>::ListenerId EntitySet<Ts ...>::addEntityRemovedListener(EntityRemovedListener listener) noexcept
{
    return m_entityRemovedListeners.emplace(std::move(listener)).first;
}

template<class ...Ts>
void EntitySet<Ts ...>::removeEntityRemovedListener(ListenerId id) noexcept
{
    m_entityRemovedListeners.erase(id);
}

template<class ...Ts>
bool EntitySet<Ts ...>::satisfyRequirements(entity_t entity) noexcept
{
    return m_entities[entity].template hasComponents<Ts...>();
}

template<class ...Ts>
void EntitySet<Ts ...>::addEntity(entity_t entity) noexcept
{
    m_entityToIndex[entity] = m_managedEntities.size();
    auto& entityData = m_entities[entity];
    entityData.addEntitySet(Type);
    m_managedEntities.emplace_back(entity, std::array<component_id_t, sizeof...(Ts)>{entityData.template getComponent<Ts>()...});
    
//  Call listeners
    for (const auto& listener : m_entityAddedListeners.getObjects())
        listener(entity);
}

template<class ...Ts>
void EntitySet<Ts ...>::removeEntity(entity_t entity, bool updateEntity) noexcept
{
//  Call listeners
    for (const auto& listener : m_entityRemovedListeners.getObjects())
        listener(entity);

    auto it = m_entityToIndex.find(entity);
    auto index = it->second;
    m_entityToIndex[m_managedEntities.back().first] = index;
    m_entityToIndex.erase(it);
    m_managedEntities[index] = m_managedEntities.back();
    m_managedEntities.pop_back();

    if (updateEntity)
        m_entities[entity].removeEntitySet(Type);
}