#include "Node.hpp"
#include "Distance.hpp"

std::ostream& operator<<(std::ostream &strm, const Node& n)
{ 
    char ip[16];
    n.getIp().toString(ip);
    return strm << ip << ":" << n.getPort();
}

Node::Node()
{
    Node::reference = new uint8_t[1];
    *(Node::reference) = 1;
    Node::my_ip = Ip();
    Node::port_ho = 0;
    Node::id = new Key(Ip(),0);
}

Node::Node(Ip ip, uint16_t port)
{
    Node::reference = new uint8_t[1];
    *(Node::reference) = 1;
	Node::my_ip = ip;
	Node::port_ho = port;
	Node::id = new Key(ip,htons(port));
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

bool Node::operator<(const Node &n)const
{
    uint64_t thisval = Node::my_ip.getIp();
    uint64_t otherval = n.getIp().getIp();
    thisval<<=32;
    otherval<<=32;
    thisval|=Node::port_ho;
    otherval|=n.getPort();
    return thisval<otherval;
}

Ip Node::getIp() const
{
	return my_ip;
}

uint16_t Node::getPort() const
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
