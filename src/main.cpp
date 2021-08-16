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
#include "player.hpp"
#include <stdint.h>
#include <iostream>
#include <unistd.h>
#include <cerrno>

Camera camera = { 0, 0 };

World *world = nullptr;
EntityManager *entmgr = nullptr;
TextureHolder *textures = nullptr;
Player *player = nullptr;


int main(int argc, char **argv)
{
    bool paused = false;
    
    if (argc != 2)
    {
        std::cout<<"Usage: sfmlgame <dir>\n<dir> must have main.lua file\n";
        return EXIT_FAILURE;
    }
    else if (chdir(argv[1]) != 0)
    {
        perror("Error opening game directory");
        return EXIT_FAILURE;
    }
    
    const int win_width = 800;
    const int win_height = 600;
    
    sf::RenderWindow window(sf::VideoMode(win_width, win_height), "SFML Game!");
    window.setFramerateLimit(60);
    
    const int world_width = 8;
    const int world_height = 10;
    
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
    
    world = new World(worlddata, world_width, world_height);
    entmgr = new EntityManager;
    textures = new TextureHolder;
    player = new Player();
    
    lua_State *L = luaL_newstate();
    
    api_init(L);
    
    if (!run_script(L, "main.lua"))
        return EXIT_FAILURE;

    // BEGIN TEST PLAYER CODE
    PlayerParams params;
    params.jump = 400;
    params.speed = 300;
    
    player->entity = new Entity;
    
    player->entity->setSize(32, 32);
    player->entity->setPosition(40, 40);
    
    textures->loadResource("resources/char.png");
    player->entity->setTexture(textures->getResource("resources/char.png"));
    
    Animation walk(0.2, {sf::IntRect(0, 0, 32, 32), sf::IntRect(32, 0, 32, 32), sf::IntRect(64, 0, 32, 32)});
    
    AnimationManager playeranims;
    playeranims.addAnimation("walk", &walk);
    
    player->entity->animations = playeranims;
    
    entmgr->addEntity(player->entity);
    
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
                
                case sf::Event::LostFocus:
                    paused = true;
                    break;
                    
                case sf::Event::GainedFocus:
                    paused = false;
                    break;
                
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Left:
                            player->left = true;
                            break;
                        
                        case sf::Keyboard::Right:
                            player->right = true;
                            break;
                        
                        case sf::Keyboard::Up:
                            player->up = true;
                            break;
                        
                        case sf::Keyboard::Down:
                            player->down = true;
                            break;
                        
                        case sf::Keyboard::Space:
                            player->jump = true;
                            break;
                        
                        default:
                            break;
                    }
                    break;
                
                case sf::Event::KeyReleased:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Left:
                            player->left = false;
                            break;
                        
                        case sf::Keyboard::Right:
                            player->right = false;
                            break;
                        
                        case sf::Keyboard::Up:
                            player->up = false;
                            break;
                        
                        case sf::Keyboard::Down:
                            player->down = false;
                            break;
                        
                        case sf::Keyboard::Space:
                            player->jump = false;
                            break;
                        
                        default:
                            break;
                    }
                    break;
                
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Right)
                        player->rmb = true;
                    else if (event.mouseButton.button == sf::Mouse::Left)
                        player->lmb = true;
                    break;
                
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Right)
                        player->rmb = false;
                    else if (event.mouseButton.button == sf::Mouse::Left)
                        player->lmb = false;
                    break;
                
                default:
                    break;

            }
        }
        
        if (!paused)
        {
            
            if (player->entity != nullptr)
            {
                auto playerpos = player->entity->getPosition();
                camera.x = playerpos.x - win_width/2;
                camera.y = playerpos.y - win_height/2;
            }
            
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            
            player->mouse_pos.x = (float)mouse_pos.x + camera.x;
            player->mouse_pos.y = (float)mouse_pos.y + camera.y;
            
            entmgr->update(dtime, *world);
            
            run_update_hooks(L, dtime);
            
            run_fall_physics(player->entity, dtime, 320);
            run_player_controls(player->entity, dtime, params);

            
            window.clear(sf::Color(40, 79, 23, 128));
            
            window.draw(*world);
            
            entmgr->draw(window);
        }
        
        window.display();
    }
    
    return EXIT_SUCCESS;
}
