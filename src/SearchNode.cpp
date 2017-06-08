#include "SearchNode.hpp"

void hash_combine(std::size_t& seed, std::size_t value)
{
    seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
}



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
    mtx.lock();//to avoid processing the answer while still sending requests
    const std::list<Node>* tmp = answer->getNodes();
    for(std::list<Node>::const_iterator it=tmp->begin(); it!=tmp->end(); ++it)
    {
        Node current = *it;
        if(asked.find(current.getKey())==asked.end())
        {
            askme.push_back(current);
        }
    }
    sort(askme.begin(),askme.end());
    mtx.unlock();
}

void SearchNode::queryTo(Node* answer)
{
    mtx.lock();//to avoid processing the answer while still sending requests
    for(int i=0;i<NBYTE;i++)
    {
        answer[i] = askme.back();
        askme.pop_back();
        asked.insert({{answer[i].getKey(),answer[i]}});
    }
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
        return Distance(*a.getKey(),*c.getKey()) > Distance(*b.getKey(),*c.getKey());
        //reversed order so I can use pop_back and back
    }
};
