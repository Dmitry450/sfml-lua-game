#ifndef LUA_API_HPP
#define LUA_API_HPP

#include <lua.hpp>

#include "entitymgr.hpp"
#include "entity.hpp"
#include "world.hpp"
#include "resource_holder.hpp"

// Resources required for API
// There should be better way to access them...
extern World *world;
extern EntityManager *entmgr;
extern TextureHolder *textures;

// Utility functions

// Checks argument count and raises error if rargc != lua_gettop()
inline void check_lua_argc(lua_State *L, int rargc)
{
    if (!lua_gettop(L) == rargc)
    {
        luaL_error(L, "expected %I args", rargc);
    }
}

// Gets integer from stack, raises error if unable to
inline int get_lua_integer(lua_State *L, int i)
{
    int success = 0;
    int result;
    
    success = lua_tointegerx(L, i, &success);
    
    if (!success)
    {
        luaL_error(L, "argument %I expected to be an integer", i);
    }
    
    return result;
}

// Gets float from stack, raises error if unable to
inline float get_lua_float(lua_State *L, int i)
{
    int success = 0;
    float result;
    
    success = lua_tonumberx(L, i, &success);
    
    if (!success)
    {
        luaL_error(L, "argument %I expected to be an integer", i);
    }
    
    return result;
}

// Gets entity from manager, raises error if unable to
inline Entity *get_entity(lua_State *L, int id)
{
    Entity *entity = entmgr->getByID((entityid)id);
    
    if (entity == nullptr)
    {
        luaL_error(L, "no entity with id %I found", id);
    }
    
    return entity;
}

// Checks is resources for api aviable, raises error if not
inline void check_resources(lua_State *L)
{
    if (world == nullptr || entmgr == nullptr || textures == nullptr)
    {
        luaL_error(L, "resources required for API are unaviable");
    }
}

// World interaction interface

// int world_getAt(int, int)
// Returns block at given position
int world_getAt(lua_State *L);

// void world_setAt(int, int, int)
// Sets block at given position. First two arguments are x and y coords, last one
// is block id FROM 0 TO 255
int world_setAt(lua_State *L);


// Entities interface

// int entity_newEntity()
// Tries to create new entity. Returns entity id. 0 means fail
int entity_newEntity(lua_State *L);

// void entity_setTexture(int, string)
// Sets texture of an entity. First argument is entity id, second
// is texture name. Texture taken from TextureHolder, so be sure to preload it
int entity_setTexture(lua_State *L);

// void entity_setAnimationsManager(int, AnimationsSpec)
// Sets entity's animation manager. First argument is entity id
// second is a table where key is animation name and value is
// Animation.
// Animation is a table with keys:
// time_per_frame - float number
// x, y, width, height - int params of the first frame
// frames - int count of HORYZONTAL frames
// mirror - boolean, means would be created mirrored copy of the animation or not
//int entity_setAnimationsManager(lua_State *L);
// Too hard for me for now. I'll return to it later


// void entity_setAnimation(int, string[, bool])
// Select entity's animation. First argument is an entity id, second is animation name.
// Do nothing if there is no such animation.
// Optional third argument means would be used mirrored animation or not. If there is no
// mirrored animation, this argument ignored
//int entity_setAnimation(lua_State *L);
// Not so hard, but useless without entity_setAnimationsManager


// void entity_setPosition(int, float, float)
// Set entity's position. First argument is entity id, other two are x and y coords
int entity_setPosition(lua_State *L);

// Vector2f entity_getPosition(int)
// Returns entity's x and y position in table Vector2f. First argument is entity id
int entity_getPosition(lua_State *L);

// void entity_setSize(int, float, float)
// Set entity's size. First argument is entity id, other two are width and height
int entity_setSize(lua_State *L);

// Size entity_getSize(int)
// Returns entitys width and height in table Size. First argument is entity id
int entity_getSize(lua_State *L);

// CollisionInfo entity_getCollisionInfo(int)
// Returns world collision info for entity. First argument is entity id.
// CollisionInfo is a table with keys:
// blockl, blockr, blocku, blockd - booleans, for left, right, up and down blocks
// that collide with entity
int entity_getCollisionInfo(lua_State *L);

// void entity_setVelocity(int, float, float)
// Set entity's velocity. First argument is entity id, other two are x and y velocity
int entity_setVelocity(lua_State *L);

// Vector2f entity_getVelocity(int)
// Returns entity's x and y velocity in table Vector2f. First argument is entity id
int entity_getVelocity(lua_State *L);

// void entity_delEntity(int)
// Removes entity. First argument is entity id
int entity_delEntity(lua_State *L);


// Resources interface

// void resources_loadTexture(string)
// Loads a texture from given path
int resources_loadTexture(lua_State *L);

#endif
