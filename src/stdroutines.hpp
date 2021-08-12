#ifndef STDROUTINES_HPP
#define STDROUTINES_HPP

#include "entity.hpp"

// Stores data about player abilities
struct PlayerParams
{
    float jump;
    float speed;
};

// Common functions that can be used for entity ai

// Makes entity fall
void run_fall_physics(Entity *entity, float dtime, float g);

// Adds keyboard controls to entity
void run_player_controls(Entity *entity, float dtime, PlayerParams &params);

#endif
