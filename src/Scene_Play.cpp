#include "Scene_Play.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include <fstream>
#include <iostream>
#include <string>

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
    registerAction(sf::Keyboard::G,         "TOOGLE_GRID");
    registerAction(sf::Keyboard::Space,     "JUMP");
    registerAction(sf::Keyboard::D,         "RIGHT");
    registerAction(sf::Keyboard::A,         "LEFT");

    // TODO: register all other gameplay Actions

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Enchrome"));

    loadLevel(levelPath);
}

// animation was already given to this entity in the first place
Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    Vec2 entitySize = entity->getComponent<CAnimation>().animation->getSize();
    
    gridX = gridX * 64;
    gridY = m_game->window().getSize().y - gridY * 64;

    float entityMidX = gridX + entitySize.x/2;
    float entityMidY = gridY - entitySize.y/2;

    return Vec2(entityMidX, entityMidY);
}

void Scene_Play::loadLevel(const std::string& filename)
{
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    std::cout << filename << "\n";

    std::ifstream file;
    file.open(filename);

    std::string type, animation, gX, gY, cW, cH, sX, sY, sM, G, B;
    
    while ( file >> type )
    {
        if( type == "Tile" )
        {
            file >> animation >> gX >> gY;

            auto brick = m_entityManager.addEntity("tile");
            brick->addComponent<CAnimation>(m_game->assets().getAnimation(animation), true);
            brick->addComponent<CBoundingBox>(m_game->assets().getAnimation(animation)->getSize());
            brick->addComponent<CTransform>(gridToMidPixel(std::stof(gX), std::stof(gY), brick));
        }
        else if( type == "Dec" )
        {
            file >> animation >> gX >> gY;

            auto grass = m_entityManager.addEntity("dec");
            grass->addComponent<CAnimation>(m_game->assets().getAnimation(animation), true);
            grass->addComponent<CTransform>(gridToMidPixel(std::stof(gX), std::stof(gY), grass));
        }
        else if( type == "Player" )
        {
            file >> gX >> gY >> cW >> cH >> sX >> sY >> sM >> G >> B;
            m_playerConfig.X        = std::stof(gX);
            m_playerConfig.Y        = std::stof(gY);
            m_playerConfig.CW       = std::stof(cW);
            m_playerConfig.CH       = std::stof(cH);
            m_playerConfig.SPEED    = std::stof(sX);
            m_playerConfig.JUMP     = std::stof(sY);
            m_playerConfig.MAXSPEED = std::stof(sM);
            m_playerConfig.GRAVITY  = std::stof(G);
            m_playerConfig.WEAPON   = std::stof(B);
        }
    }
    spawnPlayer();

    // if( brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
        // std::cout << "This could be a good way of identifying if a tile is a brick\n";
    }
}

void Scene_Play::spawnPlayer()
{
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Running"), true);
    m_player->addComponent<CTransform>(Vec2(464, 200));
    m_player->addComponent<CState>("standing");
    m_player->addComponent<CGravity>(0.6);
    Vec2 skinnySize(m_game->assets().getAnimation("Running")->getSize().x/2, m_game->assets().getAnimation("Grass")->getSize().y);
    m_player->addComponent<CBoundingBox>(skinnySize);
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
}

void Scene_Play::update()
{
    m_entityManager.update();
    // // TODO: implement pause functionality
    sState();
    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();
    m_currentFrame++;
}
void Scene_Play::sState()
{
    for ( auto e : m_entityManager.getEntities() )
    {
        if ( e->getComponent<CState>().has  && e->getComponent<CState>().timer != 0 )
        {
            e->getComponent<CState>().timer--;
            
            if( e->getComponent<CState>().timer == 0 )
            {
                e->getComponent<CState>().state = "falling";
            }
        }
    }
}


void Scene_Play::sMovement()
{
    Vec2 playerVelocity(0.0f, m_player->getComponent<CTransform>().velocity.y);
    if( m_player->getComponent<CInput>().up && m_player->getComponent<CState>().state == "jumping" )
    {
        playerVelocity.y = -10;
    }
    if ( m_player->getComponent<CInput>().left )
    {
        playerVelocity.x = -7;
    }
    if ( m_player->getComponent<CInput>().right )
    {
        playerVelocity.x = 7;
    }

    m_player->getComponent<CTransform>().velocity = playerVelocity;

    for ( auto e : m_entityManager.getEntities() )
    {

        e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;

        if (e->getComponent<CGravity>().has )
        {
            e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
            
            //Max speed
            if ( e->getComponent<CTransform>().velocity.y >= 10 )
            {
                e->getComponent<CTransform>().velocity.y = 10;
            }
            if ( e->getComponent<CTransform>().velocity.x >= 10 )
            {
                e->getComponent<CTransform>().velocity.x = 10;
            }
        }
        e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
    }
}

void Scene_Play::sLifespan()
{
    // TODO: check lifespan of entitites that have them, and destroy them if they go over
}

void Scene_Play::sCollision()
{
    for ( auto e : m_entityManager.getEntities() )
    {
        if ( e->tag() != m_player->tag() && e->getComponent<CBoundingBox>().has )
        {
            Vec2 overlap = m_physics.GetOverlap(m_player, e);
            if( overlap >= 0 )
            {
                Vec2 prevOverlap = m_physics.GetPreviousOverlap(m_player, e);
                if (prevOverlap.x > 0)
                {
                    if (m_player->getComponent<CTransform>().pos.y < e->getComponent<CTransform>().pos.y)
                    {
                        m_player->getComponent<CTransform>().pos.y -= overlap.y;
                        if( m_player->getComponent<CState>().state != "running" )
                        {
                            m_player->getComponent<CState>().state = "standing";
                            m_player->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Standing");
                            m_player->getComponent<CState>().timer = 0;
                        }
                        else
                        {
                            m_player->getComponent<CState>().state = "running";
                            m_player->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Running");
                            m_player->getComponent<CState>().timer = 0;
                        }
                    }
                    else
                    {
                        m_player->getComponent<CTransform>().pos.y += overlap.y;
                    }
                }
                else if (prevOverlap.y > 0)
                {
                    if (m_player->getComponent<CTransform>().pos.x < e->getComponent<CTransform>().pos.x )
                    {
                        m_player->getComponent<CTransform>().pos.x -= overlap.x;
                    }
                    else 
                    {
                        m_player->getComponent<CTransform>().pos.x += overlap.x;
                    }
                }
            }
        }
    }
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
        if (action.name() == "TOGGLE_TEXTURE" )   
        { 
            m_drawTextures = !m_drawTextures; 
        }
        else if (action.name() == "TOGGLE_COLLISION" ) 
        { 
            m_drawCollision = !m_drawCollision; 
        }
        else if (action.name() == "TOOGLE_GRID" )      
        { 
            m_drawGrid = !m_drawGrid; 
        }
        else if (action.name() == "JUMP" )             
        { 
            if( m_player->getComponent<CState>().state != "falling" )
            {
                if ( m_player->getComponent<CState>().state != "jumping" )
                {
                    m_player->getComponent<CState>().timer = 15;
                    m_player->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Jumping");
                }
                m_player->getComponent<CState>().state = "jumping";
                m_player->getComponent<CInput>().up = true;
            }
        }
        else if (action.name() == "RIGHT" )             
        { 
            if( m_player->getComponent<CState>().state != "jumping" && m_player->getComponent<CState>().state != "falling")
            {
                m_player->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Running");
                m_player->getComponent<CState>().state = "running";
                m_player->getComponent<CState>().timer = 0;
            }
            m_player->getComponent<CInput>().right = true; 
        }
        else if (action.name() == "LEFT" )             
        { 
            if( m_player->getComponent<CState>().state != "jumping" && m_player->getComponent<CState>().state != "falling")
            {
                m_player->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Running");
                m_player->getComponent<CState>().state = "running";
                m_player->getComponent<CState>().timer = 0;
            }
            m_player->getComponent<CInput>().left = true; 
        }
        else if (action.name() == "PAUSE" )            
        { 
            setPaused(!m_paused); 
        }
        else if (action.name() == "QUIT" )             
        { 
            onEnd(); 
        }

    }
    else if (action.type() == "END")
    {
        if (action.name() == "JUMP" )             
        { 
            m_player->getComponent<CInput>().up = false; 
        }
        else if (action.name() == "RIGHT" )             
        { 
            if( m_player->getComponent<CState>().state != "jumping" && m_player->getComponent<CState>().state != "falling")
            {
                m_player->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Standing");
                m_player->getComponent<CState>().state = "standing";
                m_player->getComponent<CState>().timer = 0;
            }
            m_player->getComponent<CInput>().right = false; 
        }
        else if (action.name() == "LEFT" )             
        { 
            if( m_player->getComponent<CState>().state != "jumping" && m_player->getComponent<CState>().state != "falling")
            {
                m_player->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Standing");
                m_player->getComponent<CState>().state = "standing";
                m_player->getComponent<CState>().timer = 0;
            }
            m_player->getComponent<CInput>().left = false; 
        }
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




