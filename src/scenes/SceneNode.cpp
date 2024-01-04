#include <algorithm>

#include "game/Game.hpp"
#include "scenes/SceneNode.hpp"

sf::View SceneNode::m_viewport;
sf::FloatRect SceneNode::m_visibleArea;

SceneNode::State SceneNode::m_state;
SceneNode::Dynasty SceneNode::m_dynasty;
int SceneNode::m_missionNum;

SceneNode::SceneNode(SceneNode* root) noexcept:
    m_rootScene(root),
    m_nextScene(nullptr),
    m_currentScene(nullptr),
    m_isLoaded(false),
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

bool SceneNode::isLoaded() const noexcept
{
    return m_isLoaded;
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

void SceneNode::switchScene() noexcept
{
    if(m_nextScene)
    {
        m_currentScene = m_nextScene;
        m_currentScene->enable();
        m_currentScene->update(0);
        m_nextScene = nullptr;
    }
}

std::string SceneNode::getMissionFileName() const noexcept
{
    if(m_missionNum < 1U || m_missionNum > 8U)
        return std::string();

    switch (m_dynasty)
    {
    case SceneNode::Dynasty::ATREIDES:
        return "Atreides" + std::to_string(m_missionNum) + ".tmx";

    case SceneNode::Dynasty::ORDOS:
        return "Ordos" + std::to_string(m_missionNum) + ".tmx";

    case SceneNode::Dynasty::HARKONNEN:
        return "Harkonnen" + std::to_string(m_missionNum) + ".tmx";
    }

    return std::string();
}

void SceneNode::setSpriteSize(sf::Sprite& sprite, int width, int height)
{
    const auto& rect = sprite.getTextureRect();

	if (rect.width && rect.height)
	{
		float dx = width / std::abs(rect.width);
		float dy = height / std::abs(rect.height);
		sprite.setScale(dx, dy);
	}
}

void SceneNode::setSpriteSize(sf::Sprite& sprite, const sf::Vector2i& size)
{
	setSpriteSize(sprite, size.x, size.y);
}

sf::Vector2i SceneNode::getSpriteSize(const sf::Sprite& sprite)
{
	const auto& rect  = sprite.getTextureRect();
	const auto& scale = sprite.getScale();

	return { static_cast<int>(rect.width * scale.x), static_cast<int>(rect.height * scale.y) };
}