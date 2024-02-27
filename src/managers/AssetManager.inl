template<class T>
T* AssetManager::create(const std::string& filename) noexcept
{
	if (m_instance)
	{
//  Textures
		if constexpr (std::is_same<T, sf::Texture>::value)
		{
			if (auto it = m_instance->m_textures.find(filename); it != m_instance->m_textures.end())
				return &it->second;

			return m_instance->tryCreate<sf::Texture>(filename, m_instance->m_textures);
		}
//  Fonts
		else if constexpr (std::is_same<T, sf::Font>::value)
		{
			if (auto it = m_instance->m_fonts.find(filename); it != m_instance->m_fonts.end())
				return &it->second;

			return m_instance->tryCreate<sf::Font>(filename, m_instance->m_fonts);
		}
//  Sounds
		else if constexpr (std::is_same<T, sf::SoundBuffer>::value)
		{
			if (auto it = m_instance->m_sounds.find(filename); it != m_instance->m_sounds.end())
				return &it->second;

			return m_instance->tryCreate<sf::SoundBuffer>(filename, m_instance->m_sounds);
		}
//  Music
		else if constexpr (std::is_same<T, sf::Music>::value)
		{
			if (auto it = m_instance->m_musics.find(filename); it != m_instance->m_musics.end())
				return &it->second;

			return m_instance->tryCreate<sf::Music>(filename, m_instance->m_musics);
		}
//  Shaders    
		else if constexpr (std::is_same<T, sf::Shader>::value)
		{
			if (auto it = m_instance->m_shaders.find(filename); it != m_instance->m_shaders.end())
				return &it->second;

			return m_instance->tryCreate<sf::Shader>(filename, m_instance->m_shaders);
		}
	}

	return nullptr;
}

template<class T>
T* AssetManager::get(const std::string& filename) noexcept
{
	if (m_instance)
	{
//  Textures
		if constexpr (std::is_same<T, sf::Texture>::value)
		{
			if (auto it = m_instance->m_textures.find(filename); it != m_instance->m_textures.end())
				return &it->second;

			return m_instance->tryLoadFromFile<sf::Texture>(filename, m_instance->m_textures);
		}
//  Fonts
		else if constexpr (std::is_same<T, sf::Font>::value)
		{
			if (auto it = m_instance->m_fonts.find(filename); it != m_instance->m_fonts.end())
				return &it->second;

			return m_instance->tryLoadFromFile<sf::Font>(filename, m_instance->m_fonts);
		}
//  Sounds
		else if constexpr (std::is_same<T, sf::SoundBuffer>::value)
		{
			if (auto it = m_instance->m_sounds.find(filename); it != m_instance->m_sounds.end())
				return &it->second;

			return m_instance->tryLoadFromFile<sf::SoundBuffer>(filename, m_instance->m_sounds);
		}
//  Music
		else if constexpr (std::is_same<T, sf::Music>::value)
		{
			if (auto it = m_instance->m_musics.find(filename); it != m_instance->m_musics.end())
				return &it->second;

			return m_instance->tryLoadFromFile<sf::Music>(filename, m_instance->m_musics);
		}
//  Shaders    
		else if constexpr (std::is_same<T, sf::Shader>::value)
		{
			if (auto it = m_instance->m_shaders.find(filename); it != m_instance->m_shaders.end())
				return &it->second;

			return m_instance->tryLoadFromFile<sf::Shader>(filename, m_instance->m_shaders);
		}
	}

	return nullptr;
}

template<class T>
void AssetManager::remove(const std::string& filename) noexcept
{
	if (m_instance)
	{
//  Textures
		if constexpr (std::is_same<T, sf::Texture>::value)
		{
			if (auto it = m_instance->m_textures.find(filename); it != m_instance->m_textures.end())
				m_instance->m_textures.erase(it);
		}
//  Fonts
		else if constexpr (std::is_same<T, sf::Font>::value)
		{
			if (auto it = m_instance->m_fonts.find(filename); it != m_instance->m_fonts.end())
				m_instance->m_fonts.erase(it);
		}
//  Sounds        
		else if constexpr (std::is_same<T, sf::SoundBuffer>::value)
		{
			if (auto it = m_instance->m_sounds.find(filename); it != m_instance->m_sounds.end())
				m_instance->m_sounds.erase(it);
		}
//  Music
		else if constexpr (std::is_same<T, sf::Music>::value)
		{
			if (auto it = m_instance->m_musics.find(filename); it != m_instance->m_musics.end())
				m_instance->m_musics.erase(it);
		}
//  Shaders
		else if constexpr (std::is_same<T, sf::Shader>::value)
		{
			if (auto it = m_instance->m_shaders.find(filename); it != m_instance->m_shaders.end())
				m_instance->m_shaders.erase(it);
		}
	}
}

template<class T>
T* AssetManager::tryLoadFromFile(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept
{
	auto [iterator, result] = container.try_emplace(filename);

	if (result)// Handling special cases at first
	{
//  sf::Music case               
		if constexpr (std::is_same<T, sf::Music>::value)
		{
			if (const std::string filepath = FileProvider::getPathToFile(filename).generic_string(); !iterator->second.openFromFile(filepath))
			{
				container.erase(iterator);

				return nullptr;
			}
		}
//  sf::Shader case. You have to upload it manually
		else if constexpr (std::is_same<T, sf::Shader>::value)
		{
			if (!sf::Shader::isAvailable())
			{
				container.erase(iterator);

				return nullptr;
			}
		}
//  sf::Image, sf::Texture, sf::Font etc...                     
		else if (const std::string filepath = FileProvider::getPathToFile(filename).generic_string(); !iterator->second.loadFromFile(filepath))
		{
			auto t = filepath;
			container.erase(iterator);

			return nullptr;
		}

		return &iterator->second;
	}

	return nullptr;
}

template<class T>
T* AssetManager::tryCreate(const std::string& filename, std::unordered_map<std::string, T>& container) noexcept
{
	auto [iterator, result] = container.try_emplace(filename);

	if (result)
		return &iterator->second;
	
	return nullptr;
}