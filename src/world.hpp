#ifndef WORLD_HPP
#define WORLD_HPP

#define BLOCK_OOB 0
#define BLOCK_AIR 1

#define BLOCK_SIZE 32

#include <cstdint>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <vector>

struct MapObject
{
    int x, y;
    std::string name;
    
    MapObject(int _x, int _y, std::string &_name):
        x(_x),
        y(_y),
        name(_name)
    {}
};

class World
{
public:
    World(uint8_t *world, uint32_t width, uint32_t height);
    
    uint8_t getAt(uint32_t x, uint32_t y);
    
    void setAt(uint32_t x, uint32_t y, uint8_t block);
    
    void setTilemap(sf::Texture *texture)
    {
        tile.setTexture(*texture);
    }
    
    void setBlockFrame(uint8_t block, int x, int y)
    {
        if (block_frames.find(block) != block_frames.end())
        {
            int* last_pos = block_frames[block];
            delete last_pos; // Free memory
        }
        
        // Compiler says i cant just 'block_frames[block] = {x, y}'
        // So i need to do this:
        block_frames[block] = new int[2]; // Allocate memory
        
        // And assign array values like this
        block_frames[block][0] = x;
        block_frames[block][1] = y;
    }
    
    void resize(uint32_t new_width, uint32_t new_height, bool save_old);
    
    std::vector<MapObject*> load(std::istream &in, uint32_t size);
    
    void save(std::ostream &out, std::vector<MapObject*> &map_objects);
    
    void draw(sf::RenderWindow &window);
private:
    uint8_t *world;
    uint32_t width, height;
    
    std::unordered_map<uint8_t, int*> block_frames;
    sf::Sprite tile;
};

#endif
