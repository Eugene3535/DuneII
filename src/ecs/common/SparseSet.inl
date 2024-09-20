template<class Id, class T>
template<class ...Args>
std::pair<Id, T&> SparseSet<Id, T>::emplace(Args&& ...args) noexcept
{
    // Add object
    size_t i = m_objects.size();
    auto& object = m_objects.emplace_back(std::forward<Args>(args)...);
    // Get the correct id and set the links
    Id id;
    if (m_freeIds.empty())
    {
        id = static_cast<Id>(m_idToIndex.size());
        m_idToIndex.push_back(i);
    }
    else
    {
        id = m_freeIds.back();
        m_freeIds.pop_back();
        m_idToIndex[static_cast<size_t>(id)] = i;
    }
    m_indexToId.push_back(id);
    
    return std::pair<Id, T&>(id, object);
}

template<class Id, class T>
void SparseSet<Id, T>::erase(Id id) noexcept
{
    // Get the index of the object to destroy
    size_t i = m_idToIndex[static_cast<size_t>(id)];
    // Swap with the last object and update its index
    std::swap(m_objects[i], m_objects.back());

    Id lastObjectId = m_indexToId.back();
    m_idToIndex[static_cast<size_t>(lastObjectId)] = i;
    m_indexToId[i] = lastObjectId;
    // Erase the last object and its index
    m_objects.pop_back();
    m_indexToId.pop_back();
    // Assign Undefined to the id
    m_idToIndex[static_cast<size_t>(id)] = Undefined;
    // Add the deleted Id to the free Ids
    m_freeIds.push_back(id);
}

template<class Id, class T>
bool SparseSet<Id, T>::has(Id id) const noexcept
{
    return static_cast<size_t>(id) < m_idToIndex.size() &&
        m_idToIndex[static_cast<size_t>(id)] != Undefined;
}

template<class Id, class T>
std::vector<T>& SparseSet<Id, T>::getObjects() noexcept
{
    return m_objects;
}

template<class Id, class T>
const std::vector<T>& SparseSet<Id, T>::getObjects() const noexcept
{
    return m_objects;
}

template<class Id, class T>
size_t SparseSet<Id, T>::getSize() const noexcept
{
    return m_objects.size();
}

template<class Id, class T>
void SparseSet<Id, T>::reserve(size_t size) noexcept
{
    m_idToIndex.reserve(size);
    m_freeIds.reserve(size);
    m_objects.reserve(size);
    m_indexToId.reserve(size);
}

template<class Id, class T>
T& SparseSet<Id, T>::operator [](Id id) noexcept
{
    return m_objects[m_idToIndex[static_cast<size_t>(id)]];
}

template<class Id, class T>
const T& SparseSet<Id, T>::operator [](Id id) const noexcept
{
    return m_objects[m_idToIndex[static_cast<size_t>(id)]];
}