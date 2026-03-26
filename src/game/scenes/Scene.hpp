#ifndef SCENE_HPP
#define SCENE_HPP

#include <cstdint>
#include <string_view>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>


class Scene:
    public sf::Drawable
{
public:
    enum Type : uint32_t
    {
        NONE = 0,
        MAIN_MENU,
        PICK_HOUSE,
        MISSION,
        HUD
    };

	Scene(class DuneII* game, const Type type) noexcept;
	virtual ~Scene();

    virtual bool load(std::string_view data) noexcept;
    virtual void update(sf::Time dt)         noexcept;
    virtual void resize(sf::Vector2u size)   noexcept;

    bool isLoaded() const noexcept;
    Type getType()  const noexcept;

protected:
    void setSpriteSizeInPixels(sf::Sprite& sprite, sf::Vector2f size) noexcept;

    class DuneII* m_game;
    bool          m_isLoaded;

    sf::View m_view;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    const Type m_type;
};

#endif // !SCENE_HPP
