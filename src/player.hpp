#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Vector.hpp>

#include "entity.hpp"

class Player
{
public:
    bool left, right, up, down;
    
    bool lmb, rmb;
    
    sf::Vector2f mouse_pos;
    
    Entity *entity;
    
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
