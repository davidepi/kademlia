#include "Kbucket.hpp"
#include "Updater.hpp"

void Kbucket::addNode(const Node n)
{
    //if already in queue push to front
    bool replaced;
    replaced = replaceNode(n, n);
    
    if(!replaced)
    {
        //if not in queue, add it
        if (Kbucket::nodeList.size() < KBUCKET_SIZE)
        {
            mtx.lock();
            if (Kbucket::nodeList.size() < KBUCKET_SIZE)
                Kbucket::nodeList.push_front(n);
            mtx.unlock();
            Logger::getInstance().logFormat("ssn",Logger::KBUCKET,
                                            "Added",&n);
        }
        else
        {
            //check if last node is still alive
            Node lastActiveNode = Kbucket::nodeList.back();
            //this  manages the update bucket when it is full
            Updater* updater = Updater::getInstance();
            updater->checkUpdateBucket(lastActiveNode, n, this);
        }
    }
    
}

void Kbucket::deleteNode(const Node n)
{
    
    for (std::list<Node>::const_iterator it = Kbucket::nodeList.begin();
         it != Kbucket::nodeList.end(); ++it)
    {
        if(n == *it)
        {
            Kbucket::mtx.lock();
            if(n == *it)
            {
                Kbucket::nodeList.remove(*it);
                Logger::getInstance().logFormat("ssn", Logger::KBUCKET,
                                                "Removed", &n);
            }
            Kbucket::mtx.unlock();
            return;
        }
    }
}

bool Kbucket::replaceNode(const Node oldNode, const Node newNode)
{
    bool retval = false;
    for (std::list<Node>::const_iterator it = Kbucket::nodeList.begin();
         it != Kbucket::nodeList.end(); ++it)
    {
        if(oldNode == *it)
        {
            Kbucket::mtx.lock();
            if(oldNode == *it)
            {
                Kbucket::nodeList.remove(*it);
                Kbucket::nodeList.push_front(newNode);
                Logger::getInstance().logFormat("ssnsn", Logger::KBUCKET,
                                 "Replaced", &oldNode, "with", &newNode);
                retval = true;
            }
            Kbucket::mtx.unlock();
            break;
        }
    }
    return retval;
}

int Kbucket::getSize() const
{
    return (int)nodeList.size();
}

int Kbucket::serialize(uint8_t out[500])const
{
    //[2 byte] number of nodes in network order
    // ->
    //  [4 byte] ip in network order    \
    //                                   |-> for each node
    //  [2 byte] port in network order  /
    short index = 0;
    uint16_t size_ho = nodeList.size();
    uint16_t size_no = htons(size_ho);
    out[index++] = *((uint8_t*)(&size_no)+0);
    out[index++] = *((uint8_t*)(&size_no)+1);
    uint32_t ip;
    uint16_t port;
    for(std::list<Node>::const_iterator j=nodeList.begin();
        j!=nodeList.end();++j)
    {
        ip = j->getIp().getIp();
        port = htons(j->getPort());
        out[index++] = *((uint8_t*)(&ip)+0);
        out[index++] = *((uint8_t*)(&ip)+1);
        out[index++] = *((uint8_t*)(&ip)+2);
        out[index++] = *((uint8_t*)(&ip)+3);
        out[index++] = *((uint8_t*)(&port)+0);
        out[index++] = *((uint8_t*)(&port)+1);
    }
    return index;
}

Kbucket::Kbucket(const uint8_t serialized[500])
{
    uint16_t size = ntohs(*(uint16_t*)serialized);
    short index = 2;
    uint32_t ip;
    uint16_t port;
    std::vector<Node> vec;
    for(int j=0;j<size;j++)
    {
        ip = *((uint32_t*)(serialized+index));
        index+=4;
        port = ntohs(*((uint16_t*)(serialized+index)));
        index+=2;
        vec.push_back(Node(Ip(ip),port));
    }
    nodeList = std::list<Node>{std::make_move_iterator(std::begin(vec)),
        std::make_move_iterator(std::end(vec))};
}

bool Kbucket::contains(Node* n)const {
    for (std::list<Node>::const_iterator it = Kbucket::nodeList.begin();
         it != Kbucket::nodeList.end(); ++it) {
        if(*n == *it) {
            return true;
        }
    }
    return false;
}

void Kbucket::print()const
{
    std::cout<<"KBucket size: "<<nodeList.size()<<std::endl;
    char ipstring[16];
    
    for(std::list<Node>::const_iterator i=nodeList.begin();
        i!=nodeList.end();++i)
    {
        i->getIp().toString(ipstring);
        std::cout<<"<"<<ipstring<<","<<i->getPort()<<">"<<std::endl;
    }
}
