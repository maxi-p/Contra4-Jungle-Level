#include "Assets.h"
#include <memory>
#include <iostream>
#include <fstream>

void Assets::loadFromFile(const std::string& assetsPath)
{
    std::ifstream file;
    file.open(assetsPath);
    std::string category, name, path, textureName;
    size_t numFrames = 1;
    size_t rate = 1;
    while( file >> category)
    {
        if( category == "Texture")
        {
            file >> name >> path;
            addTexture(name, path);
        }
        else if( category == "Animation")
        {
            file >> name >> textureName >> numFrames >> rate;
            addAnimation(name, textureName, numFrames, rate);
        }
        else if( category == "Font")
        {
            file >> name >> path;
            addFont(name, path);
        }
    }
}

void Assets::addTexture(std::string& name, std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cout << "Error in opening a texture file...\n";
        return;
    }
    m_textures[name] = texture;
}

void Assets::addAnimation(std::string& name, std::string textureName, size_t frameCount, size_t speed)
{
    auto animation = std::make_shared<Animation>(name, m_textures[textureName], frameCount, speed);
    m_animations[name] = animation; 
}

void Assets::addSound(std::string& name, std::string& path)
{
}

void Assets::addFont(std::string& name, std::string& path)
{
    sf::Font font;
    if (!font.loadFromFile(path))
    {
        std::cout << "Error in loading a font\n";
        return;
    }
    m_fonts[name] = font;
}


sf::Texture& Assets::getTexture(std::string& name)
{
    return m_textures[name];
}

std::shared_ptr<Animation>Assets::getAnimation(const std::string& name)
{
    return m_animations[name];
}

sf::Sound& Assets::getSound(std::string& name)
{
    return m_sounds[name];
}

sf::Font& Assets::getFont(const std::string& name) 
{
    return m_fonts[name];
}

