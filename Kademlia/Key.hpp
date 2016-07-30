#ifndef __KEY_HPP__
#define __KEY_HPP__

#include <cstdlib> //uint32_t
#include "Ip.hpp"
#define HASHFN SHA1

extern const unsigned short NBITS;

class Key
{
public:
    Key(Ip ip, pid_t pid);
    Key(const char* name);
    ~Key();
    
    const uint8_t* getKey() const;
    
private:
    uint8_t* key;
};


#endif