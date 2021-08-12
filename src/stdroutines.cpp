#include "stdroutines.hpp"
#include <SFML/Window/Keyboard.hpp>

void run_fall_physics(Entity *entity, float dtime, float g)
{
    if (!entity->collision_info.blockd)
        entity->velocity.y += g * dtime;
}

void run_player_controls(Entity *entity, float dtime, PlayerParams &params)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        entity->velocity.x = -params.speed;
        entity->animations.setAnimation("walk_left");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        entity->velocity.x = params.speed;
        entity->animations.setAnimation("walk_right");
    }
    else
    {
        entity->velocity.x = 0;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && entity->collision_info.blockd)
        entity->velocity.y = -params.jump;
}
