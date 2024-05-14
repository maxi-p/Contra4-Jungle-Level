#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include "Physics.h"
#include "EntityManager.h"

class Scene_Play : public Scene
{
    struct PlayerConfig
    {
        float X, Y, CX, CV, SPEEN, MAXSPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

public:
    std::shared_ptr<Entity>     m_player;
    std::string                 m_levelPath;
    PlayerConfig                m_playerConfig;
    bool                        m_drawTextures      = true;
    bool                        m_drawCollision     = true;
    bool                        m_drawGrid          = true;
    const Vec2                  m_gridSize          = { 64, 64 };
    sf::Text                    m_gridText;
    Physics                     m_physics;

    Scene_Play(GameEngine* gameEngine);
    Scene_Play(GameEngine* gameEngine, const std::string& levelPath);

    void init(const std::string& levelPath);
    void loadLevel(const std::string& filename);
    void update();
    void sDoAction(const Action& action);
    void sRender();
    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
    void spawnPlayer();
    void spawnBullet(std::shared_ptr<Entity> entity);
    void sState();
    void sMovement();
    void sLifespan();
    void sCollision();
    void sAnimation();
    void onEnd();
    size_t width() const;
    size_t height() const;
};
