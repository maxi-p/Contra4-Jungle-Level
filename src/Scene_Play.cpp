#include "Scene_Play.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include <unistd.h>
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
    // registerAction(sf::Keyboard::P,         "PAUSE");
    registerAction(7+sf::Keyboard::Escape,    "QUIT");
    registerAction(7+sf::Keyboard::T,         "TOGGLE_TEXTURE");
    registerAction(7+sf::Keyboard::C,         "TOGGLE_COLLISION");
    registerAction(7+sf::Keyboard::G,         "TOOGLE_GRID");
    // registerAction(7+sf::Keyboard::W,         "JUMP");
    registerAction(7+sf::Keyboard::Space,     "JUMP");
    registerAction(7+sf::Keyboard::D,         "RIGHT");
    registerAction(7+sf::Keyboard::A,         "LEFT");
    registerAction(7+sf::Keyboard::LShift,    "SHIFT");
    registerAction(sf::Mouse::Left,           "SHOOT");

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Enchrome"));

    loadLevel(levelPath);
}

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
    m_entityManager = EntityManager();
    std::ifstream file;
    file.open(filename);

    std::string type, animation, gX, gY, sX, sY, sM, G, Sc, bX, bY, B;
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
            file >> gX >> gY >> sX >> sY >> sM >> G >> Sc >> bX >> bY >> B;
            m_playerConfig.X        = std::stof(gX);
            m_playerConfig.Y        = std::stof(gY);
            m_playerConfig.SPEED    = std::stof(sX);
            m_playerConfig.JUMP     = std::stof(sY);
            m_playerConfig.MAXSPEED = std::stof(sM);
            m_playerConfig.GRAVITY  = std::stof(G);
            m_playerConfig.SCALE    = std::stof(Sc);
            m_playerConfig.CW       = std::stof(bX);
            m_playerConfig.CH       = std::stof(bY);
            m_playerConfig.WEAPON   = B;
        }
    }
    spawnPlayer();

    // auto e = m_entityManager.addEntity("enemy");
    // e->addComponent<CAnimation>(m_game->assets().getAnimation("Enemy"), true);
    // e->addComponent<CTransform>(Vec2(864, 200));
    // e->getComponent<CTransform>().scale = Vec2(2.0f, 2.0f);
    // e->addComponent<CGravity>(0.6);
    // e->addComponent<CState>("running");
    // e->addComponent<CBoundingBox>(m_game->assets().getAnimation("EnemyJumping")->getSize());

    // e = m_entityManager.addEntity("enemy");
    // e->addComponent<CAnimation>(m_game->assets().getAnimation("BotHigh"), true);
    // e->addComponent<CTransform>(Vec2(764, 200));
    // e->getComponent<CTransform>().scale = Vec2(3.0f, 3.0f);
    // e->addComponent<CGravity>(0.6);
    // e->addComponent<CState>("running");
    // e->addComponent<CBoundingBox>(m_game->assets().getAnimation("BotHigh")->getSize());

    // e = m_entityManager.addEntity("enemy");
    // e->addComponent<CAnimation>(m_game->assets().getAnimation("ShootingEnemyLying"), true);
    // e->addComponent<CTransform>(Vec2(1000, 200));
    // e->getComponent<CTransform>().scale = Vec2(2.0f, 2.0f);
    // e->addComponent<CGravity>(0.6);
    // e->addComponent<CState>("aiming");
    // e->addComponent<CBoundingBox>(m_game->assets().getAnimation("ShootingEnemyMid")->getSize());

    // if( brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
        // std::cout << "This could be a good way of identifying if a tile is a brick\n";
    }
}

void Scene_Play::spawnPlayer()
{
    
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("PlayerFalling"), true);

    Vec2    pos         = gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player);
    Vec2    scale       = Vec2(m_playerConfig.SCALE, m_playerConfig.SCALE);
    Vec2    boundBox    = Vec2(m_playerConfig.CW, m_playerConfig.CH);
    Vec2    velocity    = Vec2(0.0f, 0.0f);
    float   angle       = 0;

    m_player->addComponent<CTransform>(pos, velocity, scale, angle);
    m_player->addComponent<CState>("AIR", "CANNOT_JUMP");
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
    m_player->addComponent<CBoundingBox>(boundBox*m_playerConfig.SCALE);
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // auto bullet = m_entityManager.addEntity("bullet");
    // bullet->addComponent<CAnimation>(m_game->assets().getAnimation("Bullet"), true);
    // bullet->addComponent<CLifeSpan>(10, currentFrame);
    // Vec2 position = m_player->getComponent<CTransform>().pos;
    // bullet->addComponent<CTransform>(position);
}

void Scene_Play::update()
{
    m_entityManager.update();
    sState();
    sLifespan();
    sMovement();
    sAnimation();
    sCollision();
    sRender();
    m_currentFrame++;
}

void Scene_Play::sState()
{
    if (m_player->getComponent<CState>().state != "AIR")
    {
        if ( m_player->getComponent<CInput>().shift )
        {
            if ( !m_player->getComponent<CInput>().shoot)
                setPlayerAnimation("PlayerLying");
            else
                setPlayerAnimation("PlayerLyingShooting");
        }
        else
        {
            if (!m_player->getComponent<CInput>().left && !m_player->getComponent<CInput>().right)
            {
                if ( !m_player->getComponent<CInput>().shoot )
                    setPlayerAnimation("PlayerStanding");
                else
                    setPlayerAnimation("PlayerShooting");
            }
            else
            {
                if ( !m_player->getComponent<CInput>().shoot )
                    setPlayerAnimation("PlayerRunning");
                else
                    setPlayerAnimation("PlayerRunningShooting");
            }
        }
    }
}

void Scene_Play::setPlayerAnimation( std::string name )
{
    if ( name != m_player->getComponent<CAnimation>().animation->getName() )
        m_player->getComponent<CBoundingBox>().switching = true;
    else
        return;

    auto animation = m_game->assets().getAnimation(name);
    m_player->getComponent<CAnimation>().animation = animation;
    if ( name == "PlayerLying" || name == "PlayerLyingShooting")
    {
        m_player->getComponent<CBoundingBox>().setSize(animation->getSize()*m_playerConfig.SCALE);
    }
    else {
        m_player->getComponent<CBoundingBox>().setSize(Vec2(m_playerConfig.CW, m_playerConfig.CH)*m_playerConfig.SCALE);
    }
}

void Scene_Play::sMovement()
{
    Vec2 playerVelocity(0.0f, m_player->getComponent<CTransform>().velocity.y);

    if( m_player->getComponent<CInput>().up && m_player->getComponent<CState>().subState == "CAN_JUMP")
    {
        playerVelocity.y = -m_playerConfig.JUMP;
        setPlayerAnimation("PlayerJumping");
        m_player->getComponent<CState>().subState = "CANNOT_JUMP";
    }

    if ( m_player->getComponent<CInput>().left )
    {
        m_player->getComponent<CTransform>().scale = Vec2(-abs(m_player->getComponent<CTransform>().scale.x),m_player->getComponent<CTransform>().scale.y);
        if ( !m_player->getComponent<CInput>().shift )
        {
            playerVelocity.x = -m_playerConfig.SPEED;
        }
    }

    if ( m_player->getComponent<CInput>().right )
    {
        m_player->getComponent<CTransform>().scale = Vec2(abs(m_player->getComponent<CTransform>().scale.x),m_player->getComponent<CTransform>().scale.y);
        if ( !m_player->getComponent<CInput>().shift )
        {
            playerVelocity.x = m_playerConfig.SPEED;
        }
    }

    if ( m_player->getComponent<CInput>().shoot )
    {
        // spawnBullet();
    }

    m_player->getComponent<CTransform>().velocity = playerVelocity;

    for ( auto e : m_entityManager.getEntities() )
    {
        e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;

        if (e->getComponent<CGravity>().has )
        {
            e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
        }
        // Control max speed
        maxSpeed(e);
        e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
    }
}

void Scene_Play::maxSpeed(std::shared_ptr<Entity> e)
{
    if ( e->getComponent<CTransform>().velocity.y > m_playerConfig.MAXSPEED )
        e->getComponent<CTransform>().velocity.y = m_playerConfig.MAXSPEED;

    if ( e->getComponent<CTransform>().velocity.x > m_playerConfig.MAXSPEED )
        e->getComponent<CTransform>().velocity.x = m_playerConfig.MAXSPEED;

    if ( e->getComponent<CTransform>().velocity.y < -m_playerConfig.MAXSPEED )
        e->getComponent<CTransform>().velocity.y = -m_playerConfig.MAXSPEED;

    if ( e->getComponent<CTransform>().velocity.x < -m_playerConfig.MAXSPEED )
        e->getComponent<CTransform>().velocity.x = -m_playerConfig.MAXSPEED;
}

void Scene_Play::sLifespan()
{
    for ( auto e : m_entityManager.getEntities() )
    {
        if ( e->getComponent<CLifeSpan>().has )
        {
            if ( m_currentFrame <= e->getComponent<CLifeSpan>().frameCreated + e->getComponent<CLifeSpan>().lifespan )
            {
                e->destroy();
            }
        }
    }
}

void Scene_Play::sCollision()
{
    for ( auto e : m_entityManager.getEntities() )
    {
        sCollisionHelper(m_player, e);
        
        for ( auto enemy : m_entityManager.getEntities("enemy"))
        {
            sCollisionHelper(enemy, e);
        }
    }
    if ( m_player->getComponent<CBoundingBox>().switching == true )
        m_player->getComponent<CBoundingBox>().switching = false;
}

void Scene_Play::sCollisionHelper(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> e)
{
    if ( e->tag() != entity->tag() && e->getComponent<CBoundingBox>().has )
    {
        Vec2 overlap        = m_physics.GetOverlap(entity, e);
        Vec2 prevOverlap    = m_physics.GetPreviousOverlap(entity, e);

        if( overlap > 0 )
        {   
            // x overlap only
            if (prevOverlap.y > 0 && prevOverlap.x <= 0)
            {
                if (entity->getComponent<CTransform>().pos.x < e->getComponent<CTransform>().pos.x )
                {
                    entity->getComponent<CTransform>().pos.x -= overlap.x;
                }
                else 
                {
                    entity->getComponent<CTransform>().pos.x += overlap.x;
                }
                entity->getComponent<CTransform>().velocity.x = 0;
            }
            // both x and y overlaps (at the same time)
            else if ( prevOverlap.x <= 0 && prevOverlap.y <= 0 )
            {
                // this is not a typo, this is how this case is being handled
            }
            // y overlap only ... and ... the edge base of bounding box mutation due to animation switch
            else
            {
                if ( prevOverlap > 0 ) std::cout << "EMERGENCY!!!!\nCONSECUTIVE FRAMES WITH OVERLAP\n" << "Prev: " << prevOverlap.x << " " << prevOverlap.y << "\n" << "Curr: " << overlap.x << " " << overlap.y << "\n";

                if (entity->getComponent<CTransform>().pos.y < e->getComponent<CTransform>().pos.y)
                {
                    if ( entity->tag() == m_player->tag() && m_player->getComponent<CState>().subState == "CANNOT_JUMP")
                    {
                        if ( !m_player->getComponent<CInput>().up )
                        {
                            m_player->getComponent<CState>().state = "LAND";
                        }
                        m_player->getComponent<CState>().subState   = "CAN_JUMP";
                    }
                    entity->getComponent<CTransform>().pos.y -= overlap.y;
                }
                else
                {
                    entity->getComponent<CTransform>().pos.y += overlap.y;
                }
                entity->getComponent<CTransform>().velocity.y = 0;
            }
        }
    }
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
            m_player->getComponent<CInput>().up = true;
            if ( m_player->getComponent<CState>().state != "AIR" && m_player->getComponent<CState>().subState == "CAN_JUMP" )
            {
                m_player->getComponent<CState>().state = "AIR";
            }
        }
        else if (action.name() == "RIGHT" )             
        { 
            m_player->getComponent<CInput>().right = true; 
        }
        else if (action.name() == "LEFT" )             
        { 
            m_player->getComponent<CInput>().left = true; 
        }
        else if (action.name() == "SHIFT" )             
        { 
            m_player->getComponent<CInput>().shift = true;
        }
        else if (action.name() == "SHOOT" )
        {
            m_player->getComponent<CInput>().shoot = true;
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
            m_player->getComponent<CInput>().right = false; 
        }
        else if (action.name() == "LEFT" )             
        { 
            m_player->getComponent<CInput>().left = false; 
        }
        else if (action.name() == "SHIFT" )             
        { 
            m_player->getComponent<CInput>().shift = false;
        }
        else if (action.name() == "SHOOT" )
        {
            m_player->getComponent<CInput>().shoot = false;
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
    m_game->quit();
}

void Scene_Play::sRender()
{
    // usleep(250000);
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
            if ( e->getComponent<CBoundingBox>().has )
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




