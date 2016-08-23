#include "Kbucket.hpp"


Kbucket::Kbucket()
{
	Kbucket::nodeList = new std::list<Node>();
}


void Kbucket::add(Node n)
{
	std::cout << "add" << std::endl;
	if (Kbucket::nodeList->size() < KBUCKET_SIZE)
	{
		Kbucket::nodeList->push_front(n);
	} else {
		//check if last node is still alive
		Node lastActiveNode = Kbucket::nodeList->back(); 
		Kbucket::nodeList->pop_back();

		//check ping
		//rpc_ping(lastActiveNode);

		

		
	}
}


Kbucket::~Kbucket()
{
	delete[] nodeList;
}