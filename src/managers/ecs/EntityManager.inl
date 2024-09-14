template<class T, class... Args>
T* EntityManager::addComponent(int32_t entity, Args&&... args) noexcept
{
    auto& table = tables[static_cast<size_t>(entity)];

    if constexpr(std::is_same_v<T, Position>)
    {
        if (table.position == UNDEFINED_COMPONENT)
        {
            table.position = positions.size();
            auto& pair = positions.emplace_back(entity, args...);
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same_v<T, Velocity>)
    {
        if (table.velocity == UNDEFINED_COMPONENT)
        {
            table.velocity = velocities.size();
            auto& pair = velocities.emplace_back(entity, args...);
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same_v<T, Animation>)
    {
        if (table.animation == UNDEFINED_COMPONENT)
        {
            table.animation = animations.size();
            auto& pair = animations.emplace_back(entity, args...);
            pair.first = entity;

            return &pair.second;
        }
    }
    else if constexpr(std::is_same_v<T, std::string>)
    {
        if (table.name == UNDEFINED_COMPONENT)
        {
            table.name = names.size();
            auto& pair = names.emplace_back(entity, args...);
            pair.first = entity;

            return &pair.second;
        }
    }

    return nullptr;
}

template<class T>
T* EntityManager::getComponent(int32_t entity) noexcept
{
    auto& table = tables[static_cast<size_t>(entity)];

    if constexpr (std::is_same_v<T, Position>)
    {
        if (int32_t i = table.position; i != UNDEFINED_COMPONENT)
            return &positions[static_cast<size_t>(i)].second;
    }
    else if constexpr (std::is_same_v<T, Velocity>)
    {
        if (int32_t i = table.velocity; i != UNDEFINED_COMPONENT)
            return &velocities[static_cast<size_t>(i)].second;
    }
    else if constexpr (std::is_same_v<T, Animation>)
    {
        if (int32_t i = table.animation; i != UNDEFINED_COMPONENT)
            return &animations[static_cast<size_t>(i)].second;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        if (int32_t i = table.name; i != UNDEFINED_COMPONENT)
            return &names[static_cast<size_t>(i)].second;
    }

    return nullptr;
}

template<class T>
void EntityManager::removeComponent(int32_t entity) noexcept
{
    auto& table = tables[static_cast<size_t>(entity)];

    if constexpr (std::is_same_v<T, Position>)
    {
        if (int32_t i = table.position; i != UNDEFINED_COMPONENT)
        {
            removeComponentFrom<PositionContainer>(i, positions);
            table.position = UNDEFINED_COMPONENT;
        }
    }
    else if constexpr (std::is_same_v<T, Velocity>)
    {
        if (int32_t i = table.velocity; i != UNDEFINED_COMPONENT)
        {
            removeComponentFrom<VelocityContainer>(i, velocities);
            table.velocity = UNDEFINED_COMPONENT;
        }
    }
    else if constexpr (std::is_same_v<T, Animation>)
    {
        if (int32_t i = table.animation; i != UNDEFINED_COMPONENT)
        {
            removeComponentFrom<AnimationContainer>(i, animations);
            table.animation = UNDEFINED_COMPONENT;
        }
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        if (int32_t i = table.name; i != UNDEFINED_COMPONENT)
        {
            removeComponentFrom<NameContainer>(i, names);
            table.name = UNDEFINED_COMPONENT;
        }
    }
}

template<class T>
void EntityManager::removeComponentFrom(int32_t i, T& container) noexcept
{
    auto& pos = container[static_cast<size_t>(i)];
    auto& back = container.back();
    size_t index = static_cast<size_t>(back.first);

    if constexpr (std::is_same_v<T, Position>)
    {
        tables[index].position = i;
    }
    else if constexpr (std::is_same_v<T, Velocity>)
    {
        tables[index].velocity = i;
    }
    else if constexpr (std::is_same_v<T, Animation>)
    {
        tables[index].animation = i;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        tables[index].name = i;
    }
    
    std::swap(pos, back);
    container.pop_back();
}