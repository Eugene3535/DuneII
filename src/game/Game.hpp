#ifndef GAME_HPP
#define GAME_HPP

#include "scenes/SceneNode.hpp"

class Game final:
	public SceneNode
{
public:
    enum State: std::size_t
    {
        TITLE_SCREEN = 0ULL,
        BATTLE_ZONE,
        MISSION_COMPLETED,
        GAME_OVER
    };

public:
    Game(SceneNode* root) noexcept;
    ~Game();

    bool load() noexcept override;
    void update(float dt) noexcept override;

    sf::Vector2i getCursorPosition() const noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    std::unique_ptr<sf::RenderWindow> m_window;

public:
    State m_state;
    
    SceneNode* m_currentScene;
};




#endif