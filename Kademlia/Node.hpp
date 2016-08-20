#ifndef __NODE_HPP__
#define __NODE_HPP__

#include "Ip.hpp"
#include "Key.hpp"
#include "Kbucket.hpp"

class Kbucket;
class Node
{

public:
    Node(Ip ip, int port);
    ~Node();
    Ip getIp() const;
    int getPort() const;
    Key getKey() const;
    Kbucket getKBucket(int index);



private:
    Node();

    Ip my_ip;
    int port_ho; //a volte è int, altre short e altre uint16_t... why?
    Key* id;
    Kbucket* kBucketArray;
    
};

#endif