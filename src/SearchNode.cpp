#include "SearchNode.hpp"

struct Compare {
    
    Compare(const Key* c) {
        Compare::c = c;
    }
    const Key* c;
    
    bool operator()(const pnode& a, const pnode& b) {
        return Distance(*a.node.getKey(), *c) < Distance(*b.node.getKey(), *c);
    }
};

bool uniqueoperand(pnode* p1, pnode* p2)
{
    if(p1->node==p2->node)
    {
        p1->probed = p1->probed>p2->probed?p1->probed:p2->probed;
        p2->probed = p1->probed>p2->probed?p1->probed:p2->probed;
        return true;
    }
    else
        return false;
}

SearchNode::SearchNode(const Node n,const Kbucket* add) : findkey(*(n.getKey()))
{
    for(std::list<Node>::const_iterator it=add->getNodes()->begin();it!=add->getNodes()->end();++it)
    {
        pnode tmp;
        tmp.node = *it;
        tmp.probed = UNKNOWN;
        askme.push_back(tmp);
    }
}

SearchNode::SearchNode(const Key* k, const Kbucket* add) : findkey(*k)
{
    for(std::list<Node>::const_iterator it=add->getNodes()->begin();it!=add->getNodes()->end();++it)
    {
        pnode tmp;
        tmp.node = *it;
        tmp.probed = UNKNOWN;
        askme.push_back(tmp);
    }
}

SearchNode::~SearchNode()
{
    
}

void SearchNode::addAnswer(const Node whoanswer, const Kbucket* a)
{
    mtx.lock();//to avoid processing the answer while still sending requests
    bool found = false;
    askme.splice(askme.end(), reserve,reserve.begin(),reserve.end());
    reserve.clear();
    for(std::list<pnode>::iterator it=askme.begin();it!=askme.end();it++)
    {
        if(whoanswer == it->node)
        {
            it->probed = ACTIVE;
            found = true;
        }
    }
    //merge would be more efficient, but I need to append UNKNOWN to pnode
    for(std::list<Node>::const_iterator it=a->getNodes()->begin();it!=a->getNodes()->end();++it)
    {
        pnode tmp;
        tmp.node = *it;
        tmp.probed = UNKNOWN;
        askme.push_back(tmp);
    }
    askme.sort(Compare(&findkey));
    for(std::list<pnode>::iterator it=std::next(askme.begin());it!=askme.end();it++)
    {
        if(uniqueoperand(&*it, &*(std::prev(it))))
          askme.erase(it);
    }
    reserve.splice(reserve.end(),askme,std::next(askme.begin(),KBUCKET_SIZE),askme.end());
    //remove unprobed nodes in the reserve list (much high distance)
    for(std::list<pnode>::iterator i=reserve.begin();i!=reserve.end();i++)
    {
        if(i->probed==UNKNOWN)
            reserve.erase(i);
    }
    mtx.unlock();
}

int SearchNode::queryTo(Node* answer)
{
    mtx.lock();//to avoid processing the answer while still sending requests
    bool check = true;
    int retval;
    //check the queue status
    std::list<pnode>::iterator it;
    for(it=askme.begin();it!=askme.end();it++)
    {
        //missing ping answer for this node
        if(it->probed != ACTIVE)
        {
            check = false;
            //node not probed, set the iterator so I know I have to ping this one
            if(it->probed == UNKNOWN)
                break;
            else //node already probed, but missing answer
                continue;
        }
    }
    if(check) //completed
        retval = 0;
    else if(it==askme.end())
    {
        //???
        //TODO: every node has been pinged, but some answers are missing
        retval = -1;
    }
    else
    {
        int i = 0;
        while(it!=askme.end() && i!=3)
        {
            if(it->probed == UNKNOWN)
            {
                answer[i] = it->node;
                it->probed = PENDING;
                i++;
            }
            it = std::next(it);
        }
        retval = i;
    }
    mtx.unlock();
    return retval;
}
