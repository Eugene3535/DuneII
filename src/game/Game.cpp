#include <thread>

#include "scenes/intro/MainMenu.hpp"
#include "scenes/mission/Mission.hpp"

#include "game/Game.hpp"

Game::Game() noexcept
{
    next_scene = NONE;
    a_scene_needs_to_be_changed = false;
    m_is_blackout_mode = false;
}

Game::~Game()
{
}

void Game::setSpriteSize(sf::Sprite& sprite, std::int32_t width, std::int32_t height) noexcept
{
    const auto& rect = sprite.getTextureRect();

	if (rect.width && rect.height)
	{
		float dx = width / std::abs(rect.width);
		float dy = height / std::abs(rect.height);
		sprite.setScale(dx, dy);
	}
}

void Game::setSpriteSize(sf::Sprite& sprite, const sf::Vector2i& size) noexcept
{
	setSpriteSize(sprite, size.x, size.y);
}

sf::Vector2i Game::getSpriteSize(const sf::Sprite& sprite) noexcept
{
	const auto& rect  = sprite.getTextureRect();
	const auto& scale = sprite.getScale();

	return { static_cast<std::int32_t>(rect.width * scale.x), static_cast<std::int32_t>(rect.height * scale.y) };
}

std::int32_t Game::run() noexcept
{
    visible_area = sf::FloatRect(0.f, 0.f, 800.0f, 600.0f);

    std::uint32_t width  = static_cast<std::uint32_t>(visible_area.width);
    std::uint32_t height = static_cast<std::uint32_t>(visible_area.height);

    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Dune: The Battle For Arrakis");
    window->setFramerateLimit(60U);

    Scene* current_scene = nullptr;
    auto main_menu = scenes.push<MainMenu>(this);
    auto mission = scenes.push<Mission>(this); 

    if(!main_menu->load(std::string()))
        return -1;

    current_scene = main_menu;
    a_scene_needs_to_be_changed = false;

    viewport.reset(visible_area);
    

    while (window->isOpen())
    {
        sf::Event event;

        auto dt = clock.restart();

        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Resized)
            {
                const auto& center = viewport.getCenter();

                float x = center.x - event.size.width * 0.5f;
                float y = center.y - event.size.height * 0.5f;
                visible_area = sf::FloatRect(x, y, event.size.width, event.size.height);
                viewport.reset(visible_area);
            }

            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window->close();

                return 0;
            }

            if(event.type == sf::Event::KeyPressed)
            {
                event.key.code;
            }
        }

        current_scene->update(dt);

        if(a_scene_needs_to_be_changed)
        {
            m_fade_effect.prepare(viewport.getCenter(), window->getSize());
            constexpr auto delay = std::chrono::milliseconds(1000 / 60);
            std::thread t(&ScreenBlackoutEffect::apply, &m_fade_effect);
            
            while(!m_fade_effect.isOver())
            {     
                window->clear();
                window->draw(*current_scene);
                window->draw(m_fade_effect);
                window->display();
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }

            t.join();
            
            switch (next_scene)
            {
            case GameScene::MAIN_MENU:
            {
                current_scene = main_menu;
                const auto& size = viewport.getSize();
                visible_area = { 0, 0, size.x, size.y };
                viewport.reset(visible_area);
                break;
            }

            case GameScene::MISSION:
            {
                mission->load("Atreides-8.tmx");
                current_scene = mission;
                break;
            }
    
            default:
                break;
            }

            a_scene_needs_to_be_changed = false;
            next_scene = GameScene::NONE;
        }

        window->setView(viewport);
        window->clear();
        window->draw(*current_scene);
        window->display();
    }
    
    return 0;
}