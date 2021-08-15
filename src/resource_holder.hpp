#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Note: "T" should support bool loadFromFile(const std::string&) method
template <class T>
class ResourceHolder
{
public:
    // Load resource from file and store it in the resources map.
    // Do nothing if resource already loaded
    void loadResource(const std::string &path)
    {
        T *resource;
        
        if (resources.find(path) != resources.end())
            return;
        
        resource = new T;
        
        if (resource->loadFromFile(path))
        {
            resources[path] = resource;
        }
        else
        {
            delete resource; // Cannot load the resource? We don't need that memory anymore
        }
    }
    
    // Get resource from the resources map by name
    T *getResource(const std::string &name)
    {
        auto pair = resources.find(name);
        return ( pair != resources.end() ? pair->second : nullptr );
    }
private:
    std::map<std::string, T*> resources;
};

typedef ResourceHolder<sf::Texture> TextureHolder;
typedef ResourceHolder<sf::SoundBuffer> SoundBufferHolder;

#endif
