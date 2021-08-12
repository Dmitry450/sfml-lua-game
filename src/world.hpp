#ifndef WORLD_HPP
#define WORLD_HPP

#define BLOCK_OOB 0
#define BLOCK_AIR 1

#define BLOCK_SIZE 32

#include <stdint.h>
#include <SFML/Graphics.hpp>

#include "camera.hpp"

class World: public sf::Drawable
{
public:
    World(uint8_t *world, int width, int height);
    
    uint8_t getAt(int x, int y);
    
    void setAt(int x, int y, uint8_t block);
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    uint8_t *world;
    int width, height;
};

#endif
