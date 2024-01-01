#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include "scenes/SceneNode.hpp"

class MainMenu:
    public SceneNode
{
public:
    MainMenu(SceneNode* root) noexcept;
    ~MainMenu();

    void update(float dt) noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Sprite m_planet;
    sf::Text m_menuItems[3];
};

#endif // !TITLE_SCREEN_HPP