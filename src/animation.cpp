#include "animation.hpp"

Animation::Animation(float time_per_frame, std::vector<sf::IntRect> animation)
{
    this->time_per_frame = time_per_frame;
    this->animation = animation;
    
    frame_timer = time_per_frame;
    
    frameno = 0;
}

void Animation::update(float dtime)
{
    frame_timer -= dtime;
    
    if (frame_timer < 0)
    {
        frame_timer = time_per_frame;
        frameno++;
        
        if (frameno == animation.size())
            frameno = 0;
    }
}

void Animation::setSpriteRect(sf::Sprite &sprite, bool mirror)
{
    sf::IntRect rect = sf::IntRect(animation[frameno]);
    
    if (mirror)
    {
        rect.left += rect.width;
        rect.width *= -1;
    }
    
    sprite.setTextureRect(rect);
}

void AnimationManager::addAnimation(const std::string &name, Animation *animation)
{
    if (current.empty())
    {
        current = name;
    }
    
    animations[name] = animation;
}

void AnimationManager::update(float dtime)
{
    if (!animations.empty())
        animations[current]->update(dtime);
}

void AnimationManager::setSpriteRect(sf::Sprite &sprite)
{
    if (!animations.empty())
        animations[current]->setSpriteRect(sprite, mirror);
}


