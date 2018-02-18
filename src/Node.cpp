#include "Node.hpp"
#include "Distance.hpp"

std::ostream& operator<<(std::ostream &strm, const Node& n)
{ 
    char ip[16];
    n.getIp().toString(ip);
    return strm << ip << ":" << n.getPort();
}

Node::Node() : id((uint32_t)_LOCALHOST_NO_,0)
{
    Node::my_ip = Ip();
    Node::port_ho = 0;
}

Node::Node(Ip ip, uint16_t port) : id(ip,htons(port))
{
	Node::my_ip = ip;
	Node::port_ho = port;
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
	return &id;
}

bool Node::isEmpty()
{
    if(port_ho==0 || my_ip.getIp()==0)
        return true;
    else
        return false;
}
