#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <SFML/Graphics/RectangleShape.hpp>

#include "scenes/base/Scene.hpp"

class ConstructionMenu:
	public Scene
{
public:
	ConstructionMenu(struct Game& game) noexcept;
	~ConstructionMenu() noexcept;

	bool load(const std::string& info) noexcept override;
	void update(sf::Time dt)           noexcept override;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::RectangleShape m_background;
};

#endif // !CONSTRUCTION_MENU_HPP
