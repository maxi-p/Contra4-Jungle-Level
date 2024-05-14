#pragma once

#include "Action.h"
#include "EntityManager.h"

#include <memory>

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{

public:

    GameEngine*     m_game              = nullptr;
    EntityManager   m_entityManager;
    size_t          m_currentFrame      = 0;
    ActionMap       m_actionMap;
    bool            m_paused            = false;
    bool            m_hasEnded          = false;

    Scene(GameEngine* gameEngine);

    virtual void onEnd() = 0;
    void setPaused(bool paused);
    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;
    
    void simulate(const size_t frames);
    void registerAction(int inputKey, const std::string& actionName);

    virtual size_t width() const = 0;
    virtual size_t height() const = 0;
    size_t currentFrame() const;

    bool hasEnded() const;
    const ActionMap& getActionMap() const;
    void drawLine(const Vec2& p1, const Vec2& p2);
};
