#include "world.hpp"
#include "camera.hpp"

World::World(uint8_t *world, int width, int height)
{
    this->world = world;
    this->width = width;
    this->height = height;
}

uint8_t World::getAt(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return BLOCK_OOB;
    
    return world[y*width+x];
}

void World::setAt(int x, int y, uint8_t block)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;
    
    world[y*width+x] = block;
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::RectangleShape rect(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    
    rect.setFillColor(sf::Color::Red);
    
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (world[width*y+x] != BLOCK_AIR)
            {
                rect.setPosition(x*BLOCK_SIZE - camera.x, y*BLOCK_SIZE - camera.y);
                target.draw(rect);
            }
        }
    }
}
