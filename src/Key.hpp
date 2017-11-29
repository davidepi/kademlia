#ifndef __KEY_HPP__
#define __KEY_HPP__

#include <cstdlib> //uint32_t
#include <climits> //CHAR_BIT
#include <cstdio>
#include <sstream>
#include <functional>
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
    
    std::size_t operator()(const Key& c) const; //for the hash map
    
    void craft(const uint8_t* bytes);
    void print()const;

    
private:
    uint8_t key[NBYTE];
};
std::ostream& operator<<(std::ostream& strm, const Key& k);

namespace std
{
    template <> struct hash<Key>
    {
        size_t operator()(const Key & k) const
        {
            using std::hash;
            return ((hash<uint8_t>()(k.getKey()[0]) >> 1)
             ^ (hash<uint8_t>()(k.getKey()[1]) >> 2)
             ^ (hash<uint8_t>()(k.getKey()[2]) >> 3)
             ^ (hash<uint8_t>()(k.getKey()[3]) >> 4)
             ^ (hash<uint8_t>()(k.getKey()[4]) >> 5)
             ^ (hash<uint8_t>()(k.getKey()[5]) >> 6)
             ^ (hash<uint8_t>()(k.getKey()[6]) >> 7)
             ^ (hash<uint8_t>()(k.getKey()[7]) >> 8)
             ^ (hash<uint8_t>()(k.getKey()[8]) >> 9)
             ^ (hash<uint8_t>()(k.getKey()[9]) >> 10)
             ^ (hash<uint8_t>()(k.getKey()[10]) >> 11)
             ^ (hash<uint8_t>()(k.getKey()[11]) >> 12)
             ^ (hash<uint8_t>()(k.getKey()[12]) >> 13)
             ^ (hash<uint8_t>()(k.getKey()[13]) >> 14)
             ^ (hash<uint8_t>()(k.getKey()[14]) >> 15)
             ^ (hash<uint8_t>()(k.getKey()[15]) >> 16)
             ^ (hash<uint8_t>()(k.getKey()[16]) >> 17)
             ^ (hash<uint8_t>()(k.getKey()[17]) >> 18)
             ^ (hash<uint8_t>()(k.getKey()[18]) >> 19)
             ^ (hash<uint8_t>()(k.getKey()[19]) >> 20));
        }
    };
}

#endif
