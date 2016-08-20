#include "Node.hpp"


Node::Node()
{ }

Node::Node(Ip ip, int port)
{
	Node::my_ip = ip;
	Node::port_ho = port;
	Node::id = new Key(ip,port);
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
    delete Node::id;
}

