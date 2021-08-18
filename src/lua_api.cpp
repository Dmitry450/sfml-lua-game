#include <string>

#include "lua_api.hpp"

void api_init(lua_State *L)
{
    std::cout<<"Initializing lua scripting interface, please stand by...\n";
    
    // Std lua libs
    luaL_openlibs(L);
    
    
    // API functions
    lua_register(L, "world_getAt",                 world_getAt);
    lua_register(L, "world_setAt",                 world_setAt);
    lua_register(L, "world_setTilemap",            world_setTilemap);
    lua_register(L, "world_setBlockFrame",         world_setBlockFrame);
    
    lua_register(L, "entity_newEntity",            entity_newEntity);
    lua_register(L, "entity_exists",               entity_exists);
    lua_register(L, "entity_setTexture",           entity_setTexture);
    lua_register(L, "entity_setAnimationsManager", entity_setAnimationsManager);
    lua_register(L, "entity_addAnimation",         entity_addAnimation);
    lua_register(L, "entity_setAnimation",         entity_setAnimation);
    lua_register(L, "entity_mirror",               entity_mirror);
    lua_register(L, "entity_setPosition",          entity_setPosition);
    lua_register(L, "entity_getPosition",          entity_getPosition);
    lua_register(L, "entity_setSize",              entity_setSize);
    lua_register(L, "entity_getSize",              entity_getSize);
    lua_register(L, "entity_getCollisionInfo",     entity_getCollisionInfo);
    lua_register(L, "entity_setVelocity",          entity_setVelocity);
    lua_register(L, "entity_getVelocity",          entity_getVelocity);
    lua_register(L, "entity_delEntity",            entity_delEntity);
    
    lua_register(L, "player_getEntity",            player_getEntity);
    lua_register(L, "entity_getKeys",              player_getKeys);
    lua_register(L, "player_getMouse",             player_getMouse);
    
    lua_register(L, "resources_loadTexture",       resources_loadTexture);
    
    
    // Hooks
    lua_newtable(L);
    
    lua_pushstring(L, "update");
    lua_newtable(L);
    lua_settable(L, -3);
    
    lua_setglobal(L, "hooks");
    
    
    // Metatables
    // Metatable for entity userdata
    /*luaL_newmetatable(L, "Entity");
    
    lua_pushstring(L, "__index");
    
    lua_newtable(L);
    lua_pushstring(L, "set_texture");
    lua_pushcfunction(L, entityud_set_texture);
    lua_settable(L, -3);*/
    // I'll do it later
}

bool run_script(lua_State *L, const char *filename)
{
    int result = luaL_loadfile(L, filename);
    
    if (result == LUA_OK)
    {
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        
        if (result != LUA_OK)
        {
            printerr(L);
            
            return false;
        }
    }
    else
    {
        printerr(L);
        
        return false;
    }
    
    return true;
}

void run_update_hooks(lua_State *L, float dtime)
{
    lua_getglobal(L, "hooks"); // Push hooks
    
    lua_pushstring(L, "update");
    lua_gettable(L, -2); // Push hooks["update"]
    
    int update_idx = lua_absindex(L, -1);
    
    lua_pushnil(L); // Push first key    
    while (lua_next(L, update_idx) != 0) // Pop key, push value and key
    {
        lua_pushnumber(L, dtime); // Push dtime
        lua_call(L, 1, 0); // Pop dtime and value (function)
    }
    
    lua_pop(L, 2); // Pop hooks["update"] and hooks
}

int world_getAt(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 2);
    
    int x = get_lua_integer(L, 1);
    int y = get_lua_integer(L, 2);
    
    lua_pushinteger(L, world->getAt(x, y));
    
    return 1;
}

int world_setAt(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 3);
    
    int x, y, block;
    
    x = get_lua_integer(L, 1);
    y = get_lua_integer(L, 2);
    block = get_lua_integer(L, 3);
    
    if (block < 0 || block > 255)
    {
        luaL_error(L, "world_setAt expected argument 3 to be in range [0; 255]");
    }
    
    world->setAt(x, y, (uint8_t)block);
    
    return 0;
}

int world_setTilemap(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 1);
    
    std::string filename = get_lua_string(L, 1);
    
    sf::Texture *texture = textures->getResource(filename);
    
    if (texture == nullptr)
    {
        luaL_error(L, "no such texture: %s", filename);
    }
    
    world->setTilemap(texture);
    
    return 0;
}

int world_setBlockFrame(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 3);
    
    int block = get_lua_integer(L, 1);
    int x = get_lua_integer(L, 2);
    int y = get_lua_integer(L, 3);
    
    world->setBlockFrame((uint8_t)block, x, y);
    
    return 0;
}

int entity_newEntity(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 0);
    
    Entity *entity = new Entity;
    
    entmgr->addEntity(entity);
    
    lua_pushinteger(L, entity->getID());
    
    return 1;
}

int entity_exists(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 1);
    
    int id = get_lua_integer(L, 1);
    Entity *entity = entmgr->getByID(id);
    
    lua_pushboolean(L, entity != nullptr);
    
    return 1;
}

int entity_setTexture(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 2);
    
    int id = get_lua_integer(L, 1);
    std::string filename = lua_tostring(L, 2);
    
    sf::Texture *texture = textures->getResource(filename);
    
    if (texture == nullptr)
    {
        luaL_error(L, "no such texture: %s", filename);
    }
    
    Entity *entity = get_entity(L, id);
    
    entity->setTexture(texture);
    
    return 0;
}

int entity_setAnimationsManager(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 2);
    
    int id = get_lua_integer(L, 1);
    Entity *entity = get_entity(L, id);
    
    const int animations_idx = 2; // Absolute index in stack
    
    AnimationManager animations;
    
    lua_pushnil(L); // Push first key
    while (lua_next(L, animations_idx) != 0) // Pop key, push key and value
    {
        std::string name = get_lua_string(L, -2);  // Get key
        
        construct_animation(L, name, animations);  // Stack stays same
        
        lua_pop(L, 1); // Pop value
    }
    
    entity->animations = animations;
        
    return 0;
}

int entity_addAnimation(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 3);
    
    int id = get_lua_integer(L, 1);
    Entity *entity = get_entity(L, id);
    
    std::string name = get_lua_string(L, 2);
    
    construct_animation(L, name, entity->animations);
    
    return 0;
}

int entity_setAnimation(lua_State *L)
{
    check_resources(L);
    
    bool has_mirror_arg = check_lua_argc2(L, 2, 3);
    
    int id = get_lua_integer(L, 1);
    Entity *entity = get_entity(L, id);
    
    std::string name = get_lua_string(L, 2);
    
    if (entity->animations.hasAnimation(name))
        entity->animations.setAnimation(name);
    
    if (has_mirror_arg)
        entity->animations.mirror = get_lua_bool(L, 3);
    
    return 0;
}

int entity_mirror(lua_State *L)
{
    check_resources(L);
    
    bool has_mirror_arg = check_lua_argc2(L, 1, 2);
    
    int id = get_lua_integer(L, 1);
    Entity *entity = get_entity(L, id);
    
    entity->animations.mirror = ( has_mirror_arg ? get_lua_bool(L, 2) : !entity->animations.mirror );
    
    return 0;
}

int entity_setPosition(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 3);
    
    //std::cout<<lua_gettop(L);
    
    int id = get_lua_integer(L, 1);
    float x = get_lua_float(L, 2);
    float y = get_lua_float(L, 3);
    
    Entity *entity = get_entity(L, id);
    
    entity->setPosition(x, y);
    
    return 0;
}

int entity_getPosition(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 1);
    
    int id = get_lua_integer(L, 1);
    
    Entity *entity = get_entity(L, id);
    
    auto position = entity->getPosition();
    
    lua_newtable(L);
    
    lua_pushstring(L, "x");
    lua_pushnumber(L, position.x);
    lua_settable(L, -3);
    
    lua_pushstring(L, "y");
    lua_pushnumber(L, position.y);
    lua_settable(L, -3);
    
    return 1;
}

int entity_setSize(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 3);
    
    int id = get_lua_integer(L, 1);
    float w = get_lua_float(L, 2);
    float h = get_lua_float(L, 3);
    
    Entity *entity = get_entity(L, id);
    
    entity->setSize(w, h);
    
    return 0;
}

int entity_getSize(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 1);
    
    int id = get_lua_integer(L, 1);
    
    Entity *entity = get_entity(L, id);
    
    auto size = entity->getSize();
    
    lua_newtable(L);
    
    lua_pushstring(L, "width");
    lua_pushnumber(L, size.x);
    lua_settable(L, -3);
    
    lua_pushstring(L, "height");
    lua_pushnumber(L, size.y);
    lua_settable(L, -3);
    
    return 1;
}

int entity_getCollisionInfo(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 1);
    
    int id = get_lua_integer(L, 1);
    
    Entity *entity = get_entity(L, id);
    
    lua_newtable(L);
    
    lua_pushstring(L, "blockl");
    lua_pushboolean(L, entity->collision_info.blockl);
    lua_settable(L, -3);
    
    lua_pushstring(L, "blockr");
    lua_pushboolean(L, entity->collision_info.blockr);
    lua_settable(L, -3);
    
    lua_pushstring(L, "blocku");
    lua_pushboolean(L, entity->collision_info.blocku);
    lua_settable(L, -3);
    
    lua_pushstring(L, "blockd");
    lua_pushboolean(L, entity->collision_info.blockd);
    lua_settable(L, -3);
    
    return 1;
}

int entity_setVelocity(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 3);
    
    int id = get_lua_integer(L, 1);
    float x = get_lua_float(L, 2);
    float y = get_lua_float(L, 3);
    
    Entity *entity = get_entity(L, id);
    
    entity->velocity.x = x;
    entity->velocity.y = y;
    
    return 0;
}

int entity_getVelocity(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 1);
    
    int id = get_lua_integer(L, 1);
    
    Entity *entity = get_entity(L, id);
    
    lua_newtable(L);
    
    lua_pushstring(L, "x");
    lua_pushnumber(L, entity->velocity.x);
    lua_settable(L, -3);
    
    lua_pushstring(L, "y");
    lua_pushnumber(L, entity->velocity.y);
    lua_settable(L, -3);
    
    return 1;
}

int entity_delEntity(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 1);
    
    int id = get_lua_integer(L, 1);
    
    // Not using get_entity because we dont want to raise error if there is no entity
    Entity *entity = entmgr->getByID(id);
    
    if (entity != nullptr)
    {
        entmgr->delEntity(id);
        
        delete entity;
    }
    
    return 0;
}

int player_getEntity(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 0);
    
    if (player->entity != nullptr)
        lua_pushinteger(L, player->entity->getID());
    else
        lua_pushinteger(L, 0);
    
    return 1;
}

int player_getKeys(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 0);
    
    lua_newtable(L);
    
    lua_pushstring(L, "left");
    lua_pushboolean(L, player->left);
    lua_settable(L, -3);
    
    lua_pushstring(L, "right");
    lua_pushboolean(L, player->right);
    lua_settable(L, -3);
    
    lua_pushstring(L, "up");
    lua_pushboolean(L, player->up);
    lua_settable(L, -3);
    
    lua_pushstring(L, "down");
    lua_pushboolean(L, player->down);
    lua_settable(L, -3);
    
    lua_pushstring(L, "jump");
    lua_pushboolean(L, player->jump);
    lua_settable(L, -3);
    
    return 1;
}

int player_getMouse(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 0);
    
    lua_newtable(L);
    
    lua_pushstring(L, "lmb");
    lua_pushboolean(L, player->lmb);
    lua_settable(L, -3);
    
    lua_pushstring(L, "rmb");
    lua_pushboolean(L, player->rmb);
    lua_settable(L, -3);
    
    lua_pushstring(L, "x");
    lua_pushnumber(L, player->mouse_pos.x);
    lua_settable(L, -3);
    
    lua_pushstring(L, "y");
    lua_pushnumber(L, player->mouse_pos.y);
    lua_settable(L, -3);
    
    return 1;
}

int resources_loadTexture(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 1);
    
    std::string name = get_lua_string(L, 1);
    
    textures->loadResource(name);
    
    return 0;
}
