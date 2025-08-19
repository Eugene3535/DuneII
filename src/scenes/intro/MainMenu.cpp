#include <cstring>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "common/GraphicsUtils.hpp"
#include "assets/AssetManager.hpp"
#include "animation/AnimationData.hpp"
#include "game/DuneII.hpp"
#include "scenes/intro/MainMenu.hpp"


MainMenu::MainMenu(DuneII* game) noexcept:
    Scene(game)
{

}


MainMenu::~MainMenu() = default;


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
        animData.startFrame = { { 0, 0 }, { 304, 120 } };
        animData.duration = 50;
        animData.isLooped = false;
        animData.delay = sf::seconds(0.1f);

        if(auto anim = m_game->animationManager.createAnimation(animData); anim != nullptr)
        {
            m_titleScreen = std::make_unique<sf::Sprite>(*texture, anim->frames[0]);
            memcpy(static_cast<void*>(&m_animation), anim, sizeof(Animation));
        }
        else return false;                 

        sf::Vector2i screenSize = static_cast<sf::Vector2i>(m_game->window.getSize());
        GraphicsUtils::setSpriteSize(*m_titleScreen, screenSize);

        m_startGame = std::make_unique<sf::Text>(*font);
        m_settings  = std::make_unique<sf::Text>(*font);
        m_tutorial  = std::make_unique<sf::Text>(*font);

        m_startGame->setFont(*font);
        m_settings->setFont(*font);
        m_tutorial->setFont(*font);

        const std::string_view textStart("Начать новую игру");
        const std::string_view textSettings("Настройки");
        const std::string_view textTutorial("Обучение");

        m_startGame->setString(sf::String::fromUtf8(textStart.begin(), textStart.end()));
        m_settings->setString(sf::String::fromUtf8(textSettings.begin(), textSettings.end()));
        m_tutorial->setString(sf::String::fromUtf8(textTutorial.begin(), textTutorial.end()));

        m_startGame->setStyle(sf::Text::Bold);
        m_settings->setStyle(sf::Text::Bold);
        m_tutorial->setStyle(sf::Text::Bold);

        constexpr uint32_t charSize = 60;
        m_startGame->setCharacterSize(charSize);
        m_settings->setCharacterSize(charSize);
        m_tutorial->setCharacterSize(charSize);

        m_startGame->setPosition({ 920, 600 });
        m_settings->setPosition({ 920, 650 });
        m_tutorial->setPosition({ 920, 700 });

        m_startGame->setScale({ 0.5f, 0.5f });
        m_settings->setScale({ 0.5f, 0.5f });
        m_tutorial->setScale({ 0.5f, 0.5f });

        m_isLoaded = true;

        return m_isLoaded;
    }
        
    return false;
}


void MainMenu::update(const sf::Time dt) noexcept
{
    if( ! m_animation.isOver )
    {
        m_animation.timer += dt;

        if(m_animation.timer > m_animation.delay)
        {
            m_animation.currentFrame++;
            m_animation.timer = sf::Time::Zero;

            if(m_animation.currentFrame == m_animation.frames.size())
            {
                m_animation.isOver = true;
            }
            if( ! m_animation.isOver )
                m_titleScreen->setTextureRect(m_animation.frames[m_animation.currentFrame]);
        }
    }

    sf::Vector2f mouse_pos { sf::Mouse::getPosition(m_game->window) };

    auto is_button_pressed = [](sf::Text* text, const sf::Vector2f& mouse_pos) -> bool
    {
        if(text->getGlobalBounds().contains(mouse_pos))
        {
            text->setFillColor(sf::Color::Red);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))       
                return true;   
        }
        else   
            text->setFillColor(sf::Color::White);
        
        return false;
    };

    if(is_button_pressed(m_startGame.get(), mouse_pos))
    {
        m_game->notifyChangeScene(this, DuneII::GameScene::MISSION);
        m_game->window.setMouseCursorVisible(false);
    }
}


sf::Vector2i MainMenu::resize(const sf::Vector2u& size) noexcept
{
    sf::Vector2i screenSize = static_cast<sf::Vector2i>(size);
    GraphicsUtils::setSpriteSize(*m_titleScreen, screenSize);

    return { screenSize / 2 };
}


void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*m_titleScreen, states);
    target.draw(*m_startGame, states);
    target.draw(*m_settings, states);
    target.draw(*m_tutorial, states);
}