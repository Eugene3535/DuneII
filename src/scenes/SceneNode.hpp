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
    enum State: unsigned
    {
        MAIN_MENU = 0U,
        BATTLE,
        MISSION_COMPLETED,
        MISSION_FAILED,
        GAME_OVER
    };

    enum Dynasty: unsigned
    {
        ATREIDES = 1U,
        ORDOS,
        HARKONNEN
    };

public:
    SceneNode(SceneNode* root) noexcept;
    virtual ~SceneNode();

    virtual void update(float dt) noexcept = 0;
    virtual sf::Vector2i getCursorPosition() const noexcept;

public:
    State state() const noexcept;

    void enable()  noexcept;
    void disable() noexcept;
    bool isLoaded() const noexcept;
    bool isDone()   const noexcept;

protected:   
    template<class T>
    T* pushScene() noexcept;

    template<class T>
    bool setScene() noexcept;
    
    void removeScene(const SceneNode* node) noexcept;
    void switchScene() noexcept;

protected:
    std::string getMissionFileName() const noexcept;

protected: // utils
    void         setSpriteSize(sf::Sprite& sprite, int width, int height);
    void         setSpriteSize(sf::Sprite& sprite, const sf::Vector2i& size);
    sf::Vector2i getSpriteSize(const sf::Sprite& sprite);

protected:
    static sf::View      m_viewport;
    static sf::FloatRect m_visibleArea;

protected:   
    SceneNode* m_rootScene;
    SceneNode* m_nextScene;
    SceneNode* m_currentScene;
    std::vector<std::unique_ptr<SceneNode>> m_scenes;

protected:
    static State   m_state;
    static Dynasty m_dynasty;
    static int     m_missionNum;

protected:

    bool m_isLoaded;
    bool m_isDone;
};

#include "scenes/SceneNode.inl"

#endif // !SCENE_NODE_HPP