#include "resource_holder.hpp"

template <class T>
void ResourceHolder<T>::loadResource(const std::string &path)
{
    T *resource;
    
    if (resources.find(path) != resources.end())
        return;
    
    resource = new T;
    
    if (resource->loadFromFile(path))
    {
        resources[path] = resource;
    }
}

template <class T>
T *ResourceHolder<T>::getResource(const std::string &name)
{
    auto pair = resources.find(name);
    return ( pair != resources.end() ? pair->second : nullptr );
}

