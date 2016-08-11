#ifndef __KEY_HPP__
#define __KEY_HPP__

#include <cstdlib> //uint32_t
#include "Ip.hpp"
#define HASHFN SHA1

class Key
{
public:
    Key(Ip ip, pid_t pid);
    Key(const char* name);
    ~Key();
    
    const uint8_t* getKey()const;
    
    bool operator==(const Key& k)const;
    bool operator!=(const Key& k)const;
    
private:
    Key();
    uint8_t* key;
};


#endif