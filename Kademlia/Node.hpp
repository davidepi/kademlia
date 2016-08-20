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
    const Key* getKey() const;

private:
    Node();

    Ip my_ip;
    int port_ho; //a volte è int, altre short e altre uint16_t... why?
                 //dipende da come mi alzo :D l'unica sbagliata e' short cmq
    Key* id;
};

#endif