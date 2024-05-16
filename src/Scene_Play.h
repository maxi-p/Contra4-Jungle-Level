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
        float X, Y, CW, CH, SPEED, MAXSPEED, JUMP, GRAVITY, SCALE;
        std::string WEAPON;
    };

public:
    std::shared_ptr<Entity>     m_player;
    std::string                 m_levelPath;
    PlayerConfig                m_playerConfig;
    bool                        m_drawTextures      = true;
    bool                        m_drawCollision     = false;
    bool                        m_drawGrid          = false;
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
    void setPlayerAnimation(std::string name);
    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
    void spawnPlayer();
    void spawnBullet(std::shared_ptr<Entity> entity);
    void sState();
    void sMovement();
    void sLifespan();
    void sCollision();
    void sAnimation();
    void onEnd();
    void maxSpeed(std::shared_ptr<Entity> entity);
    void sCollisionHelper(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> e);
    size_t width() const;
    size_t height() const;
};
