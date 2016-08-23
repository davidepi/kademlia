#ifndef __NODE_HPP__
#define __NODE_HPP__

#include "Ip.hpp"
#include "Key.hpp"

class Node
{

public:
    Node();
    Node(Ip ip, int port);
    Node(const Node& copied); //rule of three
    Node& operator=(const Node& copied); //rule of three
    //manca la rule of five ma si vive una volta sola e si vive bene senza
    ~Node();
    Ip getIp() const;
    int getPort() const;
    const Key* getKey() const;
    

private:
    bool owner;
    Ip my_ip;
    int port_ho; //a volte è int, altre short e altre uint16_t... why?
                 //dipende da come mi alzo :D l'unica sbagliata e' short cmq
    Key* id;
};

#endif