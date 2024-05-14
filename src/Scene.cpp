#include "Scene.h"

#include "EntityManager.h"
#include <memory>

Scene::Scene(GameEngine* gameEngine)
    : m_game(gameEngine)
{

}

void Scene::setPaused(bool paused)
{

}

const ActionMap& Scene::getActionMap() const
{
    return m_actionMap;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{

}

void Scene::simulate(const size_t frames)
{

}

void Scene::registerAction(int inputKey, const std::string& actionName)
{

}

size_t Scene::currentFrame() const
{
    return m_currentFrame;
}

