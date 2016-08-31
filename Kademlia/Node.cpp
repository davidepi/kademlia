#include "Node.hpp"


Node::Node() : id(NULL)
{ }

Node::Node(Ip ip, int port)
{
    Node::owner = true;
	Node::my_ip = ip;
	Node::port_ho = port;
	Node::id = new Key(ip,port);
}

Node::Node(const Node& copied)
{
    Node::owner = false;
    Node::my_ip = copied.my_ip;
    Node::port_ho = copied.port_ho;
    Node::id = copied.id;
}

Node& Node::operator=(const Node& copied)
{
    if(Node::owner)
        delete Node::id;
    Node::owner = false;
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

Node::~Node()
{
    if(Node::owner)
        delete Node::id;
}

