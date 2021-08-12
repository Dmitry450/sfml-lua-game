#include "lua_api.hpp"

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

int entity_newEntity(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 0);
    
    Entity *entity = new Entity;
    
    entmgr->addEntity(entity);
    
    lua_pushinteger(L, entity->getID());
    
    return 1;
}

int entity_setTexture(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 2);
    
    int id = get_lua_integer(L, 1);
    const char *filename = lua_tostring(L, 2);
    
    auto texture = textures->getResource(filename);
    
    if (texture == nullptr)
    {
        luaL_error(L, "no such texture: %s", filename);
    }
    
    Entity *entity = get_entity(L, id);
    
    entity->setTexture(texture);
    
    return 0;
}

int entity_setPosition(lua_State *L)
{
    check_resources(L);
    
    check_lua_argc(L, 3);
    
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
