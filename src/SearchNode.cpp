#include "SearchNode.hpp"
#include "Performer.hpp"

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
    std::list<Node>::const_iterator it;
    for(it=add->getNodes()->begin();it!=add->getNodes()->end();++it)
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
    std::list<Node>::const_iterator it;
    for(it=add->getNodes()->begin();it!=add->getNodes()->end();++it)
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
    mtx.lock();//to avoid processing the answer while clean() is running
               //merge askme and reserve in a single list
    askme.splice(askme.end(), reserve);
    reserve.clear();
    bool found = false;
    for(std::list<pnode>::iterator it=askme.begin();it!=askme.end();it++)
    {
        if(whoanswer == it->node)
        {
            found = true;
            it->probed = ACTIVE;
        }
    }
    if(!found) //node is not in the list, either it was removed or it was fake
    {
        mtx.unlock();
        return;
    }
    std::list<Node>::const_iterator it;
    //merge would be more efficient, but I need to append UNKNOWN to pnode
    for(it=a->getNodes()->begin();it!=a->getNodes()->end();++it)
    {
        pnode tmp;
        tmp.node = *it;
        tmp.probed = UNKNOWN;
        tmp.distance = Distance(findkey,*(tmp.node.getKey())).getDistance();
        askme.push_back(std::move(tmp));
    }
    //sort by distance
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
    if(askme.size() > KBUCKET_SIZE)
        reserve.splice(reserve.end(),askme,
                       std::next(askme.begin(),KBUCKET_SIZE),askme.end());
    
    //remove unprobed nodes in the reserve list
    for(std::list<pnode>::iterator i=reserve.begin();i!=reserve.end();i++)
    {
        if(i->probed==UNKNOWN)
            i = reserve.erase(i);
    }
    mtx.unlock();
}

int SearchNode::queryTo(Node* answer)
{
    mtx.lock();//to avoid processing the answer while clean() is running
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
            //not probed, set the iterator so I know I have to ping this one
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
        retval = -1;
    }
    else
    {
        int i = 0;
        while(it!=askme.end() && i!=ALPHA_REQUESTS)
        {
            if(it->probed == UNKNOWN)
            {
                answer[i] = it->node;
                it->queryTime = std::chrono::system_clock::now();
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
    std::list<pnode>::const_iterator it;
    for(it=askme.begin();it!=askme.end();it++)
    {
        char ipstring[16];
        it->node.getIp().toString(ipstring);
        const char* status = it->probed==UNKNOWN?
        "UNKNOWN":
        it->probed==ACTIVE?
        "ACTIVE":"PENDING";
        printf("%s:%hu [%s]",ipstring,(uint16_t)it->node.getPort(),status);
        printf(" Distance: %d\n",it->distance);
    }
    printf(" -- Reserve list -- (%d)\n",(int)reserve.size());
    for(it=reserve.begin();it!=reserve.end();it++)
    {
        char ipstring[16];
        it->node.getIp().toString(ipstring);
        const char* status = it->probed==UNKNOWN?
        "UNKNOWN":
        it->probed==ACTIVE?
        "ACTIVE":"PENDING";
        printf("%s:%hu [%s]",ipstring,(uint16_t)it->node.getPort(),status);
        printf(" Distance: %d\n",it->distance);
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

void SearchNode::clean()
{
    using namespace std::chrono;
    mtx.lock();
    system_clock::time_point now=std::chrono::system_clock::now();
    std::list<pnode>::iterator it = SearchNode::askme.begin();
    bool erased = false;
    int pending = 0;
    while(it!=SearchNode::askme.end())
    {
        if(it->probed==PENDING)
            pending++;
        if(it->probed==PENDING &&
           duration_cast<seconds>(now-it->queryTime).count()>TIMEOUT)
        {
            Logger::getInstance().logFile("ssn",LOGGER_SEARCHNODE,
                                            "Erased node",&(it->node));
            it = SearchNode::askme.erase(it);
            erased = true;
            pending--;
        }
        else
        {
            it++;
        }
    }
    //none will ever answer me
    if(erased && pending==0)
    {
        Logger::getInstance().logFile("ss",LOGGER_SEARCHNODE,
                  "Sending empty Kbucket because every pending node timed out");
        Node me(Messenger::getInstance().getIp(),
                Messenger::getInstance().getPort());
        Kbucket empty;
        Message msg = generate_find_node_answer(&findkey, &empty);
        Messenger::getInstance().sendMessage(me, msg);
    }
    mtx.unlock();
}

void SearchNode::getAnswer(Kbucket *answer)
{
    for(std::list<pnode>::const_iterator it=askme.begin();it!=askme.end();it++)
        if(it->probed==ACTIVE)
            answer->addNode(it->node);
}
