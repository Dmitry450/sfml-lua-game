#ifndef LUA_API_HPP
#define LUA_API_HPP

#include <lua.hpp>
#include <iostream>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "entitymgr.hpp"
#include "entity.hpp"
#include "world.hpp"
#include "resource_holder.hpp"
#include "animation.hpp"
#include "player.hpp"

// Resources required for API
// There should be better way to access them...
extern World *world;
extern EntityManager *entmgr;
extern TextureHolder *textures;
extern Player *player;  // TODO - more players?


// Utility functions

// Checks argument count and raises error if rargc != lua_gettop()
inline void check_lua_argc(lua_State *L, int rargc)
{
    if (lua_gettop(L) != rargc)
    {
        luaL_error(L, "expected %I arguments", rargc);
    }
}

// Same as above but there can be rargc1 or rargc2 arguments.
// If arguments count == rargc2, returns true, otherwise false
inline bool check_lua_argc2(lua_State *L, int rargc1, int rargc2)
{
    int argc = lua_gettop(L);
    
    if (argc != rargc1 && argc != rargc2)
    {
        luaL_error(L, "expected %I or %I arguments", rargc1, rargc2);
    }
    
    return argc == rargc2;
}

// Gets integer from stack, raises error if unable to
inline int get_lua_integer(lua_State *L, int i)
{
    int success = 0;
    int result;
    
    result = lua_tointegerx(L, i, &success);
    
    if (!success)
    {
        luaL_error(L, "stack[%I] expected to be an integer", i);
    }
    
    return result;
}

// Gets float from stack, raises error if unable to
inline float get_lua_float(lua_State *L, int i)
{
    int success = 0;
    float result;
    
    result = lua_tonumberx(L, i, &success);
    
    if (!success)
    {
        luaL_error(L, "stack[%I] expected to be a float", i);
    }
    
    return result;
}

// Gets boolean from stack, raises error if unable to
inline bool get_lua_bool(lua_State *L, int i)
{
    if (!lua_isboolean(L, i))
    {
        luaL_error(L, "stack[%I] expected to be a boolean", i);
    }
    
    return lua_toboolean(L, i);
}

// Gets string from stack, raises error if unable to
// This function is safe to use while iterating through table with lua_next,
// since it just won't load any value if it is not a string
inline const char *get_lua_string(lua_State *L, int i)
{
    if (!lua_isstring(L, i))
    {
        luaL_error(L, "stack[%I] expected to be a string", i);
    }
    
    return lua_tostring(L, i);
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
    if (world == nullptr || entmgr == nullptr || textures == nullptr || player == nullptr)
    {
        luaL_error(L, "resources required for API are unaviable");
    }
}

// Print error occured in pcall
inline void printerr(lua_State *L)
{
    const char *msg = lua_tostring(L, -1);
    std::cout<<msg<<'\n';
    lua_pop(L, 1);
}

// Construct animation from table on top of stack
inline void construct_animation(lua_State *L, std::string &name, AnimationManager &dst)
{
    float time_per_frame;
    int x, y, width, height, frames;
    
    lua_pushstring(L, "time_per_frame"); // Push key
    lua_gettable(L, -2); // Pop key, push value[key]
    
    time_per_frame = get_lua_float(L, -1);
    
    lua_pop(L, 1); // Pop value[key]
    
    
    lua_pushstring(L, "x"); // Push key
    lua_gettable(L, -2); // Pop key, push value[key]
    
    x = get_lua_integer(L, -1);
    
    lua_pop(L, 1); // Pop value[key]
    
    
    lua_pushstring(L, "y"); // Push key
    lua_gettable(L, -2); // Pop key, push value[key]
    
    y = get_lua_integer(L, -1);
    
    lua_pop(L, 1); // Pop value[key]
    
    
    lua_pushstring(L, "width"); // Push key
    lua_gettable(L, -2); // Pop key, push value[key]
    
    width = get_lua_integer(L, -1);
    
    lua_pop(L, 1); // Pop value[key]
    
    
    lua_pushstring(L, "height"); // Push key
    lua_gettable(L, -2); // Pop key, push value[key]
    
    height = get_lua_integer(L, -1);
    
    lua_pop(L, 1); // Pop value[key]
    
    
    lua_pushstring(L, "frames"); // Push key
    lua_gettable(L, -2); // Pop key, push value[key]
    
    frames = get_lua_integer(L, -1);
    
    lua_pop(L, 1); // Pop value[key]
    
    
    sf::IntRect frame(x, y, width, height);
    Animation *anim = nullptr;
    std::vector<sf::IntRect> frames_list;
    
    for (int i = 0; i < frames; i++)
    {
        frames_list.push_back(sf::IntRect(frame));
        frame.left += width;
    }
    
    anim = new Animation(time_per_frame, frames_list);
    
    dst.addAnimation(name, anim);
}

// API

void api_init(lua_State *L);

// Returns is script was loaded without errors
bool run_script(lua_State *L, const char *filename);

void run_update_hooks(lua_State *L, float dtime);


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

// bool entity_exists(int)
// Returns is entity in EntityManager. First argument is entity id
int entity_exists(lua_State *L);

// void entity_setTexture(int, string)
// Sets texture of an entity. First argument is entity id, second
// is texture name. Texture taken from TextureHolder, so be sure to preload it
int entity_setTexture(lua_State *L);

// void entity_setAnimationsManager(int, AnimationsMap)
// Sets entity's animation manager. First argument is entity id
// second is a table where key is animation name and value is
// Animation.
// Animation is a table with keys:
// time_per_frame - float number
// x, y, width, height - int params of the first frame
// frames - int count of HORYZONTAL frames
// mirror - boolean, means would be created mirrored copy of the animation or not
int entity_setAnimationsManager(lua_State *L);

// void entity_addAnimation(int, string, Animation)
// Adds animation to entity. First argument is entity id, second is animation name,
// third is Animation (see entity_setAnimationsManager for Animation specification)
int entity_addAnimation(lua_State *L);

// void entity_mirror(int[, bool])
// Mirrors entity's sprite. First argument is entity id.
// If second argument given, set mirror state instead of toggle
int entity_mirror(lua_State *L);

// void entity_setAnimation(int, string[, bool])
// Select entity's animation. First argument is an entity id, second is animation name.
// Do nothing if there is no such animation.
// Optional third argument means would be used mirrored animation or not. If there is no
// mirrored animation, this argument ignored
int entity_setAnimation(lua_State *L);


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


// Player interface

// int player_getPlayerEntity()
// Returns entity id of players entity. If player has no entity, 0 returned
int player_getEntity(lua_State *L);

// KeyPresses player_getPlayerKeys()
// Returns key presses of player.
// KeyPresses is a table with keys:
// left, right, up, down, jump - booleans, meaning is player presses that keys or not
int player_getKeys(lua_State *L);

// Mouse player_getPlayerMouse()
// Returns players mouse state.
// Mouse is a table with keys:
// lmb, rmb - booleans, meaning is player presses left or right mouse buttons
// x, y - floats, mouse position in world (mouse position in window + camera position)
int player_getMouse(lua_State *L);


// Resources interface

// void resources_loadTexture(string)
// Loads a texture from given path
int resources_loadTexture(lua_State *L);

#endif
