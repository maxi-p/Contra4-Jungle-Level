#include "Animation.h"
#include <cmath>
#include <iostream>

Animation::Animation()
{

}

Animation::Animation(const std::string& name, const sf::Texture& t)
    : Animation(name, t, 1, 0)
{

}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
    : m_name (name),
    m_sprite (t),
    m_frameCount(frameCount),
    m_currentFrame(0),
    m_speed(speed)
{

    m_size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
    m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame)* m_size.x, 0, m_size.x, m_size.y));
}

// updates the animation to show the next frame, depending on tis speed
// animation loops when it reaches the end
void Animation::update()
{   
    if( m_frameCount != 1) 
    {
        m_gameFrame++;
        if ( m_gameFrame - m_lastUpdate >= m_speed )
        {
            size_t correctFrame = m_currentFrame % m_frameCount;
            m_sprite.setTextureRect(sf::IntRect(correctFrame*m_size.x, 0, m_size.x, m_size.y));
            m_lastUpdate = m_gameFrame;
            m_currentFrame++;
        }
    }
}

const Vec2& Animation::getSize() const
{
    return m_size;
}

const std::string& Animation::getName() const
{
    return m_name;
}

sf::Sprite& Animation::getSprite()
{
    return m_sprite;
}
    
bool Animation::hasEnded() const
{
    // TODO: detect when animation has ended (last frame was played) and return true
    return true;
}
