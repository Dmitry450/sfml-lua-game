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
    void loadResource(const std::string &path);
    
    // Get resource from the resources map by name
    T *getResource(const std::string &name);
private:
    std::map<std::string, T*> resources;
};

typedef ResourceHolder<sf::Texture> TextureHolder;
typedef ResourceHolder<sf::SoundBuffer> SoundBufferHolder;

#endif
