#include "SearchNode.hpp"

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
        //reverse order, so I can pop_back from array the lowest distance
    }
};

SearchNode::SearchNode(Node n)
{
    findme = n;
    search_ended = false;
}

SearchNode::~SearchNode()
{
    
}

void SearchNode::addAnswer(Kbucket* answer)
{
    if(search_ended) //no need to add more nodes
        return;
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
    sort(askme.begin(),askme.end(),Compare(findme));
    if(askme.back()==findme)
    {
        search_ended = true;
        askme.clear();
        askme.insert(askme.begin(), findme);
    }
    mtx.unlock();
}

int SearchNode::queryTo(Node* answer)
{
    if(search_ended)
    {
        answer[0] = findme;
        return 0;
    }
    mtx.lock();//to avoid processing the answer while still sending requests
    int i;
    for(i=0;i<ALPHA_REQUESTS;i++)
    {
        if(askme.size()>0)
        {
            answer[i] = askme.back();
            askme.pop_back();
            asked.insert({{answer[i].getKey(),answer[i]}});
        }
        else
            break;
    }
    mtx.unlock();
    return i;
}
