#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>

class Animation
{
public:
    Animation(float time_per_frame, std::vector<sf::IntRect> animation);
    
    void update(float dtime);
    
    void setSpriteRect(sf::Sprite &sprite);

private:
    float frame_timer;
    
    float time_per_frame;
    
    int frameno;
    
    std::vector<sf::IntRect> animation;
};

class AnimationManager
{
public:
    void addAnimation(const std::string &name, Animation *animation);
    
    void update(float dtime);
    
    void setSpriteRect(sf::Sprite &sprite);
    
    void setAnimation(const std::string &name)
    {
        current = name;
    }
    
private:
    std::string current;

    std::map<std::string, Animation*> animations;
};

#endif
