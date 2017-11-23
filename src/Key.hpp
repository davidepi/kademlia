#ifndef __KEY_HPP__
#define __KEY_HPP__

#include <cstdlib> //uint32_t
#include <climits> //CHAR_BIT
#include <cstdio>
#include <sstream>
#include "Ip.hpp"
#include "settings.h"

class Key
{
public:
    Key(Ip ip, pid_t pid);
    Key(const char* name);
    Key();
    ~Key();
    
    const uint8_t* getKey()const;
    
    bool operator==(const Key& k)const;
    bool operator!=(const Key& k)const;
    bool operator<(const Key& k)const;
    
    std::size_t operator()(const Key* c) const; //for the hash map
    
    void craft(const uint8_t* bytes);
    void print()const;

    
private:
    uint8_t key[NBYTE];
};

void hash_combine(std::size_t& seed, std::size_t value);
std::ostream& operator<<(std::ostream& strm, const Key& k);

#endif
