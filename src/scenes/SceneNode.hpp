#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <string>
#include <vector>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class SceneNode:
    public sf::Drawable,
    private sf::NonCopyable
{
public:
    enum State: std::size_t
    {
        MAIN_MENU = 0ULL,
        BATTLE,
        MISSION_COMPLETED,
        GAME_OVER
    };

public:
    SceneNode(SceneNode* root) noexcept;
    virtual ~SceneNode();

    virtual bool load()           noexcept = 0;
    virtual void update(float dt) noexcept = 0;
    virtual sf::Vector2i getCursorPosition() const noexcept;

public:
    State state() const noexcept;

    void enable()  noexcept;
    void disable() noexcept;
    bool isDone() const noexcept;

protected:   
    template<class T>
    T* pushScene() noexcept;

    template<class T>
    bool setScene() noexcept;
    
    void removeScene(const SceneNode* node) noexcept;

protected:   
    SceneNode* m_rootScene;
    SceneNode* m_nextScene;
    SceneNode* m_currentScene;
    std::vector<std::unique_ptr<SceneNode>> m_scenes;

protected:
    State m_state;

protected:
    bool m_isDone;
};

#include "scenes/SceneNode.inl"

#endif // !SCENE_NODE_HPP