#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

#include "world.hpp"
#include "animation.hpp"
#include "camera.hpp"

class Entity;

typedef unsigned int entityid;
typedef void (*ai_routine_t)(Entity*, float);

class Entity
{
public:
    Entity();

    void setID(entityid id)
    {
        this->id = id;
    }
    
    entityid getID()
    {
        return id;
    }
    
    void setTexture(sf::Texture *texture)
    {
        sprite.setTexture(*texture, true);
    }
    
    void setPosition(float x, float y)
    {
        rect.left = x;
        rect.top = y;
    }
    
    sf::Vector2f getPosition()
    {
        return sf::Vector2f(rect.left, rect.top);
    }
    
    void setSize(float width, float height)
    {
        rect.width = width;
        rect.height = height;
    }
    
    sf::Vector2f getSize()
    {
        return sf::Vector2f(rect.width, rect.height);
    }
    
    void setAIRoutine(ai_routine_t routine)
    {
        ai_routine = routine;
    }
    
    void update(float dtime, World &world);
    
    void collide(World &world, bool by_x);
    
    void onCollideX(int x, int y, World &world);
    void onCollideY(int x, int y, World &world);
    
    bool isCollideEntity(Entity *entity);
    
    void draw(sf::RenderWindow &window);

public:
    // Collision information for ai
    struct { bool blockl, blockr, blocku, blockd; } collision_info;
    
    sf::Vector2f velocity;
    
    AnimationManager animations;

private:
    sf::FloatRect rect;
    
    // ID of entity. 0 means this entity is not in EntityManager
    entityid id;
    
    sf::Sprite sprite;
    
    // If false, collision checks are ignored
    bool do_collide;
    
    ai_routine_t ai_routine;
};

#endif
