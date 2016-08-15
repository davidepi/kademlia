#include "Node.hpp"

#include <sys/types.h> //pid
#include <unistd.h> //pid


Node::Node()
{ }

Node::Node(Ip ip, int port)
{
	Node::my_ip = ip;
	Node::port_ho = port;
	Key key(ip, getpid()); //dovremmo mettere la porta penso
	Node::id = &key;

	for(int i=0; i<160; i++)
	{
	 	Node::kBucketArray[i] = new Kbucket();
	}
}

Ip Node::getIp() const
{
	return my_ip;
}

int Node::getPort() const
{
	return port_ho;
}

Key Node::getKey() const
{
	return *id;
}

Kbucket* Node::getKBucket(int index)
{
	return kBucketArray[index];
}

