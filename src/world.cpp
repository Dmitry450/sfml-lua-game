#include "world.hpp"
#include "camera.hpp"
#include "exceptions.hpp"
#include "serialize.hpp"

#include <iostream>

World::World(uint8_t *world, uint32_t width, uint32_t height)
{
    this->world = world;
    this->width = width;
    this->height = height;
}

uint8_t World::getAt(uint32_t x, uint32_t y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return BLOCK_OOB;
    
    return world[y*width+x];
}

void World::setAt(uint32_t x, uint32_t y, uint8_t block)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;
    
    world[y*width+x] = block;
}

void World::resize(uint32_t new_width, uint32_t new_height, bool save_old = true)
{
        uint8_t *new_world = new uint8_t[new_width*new_height];
        
        // Fill new world with BLOCK_AIR
        for (uint32_t y = 0; y < new_height; y++)
        {
            for (uint32_t x = 0; x < new_width; x++)
            {
                new_world[y*width + x] = BLOCK_AIR;
            }
        }
        
        // Copy old world into new
        if (save_old)
        {
            for (uint32_t y = 0; y < height; y++)
            {
                for (uint32_t x = 0; x < width; x++)
                {
                    new_world[y*width + x] = world[y*width + x];
                }
            }
        }
        
        delete world;
        
        world = new_world;
        width = new_width;
        height = new_height;
    }

std::vector<MapObject*> World::load(std::istream &in, uint32_t size)
{
    width = readU32(in);
    
    height = readU32(in);
    
    if (world != nullptr)
    {
        delete world; // free memory used for previous world
    }
    
    // Allocate memory for new world
    world = new uint8_t[width * height];
    
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            in>>world[y*width + x];
        }
    }
    
    std::cout<<"Read world "<<width<<'x'<<height<<" blocks\n";
    
    // World read, now read map objects
    std::vector<MapObject*> objects;
        
    // Each map object have fields:
    // int x, y
    // string name (with uint16 name size at beginning)
    
    while (!in.eof())
    {
        int x, y;
        std::string name;
        
        x = readI32(in);
        
        y = readI32(in);
        
        name = deserializeString(in);
        
        std::cout<<"Read MapObject{x: "<<x<<", y: "<<y<<", name: \""<<name<<"\"}\n";
        
        objects.push_back(new MapObject(x, y, name));
    }
    
    return objects;
}

void World::save(std::ostream &out, std::vector<MapObject*> &map_objects)
{
    writeU32(out, width);
    writeU32(out, height);
    
    out<<world;
    
    for (auto it = map_objects.begin(); it != map_objects.end(); ++it)
    {
        writeI32(out, (*it)->x);
        writeI32(out, (*it)->y);
        
        out<<serializeString((*it)->name);
    }
}

void World::draw(sf::RenderWindow &window)
{
    sf::RectangleShape rect(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    
    
    rect.setFillColor(sf::Color::Red);
    
    for (uint32_t x = 0; x < width; x++)
    {
        for (uint32_t y = 0; y < height; y++)
        {
            uint8_t block = world[width*y+x];
            
            if (block != BLOCK_AIR)
            {
                if (tile.getTexture() != nullptr and block_frames.find(block) != block_frames.end())
                {
                    int *pos = block_frames.find(block)->second;
                    tile.setTextureRect(sf::IntRect(pos[0]*BLOCK_SIZE, pos[1]*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE));
                    tile.setPosition(x*BLOCK_SIZE - (float)camera.x, y*BLOCK_SIZE - (float)camera.y);
                    window.draw(tile);
                }
                else
                {
                    rect.setPosition(x*BLOCK_SIZE - (float)camera.x, y*BLOCK_SIZE - (float)camera.y);
                    window.draw(rect);
                }
            }
        }
    }
}
