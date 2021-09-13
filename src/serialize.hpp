#ifndef SERIALIZE_H
#define SERIALIZE_H

#define MAX_STRING_SIZE 65535

#include <cstring>
#include <endian.h>
#include <string>
#include <iostream>
#include <stdint.h>

std::string serializeString(const std::string &str);
std::string deserializeString(std::istream &is);

inline void writeU16(uint8_t *dst, uint16_t i)
{
    uint16_t val = htobe16(i);
    memcpy(dst, &val, 2);
}

inline void writeU16(std::ostream &os, uint16_t i)
{
    uint16_t val = htobe16(i);
    os<<val;
}

inline uint16_t readU16(uint8_t *src)
{
    uint16_t val;
    memcpy(&val, src, 2);
    return be16toh(val);
}

inline uint16_t readU16(std::istream &is)
{
    uint16_t val;
    is>>val;
    return be16toh(val);
}

inline void writeU32(uint8_t *dst, uint32_t i)
{
    uint32_t val = htobe32(i);
    memcpy(dst, &val, 4);
}

inline void writeU32(std::ostream &os, uint32_t i)
{
    uint32_t val = htobe32(i);
    os<<val;
}

inline uint32_t readU32(uint8_t *src)
{
    uint32_t val;
    memcpy(&val, src, 4);
    return be32toh(val);
}

inline uint32_t readU32(std::istream &is)
{
    uint32_t val;
    is>>val;
    return be32toh(val);
}

inline void writeI32(uint8_t *dst, int32_t i)
{
    int32_t val = htobe32(i);
    memcpy(dst, &val, 4);
}

inline void writeI32(std::ostream &os, int32_t i)
{
    int32_t val = htobe32(i);
    os<<val;
}

inline int32_t readI32(uint8_t *src)
{
    int32_t val;
    memcpy(&val, src, 4);
    return be32toh(val);
}

inline int32_t readI32(std::istream &is)
{
    int32_t val;
    is>>val;
    return be32toh(val);
}

#endif
