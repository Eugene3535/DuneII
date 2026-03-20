template<class T>
T* AssetManager::get(const std::string& filename) noexcept
{
    if constexpr(std::is_same_v<T, sf::Texture>)
    {
        if(auto found = m_textures.find(filename); found != m_textures.end())
            return &found->second;

        return tryLoadFromFile<sf::Texture>(filename, m_textures);
    }

    if constexpr(std::is_same_v<T, sf::Font>)
    {
        if(auto found = m_fonts.find(filename); found != m_fonts.end())
            return &found->second;

        return tryLoadFromFile<sf::Font>(filename, m_fonts);
    }

    if constexpr(std::is_same_v<T, sf::SoundBuffer>)
    {
        if(auto found = m_sounds.find(filename); found != m_sounds.end())
            return &found->second;

        return tryLoadFromFile<sf::SoundBuffer>(filename, m_sounds);
    }

    if constexpr(std::is_same_v<T, sf::Music>)
    {
        if(auto found = m_music.find(filename); found != m_music.end())
            return &found->second;
        
        return tryLoadFromFile<sf::Music>(filename, m_music);
    }

    return nullptr;
}

template<class T>
T* AssetManager::tryLoadFromFile(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept
{
    const auto filepath = FileProvider::findPathToFile(filename);

    if(filepath.empty())
        return nullptr;

    if(auto [iterator, result] = container.try_emplace(filename); result)
    {
        bool success = false;

        if constexpr (std::is_same<T, sf::Music>())
		{
            success = iterator->second.openFromFile(filepath);
		}
        else if constexpr (std::is_same<T, sf::Font>())
        {
            success = iterator->second.openFromFile(filepath);
        }
        else if constexpr (std::is_same<T, sf::Texture>())
        {
            success = iterator->second.loadFromFile(filepath);
        }

        if (success)
        {
            return &iterator->second;
        }

        container.erase(iterator);
    }

    return nullptr;
}