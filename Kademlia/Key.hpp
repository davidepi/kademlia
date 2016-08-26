#ifndef __KEY_HPP__
#define __KEY_HPP__

#include <cstdlib> //uint32_t
#include <climits> //CHAR_BIT
#include "Ip.hpp"
#include "settings.h"

class Key
{
public:
    Key(Ip ip, pid_t pid);
    Key(const char* name);
    ~Key();
    
    const uint8_t* getKey()const;
    
    bool operator==(const Key& k)const;
    bool operator!=(const Key& k)const;
    
    //void print()const;

    
private:
    uint8_t key[NBYTE];
};


#endif