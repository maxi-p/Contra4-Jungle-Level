#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"

class Scene_Menu : public Scene
{

protected:
    std::vector<std::string>    m_menuStrings;
    sf::Text                    m_menuText;
    std::vector<std::string>    m_levelPaths;
    size_t                      m_selectedMenuIndex = 0;
    std::string                 m_title;

    void init();
    void update();
    void onEnd();

public:

    Scene_Menu(GameEngine * gameEngine = nullptr);
    void sRender();
    void sDoAction(const Action& action);
};
