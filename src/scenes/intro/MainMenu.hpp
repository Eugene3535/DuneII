#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include "scenes/SceneNode.hpp"

class MainMenu:
    public SceneNode
{
public:
    MainMenu(SceneNode* root) noexcept;
    ~MainMenu();

    bool load()           noexcept override;
    void update(float dt) noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Sprite m_buttons[3];

    sf::Text m_menuItems[3];
};

#endif // !TITLE_SCREEN_HPP