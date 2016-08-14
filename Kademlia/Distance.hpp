#ifndef __DISTANCE_HPP__
#define __DISTANCE_HPP__

#include "Key.hpp"
//#include "Node.hpp"

class Distance
{
public:
    Distance(Key &k1, Key& k2);
    //Distance(Node& n1, Node& n2);
    ~Distance();
    
    Distance operator+(const Distance&)const;
    void operator+=(const Distance&);
    Distance operator-(const Distance&)const;
    void operator-=(const Distance&);
    bool operator<(const Distance&)const;
    bool operator>(const Distance&)const;
    bool operator<=(const Distance&)const;
    bool operator>=(const Distance&)const;
    bool operator==(const Distance&)const;
    bool operator!=(const Distance&)const;

private:
    Distance();
    uint8_t value[20];
};

#endif