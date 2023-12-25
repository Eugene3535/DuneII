#include <algorithm>

#include "game/Game.hpp"
#include "scenes/SceneNode.hpp"

SceneNode::SceneNode(SceneNode* root) noexcept:
    m_root(root),
    m_isDone(false)
{
    m_title = "Base scene";
}

SceneNode::~SceneNode()
{
}

const std::string& SceneNode::title() const noexcept
{
    return m_title;
}

sf::Vector2i SceneNode::getCursorPosition() const noexcept
{
    if(m_root)
        return m_root->getCursorPosition();

    return sf::Vector2i(0, 0);
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

bool SceneNode::isDone() const noexcept
{
    return m_isDone;
}