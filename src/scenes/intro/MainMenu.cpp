#include <cstring>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "common/GraphicsUtils.hpp"
#include "game/Game.hpp"
#include "scenes/intro/MainMenu.hpp"

MainMenu::MainMenu(Game& game) noexcept:
    Scene(game)
{

}

MainMenu::~MainMenu()
{
}

bool MainMenu::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;

    if(auto assets = Assets; assets != nullptr)
    {
        auto font = assets->getResource<sf::Font>("AvanteNrBook.ttf");
        auto texture = assets->getResource<sf::Texture>("TitleScreen.png");

        if(!(font && texture))
            return false;

        AnimationData animData;
        animData.name = "TitleScreen";
        animData.layout = AnimationData::LINEAR;
        animData.texture = texture;
        animData.startFrame = { 0, 0, 304, 120 };
        animData.duration = 50;
        animData.isLooped = false;
        animData.delay = sf::seconds(0.1f);

        if(auto anim = m_game.animationManager.createAnimation(animData); anim != nullptr)
            memcpy(&m_titleScreen, anim, sizeof(Animation));

        sf::Vector2i screenSize = static_cast<sf::Vector2i>(m_game.window.getSize());
        GraphicsUtils::setSpriteSize(m_titleScreen, screenSize);

        m_startGame.setFont(*font);
        m_settings.setFont(*font);
        m_tutorial.setFont(*font);

        const std::string textStart("Начать новую игру");
        const std::string textSettings("Настройки");
        const std::string textTutorial("Обучение");

        m_startGame.setString(sf::String::fromUtf8(textStart.begin(), textStart.end()));
        m_settings.setString(sf::String::fromUtf8(textSettings.begin(), textSettings.end()));
        m_tutorial.setString(sf::String::fromUtf8(textTutorial.begin(), textTutorial.end()));

        m_startGame.setStyle(sf::Text::Bold);
        m_settings.setStyle(sf::Text::Bold);
        m_tutorial.setStyle(sf::Text::Bold);

        m_startGame.setPosition(550, 400);
        m_settings.setPosition(550, 450);
        m_tutorial.setPosition(550, 500);

        m_startGame.setScale(0.5f, 0.5f);
        m_settings.setScale(0.5f, 0.5f);
        m_tutorial.setScale(0.5f, 0.5f);

        m_isLoaded = true;

        return m_isLoaded;
    }
        
    return false;
}

void MainMenu::update(sf::Time dt) noexcept
{
    if(m_game.sceneNeedToBeChanged)
        return;

    if( ! m_titleScreen.isOver )
    {
        m_titleScreen.timer += dt;

        if(m_titleScreen.timer > m_titleScreen.delay)
        {
            m_titleScreen.currentFrame++;
            m_titleScreen.timer = sf::Time::Zero;

            if(m_titleScreen.currentFrame == m_titleScreen.duration)
            {
                m_titleScreen.isOver = true;
            }
            if( ! m_titleScreen.isOver )
                m_titleScreen.setTextureRect(m_titleScreen.frames[m_titleScreen.currentFrame]);
        }
    }

    sf::Vector2f mouse_pos { sf::Mouse::getPosition(m_game.window) };

    auto is_button_pressed = [](sf::Text& text, const sf::Vector2f& mouse_pos) -> bool
    {
        if(text.getGlobalBounds().contains(mouse_pos))
        {
            text.setFillColor(sf::Color::Red);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))       
                return true;   
        }
        else   
            text.setFillColor(sf::Color::White);
        
        return false;
    };

    if(is_button_pressed(m_startGame, mouse_pos))
    {
        m_game.sceneNeedToBeChanged = true;
        m_game.next_scene = Game::GameScene::MISSION;
        m_game.window.setMouseCursorVisible(false);

        return;
    }

    if(is_button_pressed(m_settings, mouse_pos))
    {
        return;
    }

    if(is_button_pressed(m_tutorial, mouse_pos))
    {
        return;
    }
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_titleScreen, states);
    target.draw(m_startGame, states);
    target.draw(m_settings, states);
    target.draw(m_tutorial, states);
}