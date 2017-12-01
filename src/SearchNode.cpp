#include "SearchNode.hpp"

//comparators ----

bool operator<(const pnode& p1, const pnode& p2) //order by distance
{                                                //higher status first
    return p1.distance!=p2.distance?p1.distance<p2.distance:p1.probed>p2.probed;
}

bool uniqueoperand(pnode* p1, pnode* p2) //uniqueness, keep the one with
{                                        //higher status
    if(p1->node==p2->node)
    {
        p1->probed = p1->probed>p2->probed?p1->probed:p2->probed;
        p2->probed = p1->probed>p2->probed?p1->probed:p2->probed;
        return true;
    }
    else
        return false;
}

// ------

SearchNode::SearchNode(const Node n,const Kbucket* add) : findkey(*(n.getKey()))
{
    for(std::list<Node>::const_iterator it=add->getNodes()->begin();it!=add->getNodes()->end();++it)
    {
        pnode tmp;
        tmp.node = *it;
        tmp.probed = UNKNOWN;
        tmp.distance = Distance(findkey,*(tmp.node.getKey())).getDistance();
        askme.push_back(tmp);
    }
    askme.sort();
}

SearchNode::SearchNode(const Key* k, const Kbucket* add) : findkey(*k)
{
    for(std::list<Node>::const_iterator it=add->getNodes()->begin();it!=add->getNodes()->end();++it)
    {
        pnode tmp;
        tmp.node = *it;
        tmp.probed = UNKNOWN;
        tmp.distance = Distance(findkey,*(tmp.node.getKey())).getDistance();
        askme.push_back(tmp);
    }
    askme.sort();
}

void SearchNode::addAnswer(const Node whoanswer, const Kbucket* a)
{
    mtx.lock();//to avoid processing the answer while still sending requests
    askme.splice(askme.end(), reserve,reserve.begin(),reserve.end());
    reserve.clear();
    for(std::list<pnode>::iterator it=askme.begin();it!=askme.end();it++)
    {
        if(whoanswer == it->node)
            it->probed = ACTIVE;
    }
    //merge would be more efficient, but I need to append UNKNOWN to pnode
    for(std::list<Node>::const_iterator it=a->getNodes()->begin();it!=a->getNodes()->end();++it)
    {
        pnode tmp;
        tmp.node = *it;
        tmp.probed = UNKNOWN;
        tmp.distance = Distance(findkey,*(tmp.node.getKey())).getDistance();
        askme.push_back(tmp);
    }
    askme.sort();
    
    //erase duplicates
    for(std::list<pnode>::iterator it=askme.begin();it!=askme.end();it++)
    {
        std::list<pnode>::iterator second = std::next(it);
        while(second!=askme.end() && second->distance==it->distance)
        {
            //if node is equal, keep higher probed status and return true
            if(uniqueoperand(&*it, &*second))
                second = askme.erase(second); //then erase the second
            else 
                second++;
        }
    }
    
    //if list < kbucket.size EVERYTHING is moved in the reserve
    //probably because of the safety of std::next(askme.begin()
    if(askme.size() > KBUCKET_SIZE)
        reserve.splice(reserve.end(),askme,std::next(askme.begin(),KBUCKET_SIZE),askme.end());
    
    //remove unprobed nodes in the reserve list (much high distance)
    for(std::list<pnode>::iterator i=reserve.begin();i!=reserve.end();i++)
    {
        if(i->probed==UNKNOWN)
            i = reserve.erase(i);
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
    //fast check only if queue is ready
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

void SearchNode::print()const
{
    printf("SearchNode for Key ");
    findkey.print();
    printf(" - Active list - (%d)\n",(int)askme.size());

    for(std::list<pnode>::const_iterator it=askme.begin();it!=askme.end();it++)
    {
        char ipstring[16];
        it->node.getIp().toString(ipstring);
        const char* status = status = it->probed==UNKNOWN?"UNKNOWN":it->probed==ACTIVE?"ACTIVE":"PENDING";
        printf("%s:%hu [%s]",ipstring,(unsigned short)it->node.getPort(),status);
        printf(" Distance: %d\n",Distance(*(it->node.getKey()),findkey).getDistance());
    }
    printf(" -- Reserve list -- (%d)\n",(int)reserve.size());
    for(std::list<pnode>::const_iterator it=reserve.begin();it!=reserve.end();it++)
    {
        char ipstring[16];
        it->node.getIp().toString(ipstring);
        const char* status = status = it->probed==UNKNOWN?"UNKNOWN":it->probed==ACTIVE?"ACTIVE":"PENDING";
        printf("%s:%hu [%s]",ipstring,(unsigned short)it->node.getPort(),status);
        printf(" Distance: %d\n",Distance(*(it->node.getKey()),findkey).getDistance());
    }
}

int SearchNode::getActive()const
{
    int count=0;
    for(std::list<pnode>::const_iterator it=askme.begin();it!=askme.end();it++)
    {
        if(it->probed==ACTIVE)
            count++;
    }
    return count;
}

int SearchNode::getUnknown()const
{
    int count=0;
    for(std::list<pnode>::const_iterator it=askme.begin();it!=askme.end();it++)
    {
        if(it->probed==UNKNOWN)
            count++;
    }
    return count;
}

int SearchNode::getPending()const
{
    int count=0;
    for(std::list<pnode>::const_iterator it=askme.begin();it!=askme.end();it++)
    {
        if(it->probed==PENDING)
            count++;
    }
    return count;
}

void SearchNode::evict(const Node n)
{
    mtx.lock();
    std::list<pnode>::iterator it;
    for(it=askme.begin();it!=askme.end();it++) //search for the correct node
    {
        if(it->probed==PENDING && it->node==n)
        {
            break;
        }
    }
    if(it!=askme.end()) //node not found
    {
        askme.erase(it);
        for(std::list<pnode>::iterator i=reserve.begin();i!=reserve.end();i++)
        {
            if(i->probed==ACTIVE)
            {
                askme.push_back(*i);
                reserve.erase(i);
                break;
            }
        }
    }
    mtx.unlock();
}

void SearchNode::getAnswer(Kbucket *answer)
{
    for(std::list<pnode>::const_iterator it=askme.begin();it!=askme.end();it++)
        if(it->probed==ACTIVE)
            answer->add(it->node);
}
