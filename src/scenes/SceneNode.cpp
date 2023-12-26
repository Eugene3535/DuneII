#include <algorithm>

#include "game/Game.hpp"
#include "scenes/SceneNode.hpp"

SceneNode::SceneNode(SceneNode* root) noexcept:
    m_rootScene(root),
    m_nextScene(nullptr),
    m_currentScene(nullptr),
    m_isDone(false)
{
}

SceneNode::~SceneNode()
{
}

sf::Vector2i SceneNode::getCursorPosition() const noexcept
{
    if(m_rootScene)
        return m_rootScene->getCursorPosition();

    return sf::Vector2i(0, 0);
}

SceneNode::State SceneNode::state() const noexcept
{
    return m_state;
}

void SceneNode::enable() noexcept
{
    m_isDone = false;
}

void SceneNode::disable() noexcept
{
    m_isDone = true;
}

bool SceneNode::isDone() const noexcept
{
    return m_isDone;
}

void SceneNode::removeScene(const SceneNode* node) noexcept
{
    for(std::size_t i = 0; i < m_scenes.size(); ++i)
    {
        if(m_scenes[i].get() == node)
        {
            if(m_scenes.size() > 1)
                m_scenes[i] = std::move(m_scenes.back());

            m_scenes.pop_back();

            return;
        }
    }
}