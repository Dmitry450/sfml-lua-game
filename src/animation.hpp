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
    
    void setSpriteRect(sf::Sprite &sprite, bool mirror);

private:
    float frame_timer;
    
    float time_per_frame;
    
    int frameno;
    
    std::vector<sf::IntRect> animation;
};

class AnimationManager
{
public:
    AnimationManager():
        mirror(false)
    {}

    void addAnimation(const std::string &name, Animation *animation);
    
    void update(float dtime);
    
    void setSpriteRect(sf::Sprite &sprite);
    
    void setAnimation(const std::string &name)
    {
        current = name;
    }
    
    bool hasAnimation(const std::string &name)
    {
        auto pair = animations.find(name);
        
        return ( pair != animations.end() ? pair->second : nullptr );
    }
    
public:
    bool mirror;
    
private:
    std::string current;

    std::map<std::string, Animation*> animations;
};

#endif
