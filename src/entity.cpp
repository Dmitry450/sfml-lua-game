#include "entity.hpp"

Entity::Entity()
{
    id = 0;
    
    do_collide = true;
    
    ai_routine = nullptr;
}

void Entity::update(float dtime, World &world)
{
    animations.update(dtime);
    
    rect.left += velocity.x * dtime;
    collide(world, true);
    
    rect.top += velocity.y * dtime;
    collide(world, false);
    
    if (ai_routine != nullptr)
    {
        ai_routine(this, dtime);
    }
}

void Entity::draw(sf::RenderWindow &window)
{
    animations.setSpriteRect(sprite);
    sprite.setPosition(sf::Vector2f(rect.left - camera.x, rect.top - camera.y));
    
    window.draw(sprite);
}

void Entity::collide(World &world, bool by_x)
{
    collision_info.blockl = false;
    collision_info.blockr = false;
    collision_info.blocku = false;
    collision_info.blockd = false;
    
    for (int y = rect.top/BLOCK_SIZE; y < (rect.top + rect.height)/BLOCK_SIZE; y++)
    {
        for (int x = rect.left/BLOCK_SIZE; x < (rect.left + rect.width)/BLOCK_SIZE; x++)
        {
            if (by_x)
                onCollideX(x, y, world);
            else
                onCollideY(x, y, world);
        }
    }
}

void Entity::onCollideX(int x, int y, World &world)
{
    if (world.getAt(x, y) != BLOCK_AIR)
    {
        if (velocity.x > 0)
        {
            rect.left = x*BLOCK_SIZE - rect.width;
            collision_info.blockr = true;
        }
        if (velocity.x < 0)
        {
            rect.left = (x+1)*BLOCK_SIZE;
            collision_info.blockl = true;
        }
        
        velocity.x = 0;
    }
}

void Entity::onCollideY(int x, int y, World &world)
{
    if (world.getAt(x, y) != BLOCK_AIR)
    {
        if (velocity.y > 0)
        {
            rect.top = y*BLOCK_SIZE - rect.height;
            collision_info.blockd = true;
        }
        if (velocity.y < 0)
        {
            rect.top = (y+1)*BLOCK_SIZE;
            collision_info.blocku = true;
        }
        
        velocity.y = 0;
    }
}
