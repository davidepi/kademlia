#ifndef __DISTANCE_HPP__
#define __DISTANCE_HPP__

#include "Key.hpp"
#include "Node.hpp"

class Distance
{
public:
    Distance(Key &k1, Key& k2);
    Distance(Node n1, Node n2);
    ~Distance();
    short getDistance()const; //ritorna il numero del primo bit diverso tra
                                //due chiavi partendo da sinistra. 1-based, 0
                                //se le chiavi sono uguali
    
    bool operator<(const Distance&)const;
    bool operator>(const Distance&)const;
    bool operator<=(const Distance&)const;
    bool operator>=(const Distance&)const;
    bool operator==(const Distance&)const;
    bool operator!=(const Distance&)const;

private:
    uint8_t value[20];
};

#endif