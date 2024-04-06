    template<class T>
    T* Assets::try_load_from_file(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept
    {
        const auto filepath = FileProvider().find_path_to_file(filename);

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
