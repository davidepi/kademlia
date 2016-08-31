#ifndef __DISTANCE_HPP__
#define __DISTANCE_HPP__

#include "Key.hpp"
#include "Node.hpp"

class Distance
{
public:
    Distance(const Key &k1, const Key& k2);
    Distance(Node n1, Node n2);
    ~Distance();
    
    Distance operator+(const Distance&)const;
    void operator+=(const Distance&);
    Distance operator-(const Distance&)const;
    
    uint8_t getDistance()const; //ritorna il numero del primo bit diverso tra
                                //due chiavi partendo da sinistra. 1-based, 0
                                //se le chiavi sono uguali
    
    void operator-=(const Distance&);
    bool operator<(const Distance&)const;
    bool operator>(const Distance&)const;
    bool operator<=(const Distance&)const;
    bool operator>=(const Distance&)const;
    bool operator==(const Distance&)const;
    bool operator!=(const Distance&)const;

    //private:
    Distance();
    uint8_t value[20];
};

#endif