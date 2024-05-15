#include "GameEngine.h"
#include "Assets.h"
#include "Scene.h"
#include "Scene_Play.h"
// #include "Scene_Menu.h"

#include <iostream>

void GameEngine::init(const std::string& path)
{
    m_assets.loadFromFile(path);

    m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
    m_window.setFramerateLimit(60);

    // changeScene("MENU", std::make_shared<Scene_Menu>(this));
    auto newScene = std::make_shared<Scene_Play>(this, "./config1.txt");
    changeScene( "PLAY", newScene );
}

void GameEngine::update() 
{
    m_sceneMap[m_currentScene]->update();
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            quit();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Q)
            {
                quit();
            }
            if(event.key.code == sf::Keyboard::X)
            {
                std::cout << "screeenshot saved to " << " test.png" << std::endl;
                sf::Texture texture;
                texture.create(m_window.getSize().x, m_window.getSize().y);
                texture.update(m_window);
                if (texture.copyToImage().saveToFile("text.png"))
                {
                    std::cout << "screenshot saved to " << "test.png" << std::endl;
                }
            }
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // value for this key does NOT exit
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
            {
                continue;
            }
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
            currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }
    }
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap[m_currentScene];
}

GameEngine::GameEngine(const std::string& path)
{
    init(path);
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    m_currentScene = sceneName;
    m_sceneMap[sceneName] = scene;
}

void GameEngine::quit() 
{
    m_running = false;
}

void GameEngine::run()
{
    while (isRunning() && m_window.isOpen())
    {
        sUserInput();
        update();
    }
}

sf::RenderWindow& GameEngine::window()
{
    return m_window;
}

Assets& GameEngine::assets() 
{
    return m_assets;
}

bool GameEngine::isRunning()
{
    return m_running && m_window.isOpen();
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
    sf::Vertex line[2];
    line[0].position = sf::Vector2f(p1.x, p1.y);
    line[0].color  = sf::Color::Red;
    line[1].position = sf::Vector2f(p2.x, p2.y);
    line[1].color = sf::Color::Red;

    m_game->window().draw(line, 2, sf::Lines);
}





