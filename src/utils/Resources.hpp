#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>

#include <SFML/Graphics.hpp>

sf::Texture* GetTexture(const std::string& filename, bool unload = false) noexcept;

#endif // !RESOURCES_HPP