#include "utils/FileProvider.hpp"
#include "game/Game.hpp"
#include "scenes/intro/MainMenu.hpp"
#include "scenes/battle/BattleField.hpp"

BattleField::BattleField(SceneNode* root) noexcept:
    SceneNode(root)
{
    m_state = SceneNode::State::BATTLE;
    m_dynasty = SceneNode::Dynasty::ATREIDES;
    m_missionNum = 8;
    std::string mission = getMissionFileName();

    if(m_tilemap.loadFromFile(FileProvider::getPathToFile(mission)))
        m_isLoaded = true;
}

BattleField::~BattleField()
{
}

void BattleField::update(float dt) noexcept
{
    if(m_isDone)
        return;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_viewport.move(-10, 0);
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_viewport.move(10, 0);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_viewport.move(0, -10);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_viewport.move(0, 10);


    if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
    {
        if(m_isDone = setScene<MainMenu>())
        {
            m_visibleArea.left = 0.0f;
            m_visibleArea.top = 0.0f;
            m_viewport.reset(m_visibleArea);
            
            removeScene(this);
            
            return;
        }
    }
}

void BattleField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isDone)
        return;

    target.setView(m_viewport);     

    for (auto& layer : m_tilemap.m_layers)
    {
        target.draw(layer.vertexBuffer, layer.texture);
    }
}