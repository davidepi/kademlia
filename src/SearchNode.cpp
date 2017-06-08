#include "SearchNode.hpp"

SearchNode::SearchNode(Node n)
{
    findme = n;
}

SearchNode::~SearchNode()
{
    //TODO
}

void SearchNode::addAnswer(Kbucket* answer)
{
    mtx.lock();
    const std::list<Node>* tmp = answer->getNodes();
    for(std::list<Node>::const_iterator it=tmp->begin(); it!=tmp->end(); ++it)
    {
        Node current = *it;
        if(asked.find(*(current.getKey()))==asked.end())
        {
            askme.push_back(current);
        }
    }
    sort(askme.begin(),askme.end());
    mtx.unlock();
}

struct Compare
{
    Compare(Node c)
    {
        Compare::c = c;
    }
    Node c;
    bool operator()(Node a, Node b)
    {
        return Distance(*a.getKey(),*c.getKey()) < Distance(*b.getKey(),*c.getKey());
    }
};
