#include "Scene_Play.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

#include <iostream>

Scene_Play::Scene_Play(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init(m_levelPath);
}

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
        : Scene(gameEngine),
        m_levelPath(levelPath)
{
    init(m_levelPath);
}

size_t Scene_Play::width() const
{
    return m_game->window().getSize().x;
}

size_t Scene_Play::height() const
{
    return m_game->window().getSize().y;
}

void Scene_Play::init(const std::string& levelPath)
{
    registerAction(sf::Keyboard::P,         "PAUSE");
    registerAction(sf::Keyboard::Escape,    "QUIT");
    registerAction(sf::Keyboard::T,         "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C,         "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G,         "TOGGLE_GRID");

    // TODO: register all other gameplay Actions

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Enchrome"));

    loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    // TODO:
    //
    //
    //
    //

    return Vec2(0, 0);
}

void Scene_Play::loadLevel(const std::string& filename)
{
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    // TODO:  
    //
    //

    // NOTE:
    //

    spawnPlayer();

    //some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT: 
    brick->addComponent<CAnimation>(m_game->assets().getAnimation("Grass"), true);
    brick->addComponent<CBoundingBox>(m_game->assets().getAnimation("Grass")->getSize());
    brick->addComponent<CTransform>(Vec2(400, 480));
    // NOTE:
    // brick->addComponent ...

    // if( brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
        // std::cout << "This could be a good way of identifying if a tile is a brick\n";
    }

    auto block = m_entityManager.addEntity("tile");
    std::cout << "added tile\n";
    block->addComponent<CAnimation>(m_game->assets().getAnimation("Cobblestone"), true);
    block->addComponent<CTransform>(Vec2(200, 200));
    // add a bounding box, this will not show up if we press the 'C' key
    block->addComponent<CBoundingBox>(m_game->assets().getAnimation("Cobblestone")->getSize());

    auto question = m_entityManager.addEntity("tile");
    question->addComponent<CAnimation>(m_game->assets().getAnimation("HalfWater"), true);
    question->addComponent<CBoundingBox>(m_game->assets().getAnimation("HalfWater")->getSize());
    question->addComponent<CTransform>(Vec2(700, 700));

    // NOTE:
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
}

void Scene_Play::spawnPlayer()
{
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    std::cout << "player created: " << &m_player << "\n";
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Running"), true);
    m_player->addComponent<CTransform>(Vec2(224, 352));
    m_player->addComponent<CBoundingBox>(m_game->assets().getAnimation("Running")->getSize());

    // TODO: be sure to add the remaining components to the player 
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
}

void Scene_Play::update()
{
    m_entityManager.update();
    // // TODO: implement pause functionality

    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();
    m_currentFrame++;
}

void Scene_Play::sMovement()
{
    // TODO: implement player movement / jumping based on its CInput component
    // TODO: implement gravity's effect on the player
    // TODO: implement the maximum player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right
}

void Scene_Play::sLifespan()
{
    // TODO: check lifespan of entitites that have them, and destroy them if they go over
}

void Scene_Play::sCollision()
{
    // REMEMBER:: SFML .....
    //
    //
    //
    //

    // TODO Implement Physics::GetOverlap() function, use it inside this function

    // TODO: Implement bullet / tile collisions
    //
    // TODO: Imlement player / tile collision and resolutions
    //
    //
    //
    // TODO: Check to see if the player has falled down a hole (y > height())
    // TODO: Don't let the player walk off the left side of the map
}

void Scene_Play::sDoAction(const Action& action)
{
    if(action.type() == "START")
    {
                 if (action.name() == "TOGGLE_TEXTURE" )    { m_drawTextures = !m_drawTextures; }
            else if (action.name() == "TOGGLE_COLLISION" )  { m_drawCollision = !m_drawCollision; }
            else if (action.name() == "TOOGLE_GRID" )       { m_drawGrid = !m_drawGrid; }
            else if (action.name() == "PAUSE" )             { setPaused(!m_paused); }
            else if (action.name() == "QUIT" )              { onEnd(); }

    }
    else if (action.type() == "END")
    {

    }
}

void Scene_Play::sAnimation()
{
    for ( auto e : m_entityManager.getEntities() )
    {
        e->getComponent<CAnimation>().animation->update();
    }
}

void Scene_Play::onEnd()
{

}

void Scene_Play::sRender()
{
    // color the background darker so you know that the game is paused

    // if ( !m_paused ) { m_game->window().clear(sf::Color(100, 100, 255)); }
    // else             { m_game->window().clear(sf::Color( 50,  50, 150)); }
    m_game->window().clear();

    // set the viewport of the window to be centered on the player if it's far enough right
    auto&       pPos            = m_player->getComponent<CTransform>().pos;
    float       windowCenterX   = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View    view            = m_game->window().getView();
    view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);

    //draw all Entity textures / animations
    if ( m_drawTextures )
    {
        for ( auto e : m_entityManager.getEntities() )
        {
            auto& transform = e->getComponent<CTransform>();

            if(e->hasComponent<CAnimation>())
            {
                auto& animation = e->getComponent<CAnimation>().animation;
                animation->getSprite().setRotation(transform.angle);
                animation->getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation->getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation->getSprite());
            }
        }
    }

    if ( m_drawCollision )
    {
        for( auto e : m_entityManager.getEntities() )
        {
            for( auto e : m_entityManager.getEntities() )
            {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x-1, box.size.y-1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0,0,0,0));
                rect.setOutlineColor(sf::Color(255, 255 ,255, 255));
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);

            }
        }
    }

    // draw the grid for debugging
    if ( m_drawGrid )
    {
        float leftX = m_game->window().getView().getCenter().x - width() / 2;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

        for ( float x = nextGridX; x < rightX; x += m_gridSize.x )
        {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for ( float y = 0; y < height(); y += m_gridSize.y )
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for ( float x = nextGridX; x < rightX; x += m_gridSize.x )
            {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x );
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y );
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }

    m_game->window().display();
}



