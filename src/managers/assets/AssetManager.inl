template<class T>
T* AssetManager::tryLoadFromFile(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept
{
    const auto filepath = FileProvider().findPathToFile(filename);

    if(filepath.empty())
        return nullptr;

    auto [iterator, result] = container.try_emplace(filename);

    if(result)
    {
        if (!iterator->second.loadFromFile(filepath.generic_string()))
        {
            container.erase(iterator);

            return nullptr;
        }

        return &iterator->second;
    }

    return nullptr;
}