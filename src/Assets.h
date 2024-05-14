#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animation.h"
#include <memory>

class Assets
{
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, std::shared_ptr<Animation>> m_animations;
    std::map<std::string, sf::Sound> m_sounds;
    std::map<std::string, sf::Font> m_fonts;

public:
    void loadFromFile(const std::string& path);
    void addTexture(std::string& name, std::string& path);
    void addAnimation(std::string& name, std::string textureName, size_t frameCount, size_t speed);
    void addSound(std::string& name, std::string& path);
    void addFont(std::string& name, std::string& path);

    sf::Texture& getTexture(std::string& name) ;
    std::shared_ptr<Animation> getAnimation(const std::string& name) ;
    sf::Sound& getSound(std::string& name);
    sf::Font& getFont(const std::string& name);
};
