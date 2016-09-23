#ifndef __IP_HPP__
#define __IP_HPP__

#include <cstdlib>  //uint32_t
#include <cstring>  //strcat
#include <iostream> //sprintf

class Ip
{
public:
    Ip(const char* ip);
    Ip(int network_ordered);
    Ip();
    ~Ip();
    uint32_t getIp() const;
    void toString(char output[16]) const;
    bool isLocalhost() const;
    
    bool operator==(const Ip&)const;
    bool operator!=(const Ip&)const;
    
private:
    uint32_t ip;
};

#endif
