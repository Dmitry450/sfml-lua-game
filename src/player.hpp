#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/System/Vector2.hpp>

#include "entity.hpp"

class Player
{
public:
    bool left, right, up, down;
    
    bool jump;
    
    bool lmb, rmb;
    
    sf::Vector2f mouse_pos;
    
    Entity *entity;

public:
    Player():
        left(false), right(false), up(false), down(false),
        jump(false),
        lmb(false), rmb(false),
        mouse_pos(0.f, 0.f),
        entity(nullptr)
    {}
    
    bool isAlive()
    {
        if (entity != nullptr)
        {
            return entity->getID() != 0;
        }
        
        return false;
    }
};

#endif
