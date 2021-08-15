#include <SFML/Graphics.hpp>
#include <lua.hpp>
#include "camera.hpp"
#include "world.hpp"
#include "entitymgr.hpp"
#include "entity.hpp"
#include "animation.hpp"
#include "stdroutines.hpp"
#include "resource_holder.hpp"
#include "lua_api.hpp"
#include <stdint.h>
#include <iostream>

Camera camera = { 0, 0 };

World *world = nullptr;
EntityManager *entmgr = nullptr;
TextureHolder *textures = nullptr;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game!");
    window.setFramerateLimit(60);
    
    const int width = 8;
    const int height = 10;
    
    uint8_t worlddata[] = {
                            2, 2, 2, 2, 2, 2, 2, 2,
                            2, 1, 1, 1, 1, 1, 1, 2,
                            2, 1, 1, 1, 1, 1, 1, 2,
                            2, 1, 1, 1, 1, 2, 1, 2,
                            2, 1, 1, 1, 1, 2, 1, 2,
                            2, 1, 1, 2, 2, 2, 1, 2,
                            2, 1, 1, 1, 1, 1, 1, 2,
                            2, 1, 1, 1, 1, 1, 1, 2,
                            2, 1, 1, 1, 1, 1, 1, 2,
                            2, 2, 2, 2, 2, 2, 2, 2, };
    
    world = new World(worlddata, width, height);
    entmgr = new EntityManager;
    textures = new TextureHolder;
    
    lua_State *L = luaL_newstate();
    
    api_init(L);
    
    run_script(L, "scripts/test.lua");
    
    // BEGIN TEST PLAYER CODE
    PlayerParams params;
    params.jump = 400;
    params.speed = 300;
    
    Entity *player = new Entity;
    
    player->setSize(32, 32);
    player->setPosition(40, 40);
    
    textures->loadResource("resources/char.png");
    player->setTexture(textures->getResource("resources/char.png"));
    
    Animation walk(0.2, {sf::IntRect(0, 0, 32, 32), sf::IntRect(32, 0, 32, 32), sf::IntRect(64, 0, 32, 32)});
    
    AnimationManager playeranims;
    playeranims.addAnimation("walk", &walk);
    
    player->animations = playeranims;
    
    entmgr->addEntity(player);
    
    // END TEST PLAYER CODE
    
    sf::Clock clock;
    
    while (window.isOpen())
    {
        float dtime = clock.getElapsedTime().asSeconds();
        clock.restart();
        
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                
                default:
                    break;

            }
        }
        
        auto playerpos = player->getPosition();
        camera.x = playerpos.x - 400;
        camera.y = playerpos.y - 300;
        
        entmgr->update(dtime, *world);
        
        run_update_hooks(L, dtime);
        
        run_fall_physics(player, dtime, 320);
        run_player_controls(player, dtime, params);
        
        window.clear(sf::Color(40, 79, 23, 128));
        
        window.draw(*world);
        
        entmgr->draw(window);
        
        window.display();
    }
    
    return EXIT_SUCCESS;
}
