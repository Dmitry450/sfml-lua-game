#ifndef WORLD_HPP
#define WORLD_HPP

#define BLOCK_OOB 0
#define BLOCK_AIR 1

#define BLOCK_SIZE 32

#include <stdint.h>
#include <unordered_map>
#include <SFML/Graphics.hpp>

class World
{
public:
    World(uint8_t *world, int width, int height);
    
    uint8_t getAt(int x, int y);
    
    void setAt(int x, int y, uint8_t block);
    
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
    
    void draw(sf::RenderWindow &window);
private:
    uint8_t *world;
    int width, height;
    
    std::unordered_map<uint8_t, int*> block_frames;
    sf::Sprite tile;
};

#endif
