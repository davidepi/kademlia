#include "Node.hpp"

Node::Node()
{
    Node::reference = new uint8_t[1];
    *(Node::reference) = 1;
    Node::my_ip = Ip(0);
    Node::port_ho = 0;
    Node::id = new Key(0,0);
}

Node::Node(Ip ip, int port)
{
    Node::reference = new uint8_t[1];
    *(Node::reference) = 1;
	Node::my_ip = ip;
	Node::port_ho = port;
	Node::id = new Key(ip,port);
}

Node::Node(const Node& copied)
{
    Node::reference = copied.reference;
    (*(Node::reference))++;
    Node::my_ip = copied.my_ip;
    Node::port_ho = copied.port_ho;
    Node::id = copied.id;
}

Node& Node::operator=(const Node& copied)
{
    Node::reference = copied.reference;
    (*(Node::reference))++;
    Node::my_ip = copied.my_ip;
    Node::port_ho = copied.port_ho;
    Node::id = copied.id;
    return *this;
}

bool Node::operator==(const Node &n)const
{
    return Node::my_ip == n.my_ip && Node::port_ho == n.port_ho;
}

bool Node::operator!=(const Node&n)const
{
    return !(*(this)==n);
}

bool Node::operator<(const Node& n)const
{
    return *(Node::id->getKey()) < *(n.id->getKey());
}

Ip Node::getIp() const
{
	return my_ip;
}

int Node::getPort() const
{
	return port_ho;
}

const Key* Node::getKey() const
{
	return id;
}

bool Node::isEmpty()
{
    if(port_ho==0 || my_ip.getIp()==0)
        return true;
    else
        return false;
}

Node::~Node()
{
    if(--(*(Node::reference)) == 0)
    {
        delete Node::id;
        delete[] Node::reference;
    }
}

