#include "Kbucket.hpp"

Kbucket::Kbucket() 
{
    Kbucket::nodeList = new std::list<Node>();
}

void Kbucket::add(const Node n)
{
    //std::cout << "add" << std::endl;
    for (std::list<Node>::const_iterator it = Kbucket::nodeList->begin(); it != Kbucket::nodeList->end(); ++it) {
        if(n == *it) { //already present, delete node and put in front
            Kbucket::nodeList->remove(*it);
            Kbucket::nodeList->push_front(*it);
            return;
        }
    }
    
    //if not present, add it
    if (Kbucket::nodeList->size() < KBUCKET_SIZE) 
    {
        Kbucket::nodeList->push_front(n); 
    } 
    else 
    {
        //check if last node is still alive
        Node lastActiveNode = Kbucket::nodeList->back();
        Kbucket::nodeList->pop_back();

        //check ping
        //rpc_ping(lastActiveNode);
    }
   
}

const std::list<Node>* Kbucket::getNodes() {
    return Kbucket::nodeList;
}

void Kbucket::setNodes(std::list<Node>* nodeList) {
    *(Kbucket::nodeList) = *nodeList;
}

int Kbucket::serialize(uint8_t out[500])const
{
#if KBUCKET_SIZE > 83
#error A Kbucket will not fit inside a single UDP datagram
#endif
    short index = 0;
    uint16_t size_ho = nodeList->size();
    uint16_t size_no = htons(size_ho);
    out[index++] = *((uint8_t*)(&size_no)+0);
    out[index++] = *((uint8_t*)(&size_no)+1);
    uint32_t ip;
    uint16_t port;
    for(std::list<Node>::iterator j=nodeList->begin();j!=nodeList->end();++j)
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
    nodeList = new std::list<Node>;
    uint16_t size = ntohs(*(uint16_t*)serialized);
    short index = 2;
    uint32_t ip;
    uint16_t port;
    for(int j=0;j<size;j++)
    {
        ip = *((uint32_t*)(serialized+index));
        index+=4;
        port = ntohs(*((uint16_t*)(serialized+index)));
        index+=2;
        nodeList->push_front(Node(Ip(ip),port));
    }
}

void Kbucket::print()
{
    std::cout<<"KBucket size: "<<nodeList->size()<<std::endl;
    char ipstring[16];
    
    for(std::list<Node>::iterator i=nodeList->begin();i!=nodeList->end();++i)
    {
        i->getIp().toString(ipstring);
        std::cout<<"<"<<ipstring<<","<<i->getPort()<<">"<<std::endl;
    }
}

Kbucket::~Kbucket()
{
    delete nodeList;
}
