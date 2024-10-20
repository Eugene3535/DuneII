template<class T>
T* AssetManager::getResource(const std::string& filename) noexcept
{
    if (m_instance)
    {
        if constexpr(std::is_same_v<T, sf::Texture>)
		{
            if(auto found = m_textures.find(filename); found != m_textures.end())
                return &found->second;

            return tryLoadFromFile<sf::Texture>(filename, m_textures);
		}
        else if constexpr(std::is_same_v<T, sf::Font>)
        {
            if(auto found = m_fonts.find(filename); found != m_fonts.end())
                return &found->second;

            return tryLoadFromFile<sf::Font>(filename, m_fonts);
        }
        else if constexpr(std::is_same_v<T, sf::SoundBuffer>)
        {
            if(auto found = m_soundBuffers.find(filename); found != m_soundBuffers.end())
                return &found->second;

            return tryLoadFromFile<sf::SoundBuffer>(filename, m_soundBuffers);
        }
        else if constexpr(std::is_same_v<T, sf::Music>)
        {
            if(auto found = m_musicBox.find(filename); found != m_musicBox.end())
                return &found->second;
            
            return tryLoadFromFile<sf::Music>(filename, m_musicBox);
        }
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
        bool sucsess = false;

        if constexpr (std::is_same_v<T, sf::Music>)
		{
            sucsess = iterator->second.openFromFile(filepath.generic_string());
		}
        else
        {
            sucsess = iterator->second.loadFromFile(filepath.generic_string());
        }

        if (sucsess)
        {
            return &iterator->second;
        }

        container.erase(iterator);
    }

    return nullptr;
}