#include "serialize.hpp"
#include "exceptions.hpp"

std::string serializeString(const std::string &str)
{
    std::string result;
    
    char buf[2];
    
    if (str.size() > MAX_STRING_SIZE)
        throw SerializationError("serializeString: too long string for serialization");
    
    result.reserve(2 + str.size());
    
    writeU16((uint8_t *)&buf[0], str.size());
    result.append(buf, 2);
    
    result.append(str);
    
    return result;
}

std::string deserializeString(std::istream &is)
{
    std::string result;
    
    char buf[2];
    
    is.read(buf, 2);
    if (is.gcount() != 2)
        throw SerializationError("deserializeString: could not read size of string");
    
    uint16_t size = readU16((uint8_t *)buf);
    if (size == 0)
        return result;
    
    result.resize(size);
    is.read(&result[0], size);
    
    if (is.gcount() != size)
        throw SerializationError("deserializeString: could not read all characters");
    
    return result;
}
