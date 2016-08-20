#ifndef __KBUCKET_HPP__
#define __KBUCKET_HPP__

#include <list>
#include "Node.hpp"

#define K 20

class Node;
class Kbucket 
{

	public:
		Kbucket();
		~Kbucket();
		void add(Node n);

	private:
		std::list<Node>* nodeList;

};

#endif