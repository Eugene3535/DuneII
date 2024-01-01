#ifndef GAME_HPP
#define GAME_HPP

#include "managers/AssetManager.hpp"
#include "scenes/SceneNode.hpp"

class Game final:
	public SceneNode
{
public:
    Game(SceneNode* root) noexcept;
    ~Game();

    void update(float dt) noexcept override;
    int run() noexcept;

    sf::Vector2i getCursorPosition() const noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Clock m_clock;

private:
    std::unique_ptr<sf::RenderWindow> m_window;

private:
    AssetManager a;
};




#endif