template<class T>
T* AssetManager::tryLoadFromFile(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept
{
    const auto filepath = FileProvider().findPathToFile(filename);

    if(filepath.empty())
        return nullptr;

    auto [iterator, result] = container.try_emplace(filename);

    if(result)
    {
        bool sucsess = false;

        if constexpr (std::is_same<T, sf::Music>::value)
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