#include <unordered_map>

#include "utils/Defines.hpp"
#include "utils/Resources.hpp"

sf::Texture* GetTexture(const std::string& filename, bool unload) noexcept
{
	static std::unordered_map<std::string, sf::Texture> textures;

	if (unload)
	{
		if(filename.empty())
			textures.clear();
		else
			if (auto it = textures.find(filename); it != textures.end())
				textures.erase(it);

		return nullptr;
	}

	if (auto it = textures.find(filename); it != textures.end())
		return &it->second;

	auto [iterator, result] = textures.try_emplace(filename);

	if (result)
	{
		if(!iterator->second.loadFromFile("res/textures/" + filename + ".png"))
			if(!iterator->second.loadFromFile("res/textures/" + filename + ".jpg"))
			{
				CHECK_EXPR(false);

				sf::Image image;
				image.create(50, 50, sf::Color::White);

				if(!iterator->second.loadFromImage(image))
				{
					textures.erase(iterator);

					return nullptr;
				}
			}

		return &iterator->second;
	}

	return nullptr;
}