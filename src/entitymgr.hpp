#ifndef ENTITYMGR_HPP
#define ENTITYMGR_HPP

#include <unordered_map>
#include <vector>

#include "entity.hpp"
#include "world.hpp"

class EntityManager
{
public:
    // Try to add entity to manager. Returns is entity was added
    bool addEntity(Entity *entity)
    {
        entityid id = getFreeID();
        
        if (id != 0)
        {
            entities[id] = entity;
            entity->setID(id);
        }
        
        return id != 0;
    }
    
    // Remove entity from manager. Do nothing if there is no such entity.
    // This function does not free memory
    void delEntity(entityid id)
    {
        auto pair = entities.find(id);
        
        if (pair != entities.end())
        {
            // Mark entity as removed from manager
            pair->second->setID(0);
        }
        
        entities.erase(id);
    }
    
    // Get entity with given id
    Entity *getByID(entityid id)
    {
        auto pair = entities.find(id);
        
        return ( pair != entities.end() ? pair->second : nullptr );
    }
    
    // Update entities
    void update(float dtime, World &world)
    {
        for (auto it = entities.begin(); it != entities.end(); ++it)
        {
            it->second->update(dtime, world);
        }
    }
    
    void draw(sf::RenderWindow &window)
    {
        for (auto it = entities.begin(); it != entities.end(); ++it)
        {
            it->second->draw(window);
        }
    }
    
    std::vector<int> findCollisions(int id)
    {
        std::vector<int> result;
        
        Entity *entity = getByID(id);
        
        if (entity == nullptr)
            return result;
        
        for (auto it = entities.begin(); it != entities.end(); ++it)
        {
            if (entity->isCollideEntity(it->second))
                result.push_back(it->second->getID());
        }
        
        return result;
    }
    
    std::vector<int> findCollisions(int id, std::vector<int> &entities)
    {
        std::vector<int> result;
        
        Entity *entity = getByID(id);
        
        if (entity == nullptr)
            return result;
        
        for (auto it = entities.begin(); it != entities.end(); ++it)
        {
            Entity *other = getByID(*it);
            
            if (other == nullptr)
                continue;
            
            if (entity->isCollideEntity(other))
                result.push_back(*it);
        }
        
        return result;
    }
    
private:
    // Get next free id or 0 if there is no one
    entityid getFreeID()
    {
        static entityid id = 0;
        
        entityid start_id = id;
        
        while (!isFreeID(++id))
        {
            if (id == start_id)
                return 0;
        }
        
        return id;
    }
    
    // Checks is id counts as free
    bool isFreeID(entityid id)
    {
        return id != 0 && entities.find(id) == entities.end();
    }
    
    std::unordered_map<entityid, Entity *> entities;
};

#endif
