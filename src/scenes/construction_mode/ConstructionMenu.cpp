#include "assets/AssetManager.hpp"
#include "game/DuneII.hpp"
#include "scenes/construction_mode/ConstructionMenu.hpp"

ConstructionMenu::ConstructionMenu(DuneII* game) noexcept:
	Scene(game)
{

}

ConstructionMenu::~ConstructionMenu() noexcept = default;

bool ConstructionMenu::load(const std::string& info) noexcept
{
	if (m_isLoaded)
		return true;

	if (auto texBg = Assets->getResource<sf::Texture>("game_menu.jpg"); texBg != nullptr)
	{
		auto video_mode = sf::VideoMode::getDesktopMode();
		float menu_width = static_cast<float>(video_mode.size.x) - 20;
		float menu_height = static_cast<float>(video_mode.size.y) - 20;

		sf::Color outline_color = sf::Color(88, 89, 120);

		m_background.setTexture(texBg);
		m_background.setSize({ menu_width, menu_height });
		m_background.setOutlineThickness(10);
		m_background.setOutlineColor(outline_color);

		m_isLoaded = true;
	}

	return m_isLoaded;
}

void ConstructionMenu::update(sf::Time dt) noexcept
{

}

void ConstructionMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background, states);
}
